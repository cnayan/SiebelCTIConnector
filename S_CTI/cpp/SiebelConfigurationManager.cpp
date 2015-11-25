#include "SiebelConfigurationManager.h"
#include "Utils.h"
#include "AutoFuncScopeLogger.h"

SiebelConfigurationManager::SiebelConfigurationManager()
{
}

void SiebelConfigurationManager::CopyTo(ISC_StrParamList* const paramList)
{
	INIT_FUNCTION_SCOPE_LOGGER();

	if (!initialized)
	{
		Initialize();
		initialized = true;
	}

	Utils::CopyToParamList(inList, paramList);
}
