
#ifndef VRISKA_LIB_THREADS_SCOPEDLOCK_HH_
# define VRISKA_LIB_THREADS_SCOPEDLOCK_HH_

# include <Vriska/Core/OS.hh>
# include <Vriska/Threads/ILockable.hh>

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

#endif // !VRISKA_LIB_THREADS_SCOPEDLOCK_HH_
