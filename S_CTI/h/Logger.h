#pragma once

#include <string>
#include <fstream>
#include <locale>
#include <mutex>

using namespace std;

class Logger
{
	static mutex g_mtx;
	static const string fileName;
	static wofstream fs;

	class InternalGlobalInstanceHandler
	{
		Logger *pInstance;

	public:
		~InternalGlobalInstanceHandler()
		{
			if (pInstance)
			{
				delete pInstance;
			}
		}

		Logger* GetInstance()
		{
			if (!pInstance)
			{
				pInstance = new Logger();
			}
			return pInstance;
		}
	};

	static InternalGlobalInstanceHandler internalInstanceHandler;

	Logger();

public:
	~Logger();

	static Logger& GetInstance()
	{
		return *internalInstanceHandler.GetInstance();
	}

	void Debug(const wstring& msg);
	void DebugLine(const wstring& msg);
};
