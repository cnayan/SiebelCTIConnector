#include "Main.h"

static HINSTANCE g_Instance;

int APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	g_Instance = (HINSTANCE)hModule;
	return TRUE;
}
