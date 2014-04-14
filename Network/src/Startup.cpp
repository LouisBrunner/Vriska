
#include <Vriska/Core/Time.h>
#include <Vriska/Core/Exceptions.hpp>
#include <winsock2.h>

namespace Vriska
{
  const Time	Time::Zero = Time();
}

BOOLEAN WINAPI DllMain(IN HINSTANCE hDllHandle, IN DWORD nReason, IN LPVOID Reserved)
{
  switch (nReason)
    {
    case DLL_PROCESS_ATTACH:
      WSADATA	wsa;
      
      if (WSAStartup(MAKEWORD(2, 2), &wsa) < 0)
	{
	  throw Vriska::UnrecoverableIncidentException("Cannot init WindowsSocketAPI");
	  return (FALSE);
	}
      break;
    case DLL_PROCESS_DETACH:
      WSACleanup();
      break;
    }
  return (TRUE);
}
