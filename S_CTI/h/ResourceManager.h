#pragma once

#include "Siebel\scapi.h"
#include "CommonDefs.h"

#include <string>

using namespace std;

class ResourceManager
{
	HMODULE languageLibraryHandle = NULL;

	class InternalGlobalInstanceHandler
	{
		ResourceManager *pInstance;

	public:
		~InternalGlobalInstanceHandler()
		{
			if (pInstance)
			{
				delete pInstance;
			}
		}

		ResourceManager* GetInstance()
		{
			if (!pInstance)
			{
				pInstance = new ResourceManager();
			}
			return pInstance;
		}
	};

	static InternalGlobalInstanceHandler internalInstanceHandler;

	ResourceManager();

public:
	~ResourceManager();

	static ResourceManager& GetInstance()
	{
		return *internalInstanceHandler.GetInstance();
	}

	void LoadLanguageLibrary(const ISC_STRING& name);
	wstring GetString(int id) const;
};
