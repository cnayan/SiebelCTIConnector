#include "AgentLogout.h"
#include "CTI.h"
#include "Utils.h"

bool AgentLogout::HandleCommand(const Param& param)
{
	return param.GetJobType() == JobType::JT_EVENT && param.GetCommandName() == STR_EVENT_LOGOUT;
}
