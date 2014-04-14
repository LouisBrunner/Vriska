
#ifndef VRISKA_LIB_THREADS_SCOPEDLOCK_H_
# define VRISKA_LIB_THREADS_SCOPEDLOCK_H_

# include <Vriska/Core/OS.h>
# include <Vriska/Threads/ILockable.h>

namespace Vriska
{
  class VRISKA_EXPORT ScopedLock
  {
  public:
    ScopedLock(ILockable& lock);
    ScopedLock(ILockable const & lock);
    virtual ~ScopedLock();

  private:
    ScopedLock(ScopedLock const & other);
    ScopedLock&	operator=(ScopedLock const & other);

  private:
    ILockable&	_lock;
  };
}

#endif // !VRISKA_LIB_THREADS_SCOPEDLOCK_H_
