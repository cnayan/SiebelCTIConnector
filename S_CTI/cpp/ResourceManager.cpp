#include "ResourceManager.h"
#include "Main.h"
#include "Utils.h"
#include "AutoFuncScopeLogger.h"

#include <string>
#include <sstream>

using namespace std;

ResourceManager::InternalGlobalInstanceHandler ResourceManager::internalInstanceHandler;

ResourceManager::ResourceManager()
{
	INIT_FUNCTION_SCOPE_LOGGER();
}

ResourceManager::~ResourceManager()
{
	INIT_FUNCTION_SCOPE_LOGGER();

	if (languageLibraryHandle != NULL)
	{
		::CloseHandle(languageLibraryHandle);
		languageLibraryHandle = NULL;
	}
}

void ResourceManager::LoadLanguageLibrary(const ISC_STRING& sLanguageCode)
{
	INIT_FUNCTION_SCOPE_LOGGER();

	if (languageLibraryHandle != NULL)
	{
		LOGD(L"A language library is already loaded. Handle = " << languageLibraryHandle);
	}
	else
	{
		wstring libName;
		{
			wstringstream wss;
			wss << CTIDLL_NAME << L"_" << TW(sLanguageCode) << L".dll";
			libName = wss.str();
		}

		LOGD(L"The language library to load is " << libName.c_str());

		// Should load the actuall DLL corresponding to the language code.
		// Ideally you should internationilze the messages in separate
		// language DLLs.
		//languageLibraryHandle = ::LoadLibraryW(libName.c_str());
	}
}

wstring ResourceManager::GetString(int id) const
{
	return Utils::GetStringW(NULL, id);
}
