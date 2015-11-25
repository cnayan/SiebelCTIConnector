#include "CTIMWConnector.h"
#include "SocketClient.h"

CTIMWConnector::SingleInstanceHandler CTIMWConnector::sih;

void CTIMWConnector::Connect(const wstring &serverName, int port)
{
	if (!connected)
	{
		pSocketClient = new SocketClient(serverName, port);
		if (pSocketClient->Start())
		{
			connected = true;
		}
	}
}

void CTIMWConnector::Disconnect()
{
	if (connected)
	{
		connected = false;
		delete pSocketClient;
		pSocketClient = NULL;
	}
}

void CTIMWConnector::SendBytes(const void *data, int length)
{
	if (connected)
	{
		pSocketClient->SendBytes(data, length);
	}
}

bool CTIMWConnector::IsConnected() const
{
	return connected;
}
