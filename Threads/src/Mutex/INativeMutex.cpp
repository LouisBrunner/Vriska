
#include <Vriska/Threads/INativeMutex.h>
#include <Vriska/Core/Utils.hpp>

#ifdef VRISKA_WINDOWS
# include <Vriska/Threads/WindowsMutex.h>
#else
# include <Vriska/Threads/LinuxMutex.h>
#endif // !VRISKA_WINDOWS

namespace Vriska
{
  INativeMutex*	INativeMutex::create(bool recursive)
  {
#ifdef VRISKA_WINDOWS
    Utils::ignore(recursive);
    return (new WindowsMutex());
#else
    return (new LinuxMutex(recursive));
#endif // !VRISKA_WINDOWS
  }
}
