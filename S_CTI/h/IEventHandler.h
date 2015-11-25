#pragma once

#include "ISiebelNotificationHandler.h"

interface IEventHandler : public ISiebelNotificationHandler
{
protected:
	IEventHandler()
	{
	}
};
