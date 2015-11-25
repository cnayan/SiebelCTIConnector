#pragma once

#include "SiebelConfigurationManager.h"

class CommandsSupport : public SiebelConfigurationManager
{
protected:
	void Initialize();

public:
	CommandsSupport();
	~CommandsSupport();
};
