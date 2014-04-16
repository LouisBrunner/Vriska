
#include <Vriska/Threads/WindowsMutex.h>

namespace Vriska
{
  WindowsMutex::WindowsMutex()
  {
    InitializeCriticalSection(&_mutex);
  }

  WindowsMutex::~WindowsMutex()
  {
    DeleteCriticalSection(&_mutex);
  }

  bool	WindowsMutex::lock()
  {
    EnterCriticalSection(&_mutex);
    return (true);
  }

  bool	WindowsMutex::tryLock()
  {
    return (TryEnterCriticalSection(&_mutex) == TRUE);
  }

  bool	WindowsMutex::unlock()
  {
    LeaveCriticalSection(&_mutex);
    return (true);
  }

  void*	WindowsMutex::getNative()
  {
    return (&_mutex);
  }
}
