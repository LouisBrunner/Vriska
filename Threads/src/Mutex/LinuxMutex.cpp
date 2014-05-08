
#include <Vriska/Threads/LinuxMutex.h>

namespace Vriska
{
  LinuxMutex::LinuxMutex(bool recursive) : _recursive(recursive)
  {
	pthread_mutexattr_init(&_attr);
	if (recursive)
		pthread_mutexattr_settype(&_attr, PTHREAD_MUTEX_RECURSIVE);
	
    pthread_mutex_init(&_mutex, &_attr);
  }

  LinuxMutex::~LinuxMutex()
  {
    pthread_mutex_destroy(&_mutex);
	pthread_mutexattr_destroy(&_attr);
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

  bool	LinuxMutex::isRecursive() const
  {
    return (_recursive);
  }

  void*	LinuxMutex::getNative()
  {
    return (&_mutex);
  }
}
