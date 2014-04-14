
#ifndef VRISKA_LIB_THREADS_ILOCKABLE_H_
# define VRISKA_LIB_THREADS_ILOCKABLE_H_

namespace Vriska
{
  class ILockable
  {
  public:
    virtual ~ILockable() {}

    virtual bool	lock() = 0;
    virtual bool	tryLock() = 0;
    virtual bool	unlock() = 0;
  };
}

#endif // !VRISKA_LIB_THREADS_ILOCKABLE_H_
