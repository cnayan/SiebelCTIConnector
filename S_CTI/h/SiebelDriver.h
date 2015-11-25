#pragma once

#include <string>
#include <vector>

using namespace std;

#include "AgentService.h"

class SiebelDriver
{
	wstring configName;
	vector<AgentService*> agents;

public:
	SiebelDriver(const wstring &configNameString);
	~SiebelDriver();

	wstring GetConfigName() const;
	bool IsConnectedToMiddlewareService() const;

	void AddAgent(AgentService* pAgentService);
	void RemoveAgent(AgentService* pAgentService);

	bool Manages(const AgentService* const pAgentService) const;
};
