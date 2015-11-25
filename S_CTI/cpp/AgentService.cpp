#include "AgentService.h"
#include "Utils.h"
#include "ResourceManager.h"
#include "resource.h"
#include "AutoFuncScopeLogger.h"

#include "cpplinq.hpp"

using namespace cpplinq;

AgentService::AgentService(const wstring& agentId, ISC_CLIENT_INTERFACE clientInterface)
	: agentId(agentId), clientInterface(clientInterface)
{
	INIT_FUNCTION_SCOPE_LOGGER();

	// TODO : set the phone number
	phone.Initialize();

	agentState = UNKNOWN;
}

AgentService::~AgentService()
{
	INIT_FUNCTION_SCOPE_LOGGER();
}

void AgentService::Initialize(const struct ISC_KVParamList* datasetParams)
{
	INIT_FUNCTION_SCOPE_LOGGER();

	wstring agentId;

	LOGD(L"Looking for configuration [" << STR_DEFAULT_AGENT_AVAIL_STATE << L"]");

	for (auto i = 0; i < datasetParams->len; i++)
	{
		auto kv = datasetParams->dataItems[i];

		LOGD(L"Reading value of [" << TW(kv.paramName) << L"] = [" << TW(kv.paramValue) << L"]");

		if (Utils::iequal(TW(kv.paramName), STR_DEFAULT_AGENT_AVAIL_STATE))
		{
			LOGD(L"Found [" << STR_DEFAULT_AGENT_AVAIL_STATE << L"] = [" << TW(kv.paramValue) << L"]");

			auto val = TW(kv.paramValue);
			if (Utils::iequal(val, STR_AUTO_IN))
			{
				agentState = AUTO_IN;
			}
			else if (Utils::iequal(val, STR_MANUAL_IN))
			{
				agentState = MANUAL_IN;
			}

			LOGD(L"State = [" << val << L"] = [" << agentState << L"]");

			break;
		}
	}
}

bool AgentService::ProcessCommand(Param *pParam)
{
	INIT_FUNCTION_SCOPE_LOGGER();

	return phone.HandleCommand(*pParam);
}

void AgentService::UpdateSiebelUI()
{
	INIT_FUNCTION_SCOPE_LOGGER();

	list<wstring> commands;
	list<wstring> commandDescriptions;
	vector<long> commandStatuses;

	ISC_StrParamList siebelCommandNames;
	ISC_StrParamList siebelCommandDescriptions;
	ISC_LongParamList siebelCommandStatuses;

	ISC_KVParamList datasetParam;

	try
	{
		bool bIsNotReady = false;		// Agent is not available to take a call
		bool bTalking = false;			// Agent is talking

/*
		//////// Samples /////////
		// AutoIn
		commands.push_back(STR_AUTO_IN);
		commandDescriptions.push_back(ResourceManager::GetInstance().GetString(IDS_MESSAGE_CHANGETOAUTOIN));
		commandStatuses.push_back(agentState == AUTO_IN ? SC_CF_DISABLED : SC_CF_NOPARAMSOK);

		// ManualIn
		commands.push_back(STR_MANUAL_IN);
		commandDescriptions.push_back(ResourceManager::GetInstance().GetString(IDS_MESSAGE_CHANGETOMANUALIN));
		commandStatuses.push_back(agentState == MANUAL_IN ? SC_CF_DISABLED : SC_CF_NOPARAMSOK);
*/
		/////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Login
		commands.push_back(STR_LOGIN);
		commandDescriptions.push_back(ResourceManager::GetInstance().GetString(IDS_MESSAGE_LOGIN));
		if (agentState == eNON_ACD)
		{
			commandStatuses.push_back(SC_CF_DISABLED);
		}
		else if (agentState != eLOG_OUT && agentState != eNON_ACD)
		{
			commandStatuses.push_back(SC_CF_NOPARAMSOK | SC_CF_DISABLED);
		}
		else
		{
			commandStatuses.push_back(SC_CF_NOPARAMSOK);
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Logout
		commands.push_back(STR_LOGOUT);
		commandDescriptions.push_back(ResourceManager::GetInstance().GetString(IDS_MESSAGE_LOGIN));
		if (agentState == eNON_ACD)
		{
			commandStatuses.push_back(SC_CF_DISABLED);
		}
		else if (agentState != eLOG_OUT && agentState != eNON_ACD)
		{
			commandStatuses.push_back(SC_CF_NOPARAMSOK);
		}
		else
		{
			commandStatuses.push_back(SC_CF_NOPARAMSOK | SC_CF_DISABLED);
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		siebelCommandNames.len = siebelCommandDescriptions.len = siebelCommandStatuses.len = commands.size();

		LOGD(L"Setting up commands for Siebel");

		Utils::CopyToParamList(commands, &siebelCommandNames);
		Utils::CopyToParamList(commandDescriptions, &siebelCommandDescriptions);
		Utils::CopyToParamList(commandStatuses, &siebelCommandStatuses);

		{
			try
			{
				CacheCommandInformation_Helper(clientInterface, &siebelCommandNames, &siebelCommandDescriptions, &siebelCommandStatuses);
			}
			catch (...)
			{
				LOGD(L"General Exception - Agent = " << agentId);
			}
		}

		datasetParam.len = 2;

		// Agent talk status
		datasetParam.dataItems = (ISC_KeyValue*)malloc(sizeof(ISC_KeyValue) * (datasetParam.len));

		wstring tmpAgentBuf = to_wstring(SC_OP_ISNOTREADY);
		datasetParam.dataItems[0].paramName = Utils::ToDoubleByte(tmpAgentBuf);
		datasetParam.dataItems[0].paramValue = Utils::ToDoubleByte(bIsNotReady ? L"TRUE" : L"FALSE");

		LOGD(L"Setting IsNotReady to " << (bIsNotReady ? L"TRUE" : L"FALSE") << L". Agent = " << agentId);

		LOGD(L"Agent State = [" << agentState << L"]");

		wstring sTemp = L"The Agent State is : (";
		sTemp += agentState + L") ";

		switch (agentState)
		{
			case AUTO_IN:
				sTemp += L"AUTO_IN";
				break;
			case MANUAL_IN:
				sTemp += L"MANUAL_IN";
				break;
			case AUX_WORK:
				sTemp += L"AUX_WORK";
				break;
			case AFTER_CALL_WORK:
				sTemp += L"AFTER_CALL_WORK";
				break;
			case LOG_OUT:
				sTemp += L"LOG_OUT";
				break;
		}

		sTemp += L", Agent = ";
		sTemp += agentId;

		LOGD(sTemp);

		tmpAgentBuf = to_wstring(SC_OP_ISBUSY);
		datasetParam.dataItems[1].paramName = Utils::ToDoubleByte(tmpAgentBuf);
		datasetParam.dataItems[1].paramValue = Utils::ToDoubleByte(bTalking ? L"TRUE" : L"FALSE");

		{
			try
			{
				UpdateObjectInformation_Helper(clientInterface, // interface handle
					0, // ID
					(ISC_STRING)agentId.c_str(),
					&datasetParam);
			}
			catch (...)
			{
				LOGD(L"General Exception - Agent = " << agentId);
			}
		}
	}
	catch (...)
	{
		LOGD(L"General Exception - Agent = " << agentId);
	}

	// Free allocated memory
	FreeSCStrParamList(siebelCommandNames);
	FreeSCStrParamList(siebelCommandDescriptions);
	FreeSCLongParamList(siebelCommandStatuses);

	if (datasetParam.dataItems)
	{
		for (long l = 0; l < datasetParam.len; l++)
		{
			free(datasetParam.dataItems[l].paramName);
			free(datasetParam.dataItems[l].paramValue);
		}
		free(datasetParam.dataItems);
	}
}

