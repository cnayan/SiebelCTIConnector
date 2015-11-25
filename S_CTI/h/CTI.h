#pragma once

#include "Siebel\scapi.h"
#include "CommonDefs.h"

/* Config */
const wchar_t* const STR_DEFAULT_AGENT_AVAIL_STATE = L"DefaultAgentAvailState";

/* Commands */
const wchar_t* const STR_LOGIN = L"LogIn";
const wchar_t* const STR_LOGOUT = L"LogOut";
const wchar_t* const STR_AUTO_IN = L"AUTO_IN";
const wchar_t* const STR_MANUAL_IN = L"MANUAL_IN";

/* Events */
const wchar_t* const STR_EVENT_LOGIN = L"OnLogin";
const wchar_t* const STR_EVENT_LOGOUT = L"OnLogout";

/* Configuration */
const wchar_t* const STR_CONFIGURATION = L"Configuration";
const wchar_t* const STR_CTMW_SERVER_NAME = L"CTIMW_SERVER";
const wchar_t* const STR_CTMW_SERVER_PORT = L"CTIMW_SERVER_PORT";
const wchar_t* const STR_AGENT_ID = L"AgentID";

enum AgentState
{
	UNKNOWN = -1,
	AUTO_IN,
	MANUAL_IN,
	AUX_WORK,
	AFTER_CALL_WORK,
	LOG_OUT
};

//////////////////////////////////////////////////////////////////////////////////////////////
// Non SCAPI functions
//////////////////////////////////////////////////////////////////////////////////////////////
ISC_RESULT GetCommandStatus(
	/* in  */ ISC_SERVICE_HANDLE pHandle,
	/* in  */ const ISC_STRING name,
	/* out */ long* status
	);

void FreeSCLongParamList(/* in  */ struct ISC_LongParamList &longList);
