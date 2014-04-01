
#include <Vriska/Core/OS.hh>

namespace Vriska
{
  OS::Name	OS::getOS()
  {
#ifdef VRISKA_WINDOWS
    return (Windows);
#else
    return (Linux);
#endif // !VRISKA_WINDOWS
  }
}
