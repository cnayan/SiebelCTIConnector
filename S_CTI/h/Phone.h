#pragma once

#include "ICommandHandler.h"
#include "IEventHandler.h"
#include "Param.h"

#include <vector>

using namespace std;

class Phone
{
	vector<ICommandHandler*> commandHandlers;
	vector<IEventHandler*> eventHandlers;

public:
	Phone();
	~Phone();

	void Initialize();

	bool HandleCommand(const Param&);
};
