
#include <Vriska/Network/INativeSocket.hh>

#ifdef VRISKA_WINDOWS
# include <Vriska/Network/WindowsSocket.hh>
#else
# include <Vriska/Network/LinuxSocket.hh>
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
