#include "Phone.h"

#include "AnswerCall.h"

#include "AgentLogin.h"
#include "AgentLogout.h"

Phone::Phone()
{
}

Phone::~Phone()
{
}

void Phone::Initialize()
{
	commandHandlers.push_back(new AnswerCall());

	eventHandlers.push_back(new AgentLogin());
	eventHandlers.push_back(new AgentLogout());
}

bool Phone::HandleCommand(const Param& param)
{
	bool handled = false;
	for (auto eventHandler : eventHandlers)
	{
		if (eventHandler->HandleCommand(param))
		{
			handled = true;
			break;
		}
	}

	return handled;
}
