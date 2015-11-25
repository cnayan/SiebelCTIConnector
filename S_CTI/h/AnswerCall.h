#pragma once

#include "ICommandHandler.h"

class AnswerCall : public ICommandHandler
{
public:
	AnswerCall();
	~AnswerCall();

	bool HandleCommand(const Param&);
};
