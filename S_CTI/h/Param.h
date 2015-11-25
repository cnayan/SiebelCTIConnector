#pragma once

#include "Siebel\scapi.h"

#include <map>
#include <string>

using namespace std;

enum JobType
{
	JT_UNKNOWN = 0,
	JT_COMMAND = 1,
	JT_EVENT = 2,
	JT_CONF = 3
};

class Param
{
	JobType jt;
	map<wstring, wstring> list;
	wstring commandName;

public:
	Param(JobType jt, const struct ISC_KVParamList*, const ISC_STRING& requestName);

	inline wstring GetCommandName() const
	{
		return commandName;
	}

	inline JobType GetJobType() const
	{
		return jt;
	}
};