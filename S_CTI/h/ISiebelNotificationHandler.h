#pragma once

#include "Main.h"
#include "Param.h"

interface ISiebelNotificationHandler
{
protected:
	ISiebelNotificationHandler()
	{
	}

public:
	virtual bool HandleCommand(const Param&) = 0;
};
