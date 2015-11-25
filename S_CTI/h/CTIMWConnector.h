#pragma once

#include "SocketClient.h"

#include <string>

using namespace std;

class CTIMWConnector
{
private:
	bool connected;
	SocketClient *pSocketClient;

	class SingleInstanceHandler
	{
		CTIMWConnector *pCTIMWConnector;

	public:
		~SingleInstanceHandler()
		{
			if (pCTIMWConnector)
			{
				delete pCTIMWConnector;
			}
		}

		CTIMWConnector* GetInstance()
		{
			if (!pCTIMWConnector)
			{
				pCTIMWConnector = new CTIMWConnector();
			}
			return pCTIMWConnector;
		}
	};

	CTIMWConnector()
	{
	}

public:
	static SingleInstanceHandler sih;

	static CTIMWConnector& GetInstance()
	{
		return *sih.GetInstance();
	}

	void Connect(const wstring & serverName, int port);
	void Disconnect();
	void SendBytes(const void *data, int length);

	bool IsConnected() const;

	~CTIMWConnector()
	{
		Disconnect();
	}
};
