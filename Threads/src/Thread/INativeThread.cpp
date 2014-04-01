
#include <Vriska/Threads/INativeThread.hh>

#ifdef VRISKA_WINDOWS
# include <Vriska/Threads/WindowsThread.hh>
#else
# include <Vriska/Threads/LinuxThread.hh>
#endif // !VRISKA_WINDOWS

namespace Vriska
{
  INativeThread*	INativeThread::create()
  {
#ifdef VRISKA_WINDOWS
    return (new WindowsThread());
#else
    return (new LinuxThread());
#endif // !VRISKA_WINDOWS
  }
}
