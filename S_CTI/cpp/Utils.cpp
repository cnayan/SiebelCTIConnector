#include "Utils.h"
#include "AutoFuncScopeLogger.h"

#include <malloc.h>
#include <string.h>
#include <sstream>

#include "utf8.h"

using namespace std;

namespace Utils {
	wstring ToWstring(const char* const msg) {
		std::wstringstream wss;
		wss << msg;
		return wss.str();
	}

	wstring ToWstring(const string& msg) {
		wstringstream wss;
		wss << msg.c_str();
		return wss.str();
	}

	wstring ToWstring(const ISC_STRING &msg) {
		return wstring(TW(msg));
	}

	string ToString(const wstring &msg) {
		string str(msg.begin(), msg.end());
		return str;
	}

	string ToString(const ISC_STRING &msg) {
		return ToString(ToWstring(msg));
	}

	wstring ToWide(ISC_STRING &msg, int len)
	{
		string narrow;
		utf8::utf16to8(msg, msg + len, back_inserter(narrow));
		return Utils::ToWstring(narrow);
	}


	ISC_STRING ToDoubleByte(const wstring& wSrcStr)
	{
		INIT_FUNCTION_SCOPE_LOGGER();

		const int NULL_PAD = 1;

		ISC_STRING dest = new ISC_CHAR[wSrcStr.length() + NULL_PAD];

		wcsncpy(TW(dest), wSrcStr.c_str(), wSrcStr.length());

		return dest;
	}

	void CopyToParamList(const list<wstring>& inList, ISC_StrParamList* paramList)
	{
		INIT_FUNCTION_SCOPE_LOGGER();
		try
		{
			paramList->len = inList.size();

			paramList->dataItems = new ISC_STRING[paramList->len];

			int i = 0;
			for (auto str : inList)
			{
				paramList->dataItems[i] = ToDoubleByte(str);
				LOGD("Setting parameter " << Utils::ToWide(paramList->dataItems[i], str.length()));
				i++;
			}
		}
		catch (exception ex)
		{
			LOGD(L"Exception - " << ToWstring(ex.what()));
		}
		catch (...)
		{
			LOGD(L"Unknown exception");
		}
	}

	void CopyToParamList(const vector<long>& inList, ISC_LongParamList* const paramList)
	{
		INIT_FUNCTION_SCOPE_LOGGER();
		try
		{
			paramList->len = inList.size();
			paramList->dataItems = new long[paramList->len];

			int i = 0;
			for (auto num : inList)
			{
				paramList->dataItems[i] |= num;
				i++;
			}
		}
		catch (exception ex)
		{
			LOGD(L"Exception - " << ToWstring(ex.what()));
		}
		catch (...)
		{
			LOGD(L"Unknown exception");
		}
	}

	/// Compares two string ignoring the case
	bool iequal(const wstring& s1, const wstring &s2)
	{
		return equal(s1.begin(), s1.end(), s2.begin(),
			[](wchar_t a, wchar_t b) { return towupper(a) == towupper(b); }
		);
	}

	string GetTime()
	{
		time_t now = time(0);

#if defined(__clang__)
		string dt(ctime(&now));
#elif defined(__GNUC__) || defined(__GNUG__)
		string dt(ctime(&now));
#elif defined(_MSC_VER)
		char dt_[128];
		ctime_s(dt_, sizeof dt_, &now);
		string dt(dt_);
#endif
		dt = dt.substr(0, dt.length() - 1);

		return dt;
	}

	wstring GetStringW(HMODULE hSource, int ID)
	{
		HMODULE hModule;
		WCHAR LoadStringData[BUF_SIZE_256];

		hModule = hSource;
		if (hSource == NULL)
		{
			hModule = ::GetModuleHandle(NULL);
		}

		::LoadStringW(hModule, (UINT)ID, LoadStringData, sizeof(WCHAR) * BUF_SIZE_256);

		return wstring(LoadStringData);
	}
}
