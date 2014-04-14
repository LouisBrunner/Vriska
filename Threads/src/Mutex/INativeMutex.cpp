
#include <Vriska/Threads/INativeMutex.h>

#ifdef VRISKA_WINDOWS
# include <Vriska/Threads/WindowsMutex.h>
#else
# include <Vriska/Threads/LinuxMutex.h>
#endif // !VRISKA_WINDOWS

namespace Vriska
{
  INativeMutex*	INativeMutex::create()
  {
#ifdef VRISKA_WINDOWS
    return (new WindowsMutex());
#else
    return (new LinuxMutex());
#endif // !VRISKA_WINDOWS
  }
}
