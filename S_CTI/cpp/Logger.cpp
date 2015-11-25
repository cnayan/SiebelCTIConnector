#include "Logger.h"
#include "Utils.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

mutex Logger::g_mtx;
const string Logger::fileName = "C:/S_CTI.log"; //Just a file location for my Windows OS based PC
wofstream Logger::fs(Logger::fileName.c_str(), ios_base::out | ios_base::app);
Logger::InternalGlobalInstanceHandler Logger::internalInstanceHandler;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

Logger::Logger() {
}

Logger::~Logger() {
	DebugLine(L"Logger shutting down.");
	DebugLine(L"");
	fs.flush();
	fs.close();
}

void Logger::Debug(const wstring& msg) {
	g_mtx.lock();
	fs << Utils::GetTime().c_str() << L" - " << msg;
	g_mtx.unlock();
}

void Logger::DebugLine(const wstring& msg) {
	g_mtx.lock();
	fs << Utils::GetTime().c_str() << L" - {Thread = " << ::GetCurrentThreadId() << L"} - " << msg << endl;
	g_mtx.unlock();
}
