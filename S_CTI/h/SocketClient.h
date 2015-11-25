#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <algorithm>
#include <functional>
#include <ws2tcpip.h>
#include <winsock2.h>

// link with Ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define DEFAULT_PORT "http" //80

#include "Utils.h"

using namespace std;

class SocketClient
{
public:
	typedef function<void(const wstring &msg)> ErrorEventHandler;
	typedef function<void()> ConnectEventHandler;

private:
	WSADATA wsaData;
	SOCKET connectSocket = INVALID_SOCKET;
	struct addrinfo *result = NULL, *ptr, hints;
	wstring serverName;
	string port = DEFAULT_PORT;

	bool bad_flag = false;

public:
	SocketClient(const wstring &servername, const int port);
	bool Start();
	bool Stop();
	bool SendBytes(const void *data, const int length);


	~SocketClient();
};
