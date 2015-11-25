#include "SocketClient.h"
#include "NetManager.h"
#include "AutoFuncScopeLogger.h"

using namespace std;

static wstring GetErrorMessage(const int errCode)
{
	LPSTR errString = NULL;

	int size = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		0,
		errCode,
		0,
		(LPSTR)&errString,
		0,
		0);

	wstringstream wss;
	auto msg = Utils::ToWstring(errString);
	wss << L"Error : [" << errCode << L"] - " << msg.substr(0, msg.length() - 2);

	LocalFree(errString);

	return wss.str();
}

SocketClient::SocketClient(const wstring &serverName, const int port)
{
	INIT_FUNCTION_SCOPE_LOGGER();

	this->serverName = serverName;
	this->connectSocket = INVALID_SOCKET;

	// Initialize Winsock
	NetManager::GetInstance().Init();

	bad_flag = this->serverName.length() == 0;

	if (port == 0)
	{
		LOGD(L"Error : Invalid port [" << port << L"] passed. Using the deault.");
		bad_flag = true;
	}
	else if (port != 80)
	{
		this->port = to_string(port);
	}
}

bool SocketClient::Start()
{
	INIT_FUNCTION_SCOPE_LOGGER();

	if (bad_flag)
	{
		LOGD(L"Error : Server or port is not configured properly. Aborting connection.");
		return false;
	}

	if (NetManager::GetInstance().IsInitialized())
	{
		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;

		// Resolve the server address and port
		auto iResult = getaddrinfo(Utils::ToString(serverName).c_str(), port.c_str(), &hints, &result);
		if (iResult != 0)
		{
			wstringstream wss;
			wss << L"Unable to get local system's network address information. Error code = " << iResult;

			LOGD(L"Error : " << wss.str());
		}
		else
		{
			// Attempt to connect to an address until one succeeds
			for (ptr = result; ptr != NULL; ptr = ptr->ai_next)
			{
				// Create a SOCKET for connecting to server
				connectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
				if (connectSocket == INVALID_SOCKET)
				{
					wstringstream wss;
					wss << L"Unable to create socket connection to server. " << GetErrorMessage(WSAGetLastError());

					LOGD(L"Error : " << wss.str());
					break;
				}

				// Connect to server.
				iResult = connect(connectSocket, ptr->ai_addr, ptr->ai_addrlen);
				if (iResult != SOCKET_ERROR)
				{
					break;
				}

				LOGD(GetErrorMessage(WSAGetLastError()));

				closesocket(connectSocket);
				connectSocket = INVALID_SOCKET;
				if (ptr->ai_next != NULL)
				{
					LOGD(L"Trying next socket");
				}
			}

			freeaddrinfo(result);

			if (connectSocket == INVALID_SOCKET)
			{
				wstringstream wss;
				wss << L"Unable to connect to server [" << serverName << L"].";

				LOGD(L"Error : - " << wss.str());
			}
			else
			{
				// successfully connected.
				return true;
			}
		}
	}
	else
	{
		LOGD(L"Error : NetManager not initialized.");
	}

	return false;
}

bool SocketClient::Stop()
{
	INIT_FUNCTION_SCOPE_LOGGER();

	if (connectSocket != INVALID_SOCKET)
	{
		// shutdown the connection since no more data will be sent
		auto iResult = shutdown(connectSocket, SD_BOTH);
		auto lastError = WSAGetLastError();

		closesocket(connectSocket);

		if (iResult == SOCKET_ERROR)
		{
			wstringstream wss;
			wss << L"Unable to disconnect. " << GetErrorMessage(lastError);

			LOGD(L"Error :" + wss.str());
			return false;
		}
	}
	else
	{
		LOGD(L"Error : Connection not initialized.");

		return false;
	}

	connectSocket = INVALID_SOCKET;

	return true;
}

bool SocketClient::SendBytes(const void* msg, const int length)
{
	INIT_FUNCTION_SCOPE_LOGGER();

	if (connectSocket != INVALID_SOCKET)
	{
		int iResult = send(connectSocket, static_cast<const char*>(msg), length, 0);

		if (iResult == SOCKET_ERROR)
		{
			wstringstream wss;
			wss << L"Unable to send data. " << GetErrorMessage(WSAGetLastError());

			LOGD(L"Error : " + wss.str());
			return false;
		}
	}
	else
	{
		LOGD(L"Error : Connection not initialized.");
		return false;
	}

	return true;
}

SocketClient::~SocketClient()
{
	INIT_FUNCTION_SCOPE_LOGGER();

	if (connectSocket != INVALID_SOCKET)
	{
		Stop();
	}
}
