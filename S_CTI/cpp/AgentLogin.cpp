#include "AgentLogin.h"
#include "CTI.h"

bool AgentLogin::HandleCommand(const Param& param)
{
	return param.GetJobType() == JT_EVENT && param.GetCommandName() == STR_EVENT_LOGIN;
}
