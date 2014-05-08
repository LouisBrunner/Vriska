
#include <Vriska/Threads/Mutex.h>

namespace Vriska
{
  VRISKA_ACCESSIBLE
  Mutex::Mutex(bool recursive) : _mutex(*INativeMutex::create(recursive))
  {
  }

  VRISKA_ACCESSIBLE
  Mutex::~Mutex()
  {
    delete &_mutex;
  }

  VRISKA_ACCESSIBLE
  bool	Mutex::lock()
  {
    return (_mutex.lock());
  }

  VRISKA_ACCESSIBLE
  bool	Mutex::tryLock()
  {
    return (_mutex.tryLock());
  }

  VRISKA_ACCESSIBLE
  bool	Mutex::unlock()
  {
    return (_mutex.unlock());
  }

  VRISKA_ACCESSIBLE
  bool	Mutex::isRecursive() const
  {
    return (_mutex.isRecursive());
  }

  INativeMutex&	Mutex::getNative()
  {
    return (_mutex);
  }
}
