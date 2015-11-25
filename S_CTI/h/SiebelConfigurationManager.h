#pragma once

#include <string>
#include <list>
#include "Siebel\scapi.h"

using namespace std;

class SiebelConfigurationManager
{
	bool initialized;

protected:
	list<wstring> inList;

	SiebelConfigurationManager();
	virtual void Initialize() = 0; // { }

public:
	virtual ~SiebelConfigurationManager()
	{
	}

	virtual void CopyTo(ISC_StrParamList* const paramList);
};

