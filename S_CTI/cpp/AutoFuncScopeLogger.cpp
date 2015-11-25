#include "AutoFuncScopeLogger.h"
#include "Utils.h"

using namespace std;

AutoFuncScopeLogger::AutoFuncScopeLogger(const char* const fname, int lineno, const char* const functionName)
	: fileName(Utils::ToWstring(fname)), functionName(Utils::ToWstring(functionName))
{
	wstringstream wss;
	wss << this->fileName << L" - " << this->functionName << L" : Entering function.";
	LOG_DEBUG(wss.str());
}

AutoFuncScopeLogger::~AutoFuncScopeLogger()
{
	wstringstream wss;
	wss << this->fileName << L" - " << this->functionName << L" : Returning from function.";
	LOG_DEBUG(wss.str());
}

void AutoFuncScopeLogger::WriteLine(const wstring& msg)
{
	wstringstream wss;
	wss << this->fileName << L" - " << this->functionName << L" : " << msg;
	LOG_DEBUG(wss.str());
}
