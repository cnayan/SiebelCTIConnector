#include "NetManager.h"
#include "AutoFuncScopeLogger.h"

NetManager::SingleInstanceHandler NetManager::sih;

NetManager::NetManager()
{
	INIT_FUNCTION_SCOPE_LOGGER();

	WSAStartupRetCode = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (WSAStartupRetCode != 0)
	{
		LOGD(L"Unable to initialize the network connection (Winsock failure.)");
	}
}

void NetManager::Init()
{
	// Dummy.
}
