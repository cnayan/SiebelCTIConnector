#pragma once

#include "ISiebelNotificationHandler.h"

interface ICommandHandler : public ISiebelNotificationHandler
{
protected:
	ICommandHandler()
	{
	}
};
