#include "CTI.h"
#include "Main.h"
#include "Utils.h"
#include "CommandsSupport.h"
#include "EventsSupport.h"
#include "Logger.h"
#include "SiebelDriver.h"
#include "AutoFuncScopeLogger.h"
#include "ResourceManager.h"
#include "AgentService.h"
#include "Param.h"
#include "CTIMWConnector.h"

#include "cpplinq.hpp"
#include "utf8.h"

#include <vector>
#include <sstream>
#include <map>
#include <thread>

using namespace std;
using namespace cpplinq;

// ---------------------------------------------------------------------------------------

mutex g_SiebelDriverCreationMutex, g_AgentServiceCreationMutex;

CommandsSupport commandSupport;
EventsSupport eventsSupport;

list<SiebelDriver*> siebelDrivers;

static wstring ctimwServerName;
static int ctimwServerPort;

// ---------------------------------------------------------------------------------------

#define EXEC_CODE_DRIVER_IF_PRESENT(pSiebelDriveHandle,trueCode,falseCode) \
SiebelDriver* pSiebelDriver = static_cast<SiebelDriver*>(from(siebelDrivers) >> first_or_default([&](const SiebelDriver* const pSiebelDriver) { return pSiebelDriver == pSiebelDriveHandle; }));\
if (pSiebelDriver != NULL) {\
	trueCode\
}\
else {\
	falseCode\
}

#define EXEC_CODE_AGENT_IF_PRESENT(pAgentServiceHandle,trueCode,falseCode)\
AgentService* pAgentService = NULL; SiebelDriver* pSiebelDriver = NULL;\
for (auto siebelDriver : siebelDrivers)\
{\
	if (siebelDriver->Manages(static_cast<AgentService*>(pAgentServiceHandle))) {\
		pSiebelDriver = siebelDriver;\
		pAgentService = static_cast<AgentService*>(pAgentServiceHandle);\
		break; }\
}\
if (pAgentService != NULL) {\
	trueCode\
}\
else {\
	falseCode\
}

// ---------------------------------------------------------------------------------------

/*
	Connects to the windows service running on the server specified.
*/
void ConnectToCTIMiddleWare()
{
	INIT_FUNCTION_SCOPE_LOGGER();

	if (ctimwServerName.length() == 0)
	{
		LOGD(L"Error : Invalid server name specified. Functionalities will be disabled.");
	}
	else
	{
		CTIMWConnector::GetInstance().Connect(ctimwServerName, ctimwServerPort);
	}
}

EXTERN_C ISCAPI long APIVersion()
{
	INIT_FUNCTION_SCOPE_LOGGER();

	LOGD(L"SCAPI_VERSION = " << SCAPI_VERSION);

	return SCAPI_VERSION;
}


EXTERN_C ISCAPI ISC_RESULT CreateISCDriverInstance(
	/* in  */ const ISC_STRING mediaTypeStr,
	/* in  */ const ISC_STRING languageCode,
	/* in  */ const ISC_STRING connectString,
	/* in  */ const struct ISC_KVParamList* datasetParams,
	/* out */ ISC_DRIVER_HANDLE* pHandle
	)
{
	INIT_FUNCTION_SCOPE_LOGGER();

	if (sizeof(wchar_t) != sizeof(ISC_CHAR))
	{
		LOGD("====CRITICAL==== : Size of wide char is not equal to " << sizeof(ISC_CHAR) << L" bytes, as required by Siebel. Will not proceed.");
		return SC_EC_ERROR;
	}

	ISC_RESULT iscResult = SC_EC_OK;

	g_SiebelDriverCreationMutex.lock();

	wstring configName;

	SiebelDriver* pSiebelDriver = NULL;

	auto kv =
		from_iterators(datasetParams->dataItems, datasetParams->dataItems + datasetParams->len)
		>> first_or_default([](const ISC_KeyValue &kv) { return Utils::iequal(TW(kv.paramName), STR_CONFIGURATION); }
	);

	if (kv.paramName && Utils::ToWstring(kv.paramName).length() > 0)
	{
		configName = TW(kv.paramValue);
		LOGD(L"Configuration Name [" << configName << L"] passed from Siebel.");
	}

	kv =
		from_iterators(datasetParams->dataItems, datasetParams->dataItems + datasetParams->len)
		>> first_or_default([](const ISC_KeyValue &kv) { return Utils::iequal(TW(kv.paramName), STR_CTMW_SERVER_NAME); }
	);

	if (kv.paramName && Utils::ToWstring(kv.paramName).length() > 0)
	{
		ctimwServerName = TW(kv.paramValue);
		LOGD(L"CTIMW server [" << ctimwServerName << L"] passed from Siebel.");
	}

	kv =
		from_iterators(datasetParams->dataItems, datasetParams->dataItems + datasetParams->len)
		>> first_or_default([](const ISC_KeyValue &kv) { return Utils::iequal(TW(kv.paramName), STR_CTMW_SERVER_PORT); }
	);

	if (kv.paramName && Utils::ToWstring(kv.paramName).length() > 0)
	{
		ctimwServerPort = std::stoi(TW(kv.paramValue));
		LOGD(L"CTIMW server port [" << TW(kv.paramValue) << L"] passed from Siebel.");
	}

	if (configName.length() != 0)
	{
		pSiebelDriver = from(siebelDrivers)
			>> first_or_default([&](const SiebelDriver* const pDriver) { return Utils::iequal(pDriver->GetConfigName(), configName); });

		if (pSiebelDriver != NULL)
		{
			LOGD(L"Configuration Name [" << configName << L"] found among existing drivers.");
		}
		else
		{
			pSiebelDriver = new SiebelDriver(configName);
			if (pSiebelDriver == NULL)
			{
				iscResult = SC_EC_ERROR;
				LOGD(L"Unable to create SiebelDriver object. Insufficient memory.");
			}
			else
			{
				LOGD(L"Driver object created. Handle = " << pSiebelDriver);

				// The following call sets the language for the messages to Siebel
				ResourceManager::GetInstance().LoadLanguageLibrary(languageCode);

				LOGD(L"Driver initialized.");

				*pHandle = static_cast<ISC_SERVICE_HANDLE>(pSiebelDriver);

				siebelDrivers.push_back(pSiebelDriver);

				LOGD(L"Handle = " << *pHandle << L", ConfigName = " << configName);

				// Start the connection synchronously
				ConnectToCTIMiddleWare();
			}
		}
	}
	else
	{
		iscResult = SC_EC_ERROR;
		LOGD(L"ERROR: Configuration parameter not passed.");
	}

	g_SiebelDriverCreationMutex.unlock();

	return iscResult;
}

EXTERN_C ISCAPI ISC_RESULT ReleaseISCDriverInstance(
	/* in */ ISC_DRIVER_HANDLE pHandle
	)
{
	INIT_FUNCTION_SCOPE_LOGGER();

	ISC_RESULT iscResult = SC_EC_OK;

	g_SiebelDriverCreationMutex.lock();

	EXEC_CODE_DRIVER_IF_PRESENT(pHandle,
	{
		LOGD(L"Driver found. Handle = " << pHandle);
		siebelDrivers.remove(pSiebelDriver);
		delete pSiebelDriver;
	},
	{
		LOGD(L"Invalid handle = " << pHandle);
		iscResult = SC_EC_ERROR;
	});

	g_SiebelDriverCreationMutex.unlock();

	return iscResult;
}

EXTERN_C ISCAPI ISC_RESULT RequestService(
	/* in  */ ISC_DRIVER_HANDLE pHandle,
	/* in  */ const struct ISC_CLIENT_INTERFACE clientInterface,
	/* in  */ const ISC_STRING connectString,
	/* in  */ const struct ISC_KVParamList* datasetParams,
	/* out */ ISC_SERVICE_HANDLE* pAgentServiceHandle
	)
{
	INIT_FUNCTION_SCOPE_LOGGER();

	g_AgentServiceCreationMutex.lock();

	ISC_RESULT iscResult = SC_EC_OK;
	if (pHandle == NULL)
	{
		LOGD(L"Driver handle = " << pHandle << L". Null pointer passed.");
		iscResult = SC_EC_SERVICE_CREATION_ERR;
	}
	else
	{
		EXEC_CODE_DRIVER_IF_PRESENT(pHandle,
		{
			if (!CTIMWConnector::GetInstance().IsConnected())
			{
				wstring msg = ResourceManager::GetInstance().GetString(IDS_ERROR_CONNECT_MW);
				try
				{
					HandleError_Helper(clientInterface, 0, (ISC_STRING)msg.c_str());
				}
				catch (...)
				{
					LOGD(L"General Exception : " << msg);
				}

				LOGD(L"Driver handle = " << pHandle << L" - " << msg);
				iscResult = SC_EC_SERVICE_CREATION_ERR;
			}
			else
			{
				wstring agentId;
				auto kv =
					from_iterators(datasetParams->dataItems, datasetParams->dataItems + datasetParams->len)
					>> first_or_default([](const ISC_KeyValue &kv) { return Utils::iequal(TW(kv.paramName), STR_AGENT_ID); }
				);

				if (kv.paramName && Utils::ToWstring(kv.paramName).length() > 0)
				{
					agentId = TW(kv.paramValue);
					LOGD(L"Agent Id = [" << agentId << L"] passed from Siebel.");

					auto pAgentService = new AgentService(agentId, clientInterface);
					*pAgentServiceHandle = pAgentService;
					pSiebelDriver->AddAgent(pAgentService);
					pAgentService->Initialize(datasetParams);

					LOGD(L"Agent service object created. Agent = " << agentId << L", ServiceHandle = " << pAgentService
						<< L", Driver handle = " << pHandle);

					// Dummy. You should actually pass the "login" event to your PBX and get acknowledgement from it,
					// and then possibly update the Siebel UI.
					pAgentService->UpdateSiebelUI();
				}
				else
				{
					LOGD(L"Agent service initialization failed.");
					iscResult = SC_EC_SERVICE_CREATION_ERR;
				}
			}
		},
		{
			LOGD(L"Invalid handle " << pHandle << L" passed. Driver may have been deleted.");
			iscResult = SC_EC_SERVICE_CREATION_ERR;
		});
	}

	g_AgentServiceCreationMutex.unlock();

	return iscResult;
}

EXTERN_C ISCAPI ISC_RESULT ReleaseISCServiceInstance(
	/* in */ ISC_SERVICE_HANDLE pAgentServiceHandle
	)
{
	INIT_FUNCTION_SCOPE_LOGGER();

	ISC_RESULT iscResult = SC_EC_OK;

	g_AgentServiceCreationMutex.lock();

	EXEC_CODE_AGENT_IF_PRESENT(pAgentServiceHandle,
	{
		LOGD(L"Agent service found. Handle = " << pAgentServiceHandle);
		pSiebelDriver->RemoveAgent(pAgentService);
	},
	{
		LOGD(L"Invalid handle = " << pAgentServiceHandle);
		iscResult = SC_EC_SERVICE_RELEASE_ERR;
	});

	g_AgentServiceCreationMutex.unlock();

	return SC_EC_OK;
}

EXTERN_C ISCAPI ISC_RESULT InvokeCommand(
	/* in  */ ISC_SERVICE_HANDLE pAgentServiceHandle,
	/* in  */ const ISC_STRING clntCmdTrackID,
	/* in  */ const ISC_STRING requestName,
	/* in  */ const ISC_STRING stringParam,
	/* in  */ const struct ISC_KVParamList* datasetParam
	)
{
	INIT_FUNCTION_SCOPE_LOGGER();

	ISC_RESULT iscResult = SC_EC_OK;

	EXEC_CODE_AGENT_IF_PRESENT(pAgentServiceHandle,
	{
		LOGD(L"Agent service found. Handle = " << pAgentServiceHandle);

		auto pParam = new Param(JT_COMMAND, datasetParam, requestName);

		// Dummy
		pAgentService->ProcessCommand(pParam);
	},
	{
		LOGD(L"Invalid handle = " << pAgentServiceHandle);
		iscResult = SC_EC_SERVICE_RELEASE_ERR;
	});

	return iscResult;
}

EXTERN_C ISCAPI ISC_RESULT RequestCommandEventList(
/* in  */ const ISC_STRING mediaTypeStr,
/* out */ struct ISC_StrParamList* commandList,
/* out */ struct ISC_StrParamList* eventList
	)
{
	INIT_FUNCTION_SCOPE_LOGGER();

	// Commands
	LOG_DEBUG(L"Setting supported commands");
	commandSupport.CopyTo(commandList);

	// Events
	LOG_DEBUG(L"Setting supported events");
	eventsSupport.CopyTo(eventList);

	return SC_EC_OK;
}

ISC_RESULT GetCommandDescription(
	/* in  */ ISC_SERVICE_HANDLE pHandle,
	/* in  */ const ISC_STRING commandName,
	/* out */ ISC_STRING* description
	)
{
	INIT_FUNCTION_SCOPE_LOGGER();

	// Dummy
	LOGD(L"Command [" << TW(commandName) << L"] description requested.");

	return SC_EC_OK;
}

ISC_RESULT GetCommandStatus(
	/* in  */ ISC_SERVICE_HANDLE pHandle,
	/* in  */ const ISC_STRING name,
	/* out */ long* status
	)
{
	INIT_FUNCTION_SCOPE_LOGGER();

	*status |= SC_CF_STRPARAMSOK;

	return SC_EC_OK;
}

ISC_RESULT SiebelHandleEvent()
{
	return SC_EC_OK;
}

ISCAPI void FreeSCStrParamList(
/* in  */ struct ISC_StrParamList strList
	)
{
	INIT_FUNCTION_SCOPE_LOGGER();

	if (strList.dataItems)
	{
		for (long i = 0; i < strList.len; i++)
		{
			if (strList.dataItems[i])
			{
				delete [] strList.dataItems[i];
			}

			strList.dataItems[i] = NULL;
		}

		delete [] strList.dataItems;
	}

	strList.dataItems = NULL;
	strList.len = 0;
}

/////////////////////////////////////////////////////////////////////////////
//   To release the memory which is initially allocated at the driver side.
//   Associated w/ sending toolbar status to Siebel.
/////////////////////////////////////////////////////////////////////////////
void FreeSCLongParamList(
/* in  */struct ISC_LongParamList &longList
)
{
	INIT_FUNCTION_SCOPE_LOGGER();

	if (longList.dataItems)
	{
		delete[] longList.dataItems;
	}

	longList.dataItems = NULL;
	longList.len = 0;
}

