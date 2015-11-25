#include "SiebelDriver.h"

#include "Main.h"
#include "Utils.h"
#include "AutoFuncScopeLogger.h"

#include <sstream>

#include "cpplinq.hpp"

using namespace std;

SiebelDriver::SiebelDriver(const wstring &configNameString)
	: configName(configNameString)
{
}

SiebelDriver::~SiebelDriver()
{
	INIT_FUNCTION_SCOPE_LOGGER();

	for (auto pAS : agents)
	{
		delete pAS;
	}

	agents.clear();
}

wstring SiebelDriver::GetConfigName() const
{
	return configName;
}

void SiebelDriver::AddAgent(AgentService* pAgentService)
{
	if (pAgentService != NULL)
	{
		agents.push_back(pAgentService);
	}
}

void SiebelDriver::RemoveAgent(AgentService* pAgentService)
{
	if (pAgentService != NULL)
	{
		auto it = std::find(agents.begin(), agents.end(), pAgentService);
		if (it != agents.end())
		{
			// Mark the agent service object as defunct, but don't release
			// from collection, or it will be memory leak.
			//agents.erase(it);

			// TODO : Mark agent service defunct.
		}
	}
}

bool SiebelDriver::Manages(const AgentService* const pAgentService) const
{
	AgentService* pAS = cpplinq::from(agents)
		>> cpplinq::first_or_default([&](const AgentService* const pAgentService_) { return pAgentService_ == pAgentService; });

	return pAS != NULL;
}
