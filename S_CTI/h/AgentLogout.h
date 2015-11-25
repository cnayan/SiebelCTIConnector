#include "IEventHandler.h"

class AgentLogout : public IEventHandler
{
public:
	bool HandleCommand(const Param&);
};
