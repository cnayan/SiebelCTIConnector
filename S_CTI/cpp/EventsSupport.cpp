#include "EventsSupport.h"
#include "CTI.h"
#include "AutoFuncScopeLogger.h"

EventsSupport::EventsSupport()
{
}

EventsSupport::~EventsSupport()
{
}

void EventsSupport::Initialize()
{
	INIT_FUNCTION_SCOPE_LOGGER();

	inList.push_back(STR_EVENT_LOGIN);
	inList.push_back(STR_EVENT_LOGOUT);
}
