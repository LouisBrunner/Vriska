
#include <Vriska/Network/INativeSocket.h>

#ifdef VRISKA_WINDOWS
# include <Vriska/Network/WindowsSocket.h>
#else
# include <Vriska/Network/LinuxSocket.h>
#endif // !VRISKA_WINDOWS

namespace Vriska
{
  VRISKA_ACCESSIBLE
  INativeSocket*	INativeSocket::create()
  {
#ifdef VRISKA_WINDOWS
    return (new WindowsSocket());
#else
    return (new LinuxSocket());
#endif // !VRISKA_WINDOWS
  }
}
