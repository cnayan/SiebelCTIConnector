#pragma once

#include <fstream>
#include <string>
#include <sstream>

using namespace std;

class AutoFuncScopeLogger
{
	wstring fileName, functionName;

public:
	AutoFuncScopeLogger(const char* const fname, int lineno, const char* const functionName);
	~AutoFuncScopeLogger();

	void WriteLine(const wstring& msg);
};

#define INIT_FUNCTION_SCOPE_LOGGER() AutoFuncScopeLogger ____al____(__FILE__, __LINE__, __FUNCTION__)

#define LOGD(msg) {\
wstringstream wss________;\
wss________ << L"[" << __LINE__ << L"] - " << msg;\
____al____.WriteLine(wss________.str());\
}
