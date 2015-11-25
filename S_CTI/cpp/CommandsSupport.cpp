#include "CommandsSupport.h"
#include "CTI.h"
#include "AutoFuncScopeLogger.h"

CommandsSupport::CommandsSupport()
{
}

CommandsSupport::~CommandsSupport()
{
}

void CommandsSupport::Initialize()
{
	INIT_FUNCTION_SCOPE_LOGGER();

	inList.push_back(STR_LOGIN);
	inList.push_back(STR_LOGOUT);
}
