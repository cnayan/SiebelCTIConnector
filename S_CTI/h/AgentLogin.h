#include "IEventHandler.h"

class AgentLogin : public IEventHandler
{
public:
	bool HandleCommand(const Param&);
};
