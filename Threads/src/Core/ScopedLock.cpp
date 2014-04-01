
#include <Vriska/Threads/ScopedLock.hh>

namespace Vriska
{
  VRISKA_ACCESSIBLE
  ScopedLock::ScopedLock(ILockable& lock) : _lock(lock)
  {
    _lock.lock();
  }

  VRISKA_ACCESSIBLE
  ScopedLock::ScopedLock(ILockable const & lock) : _lock(const_cast<ILockable&>(lock))
  {
    _lock.lock();
  }

  VRISKA_ACCESSIBLE
  ScopedLock::~ScopedLock()
  {
    _lock.unlock();
  }
}

