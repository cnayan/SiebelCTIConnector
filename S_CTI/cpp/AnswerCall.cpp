#include "AnswerCall.h"
#include "CTI.h"

AnswerCall::AnswerCall()
{
}

AnswerCall::~AnswerCall()
{
}

bool AnswerCall::HandleCommand(const Param& param)
{
	return false; //param.GetCommandName() == MAKE_CALL;
}
