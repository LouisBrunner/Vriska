
#include <Vriska/Core/OS.h>

namespace Vriska
{
  OS::Name	OS::getOS()
  {
#if defined(VRISKA_WINDOWS)
    return (Windows);
#elif defined(VRISKA_MAC)
    return (MacOS);
#else
    return (Linux);
#endif // !VRISKA_WINDOWS
  }
}
