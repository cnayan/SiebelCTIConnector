#pragma once

#include <ctime>
#include <string>
#include <list>
#include <vector>
#include <functional>

#include "Siebel\scapi.h"
#include "Logger.h"
#include "CommonDefs.h"

using namespace std;

namespace Utils
{
	wstring ToWstring(const char* const msg);
	wstring ToWstring(const string& msg);
	wstring ToWstring(const ISC_STRING &msg);

	string ToString(const wstring &);
	string ToString(const ISC_STRING &);

	ISC_STRING ToDoubleByte(const wstring& srcStr);

	void CopyToParamList(const list<wstring>& inList, ISC_StrParamList* paramList);
	void CopyToParamList(const vector<long>& inList, ISC_LongParamList* paramList);
	bool iequal(const wstring& s1, const wstring &s2);
	string GetTime();

#define TW(t) (wchar_t*)t
#define T(t) (char*)t
	wstring ToWide(ISC_STRING &msg, int len);

#define LOG_DEBUG Logger::GetInstance().DebugLine

#define BUF_SIZE_256            256

	wstring GetStringW(HMODULE hSource, int ID);
}
