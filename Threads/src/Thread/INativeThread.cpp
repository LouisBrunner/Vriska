
#include <Vriska/Threads/INativeThread.h>

#ifdef VRISKA_WINDOWS
# include <Vriska/Threads/WindowsThread.h>
#else
# include <Vriska/Threads/LinuxThread.h>
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
