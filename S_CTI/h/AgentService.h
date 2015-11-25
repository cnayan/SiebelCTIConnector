#pragma once

#include "Param.h"
#include "Phone.h"
#include "CTI.h"

#include "Siebel\scapi.h"

#include <string>

using namespace std;

class AgentService
{
	Phone phone;
	wstring agentId;
	ISC_CLIENT_INTERFACE clientInterface;
	AgentState agentState = eUNKNOWN;

public:
	AgentService(const wstring& agentId, ISC_CLIENT_INTERFACE clientInterface);
	~AgentService();
	bool ProcessCommand(Param *pParam);
	void Initialize(const struct ISC_KVParamList* datasetParams);
	void UpdateSiebelUI();

	inline ISC_CLIENT_INTERFACE& GetClientInterface()
	{
		return clientInterface;
	}
};
