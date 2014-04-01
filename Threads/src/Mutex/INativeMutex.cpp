
#include <Vriska/Threads/INativeMutex.hh>

#ifdef VRISKA_WINDOWS
# include <Vriska/Threads/WindowsMutex.hh>
#else
# include <Vriska/Threads/LinuxMutex.hh>
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
