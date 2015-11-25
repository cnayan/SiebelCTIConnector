#pragma once

#include "SiebelConfigurationManager.h"

class EventsSupport : public SiebelConfigurationManager
{
protected:
	void Initialize();

public:
	EventsSupport();
	~EventsSupport();
};
