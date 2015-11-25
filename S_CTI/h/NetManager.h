#pragma once

#include <winsock2.h>

class NetManager
{
private:
	int WSAStartupRetCode;
	WSADATA wsaData;

	class SingleInstanceHandler
	{
		NetManager *pNetManager;

	public:
		~SingleInstanceHandler()
		{
			if (pNetManager)
			{
				delete pNetManager;
			}
		}

		NetManager* GetInstance()
		{
			if (!pNetManager)
			{
				pNetManager = new NetManager();
			}
			return pNetManager;
		}
	};

	static SingleInstanceHandler sih;

	NetManager();

	~NetManager()
	{
		if (WSAStartupRetCode == 0)
		{
			WSACleanup();
		}
	}

public:
	static NetManager& GetInstance()
	{
		return *sih.GetInstance();
	}

	void Init();

	bool IsInitialized() const
	{
		return WSAStartupRetCode == 0;
	}
};

