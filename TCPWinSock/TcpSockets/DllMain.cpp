#include "Header.h"

WinSockInitializer* initializer = nullptr;

BOOL WINAPI DllMain(
    HINSTANCE hinstDLL,  // handle to DLL module
    DWORD fdwReason,     // reason for calling function
    LPVOID lpReserved)  // reserved
{
    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:
        initializer = WinSockInitializer::GetInstance();
        break;   

    case DLL_PROCESS_DETACH:
        delete initializer;
        break;
    }
    return TRUE;  // Successful DLL_PROCESS_ATTACH.
}