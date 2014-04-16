
#include <Vriska/Threads/LinuxMutex.h>

namespace Vriska
{
  LinuxMutex::LinuxMutex()
  {
    pthread_mutex_init(&_mutex, NULL);
  }

  LinuxMutex::~LinuxMutex()
  {
    pthread_mutex_destroy(&_mutex);
  }

  bool	LinuxMutex::lock()
  {
    return (pthread_mutex_lock(&_mutex) == 0);
  }

  bool	LinuxMutex::tryLock()
  {
    return (pthread_mutex_trylock(&_mutex) == 0);
  }

  bool	LinuxMutex::unlock()
  {
    return (pthread_mutex_unlock(&_mutex) == 0);
  }

  void*	LinuxMutex::getNative()
  {
    return (&_mutex);
  }
}
