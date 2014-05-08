
#ifndef VRISKA_LIB_THREADS_INATIVEMUTEX_H_
# define VRISKA_LIB_THREADS_INATIVEMUTEX_H_

# include <Vriska/Core/OS.h>
# include <Vriska/Threads/ILockable.h>

namespace Vriska
{
  class INativeMutex : public ILockable
  {
  public:
    virtual ~INativeMutex() {}

  public:
    virtual bool    isRecursive() const = 0;

  public:
    virtual void*	getNative() = 0;

  public:
    static INativeMutex*	create(bool recursive);
  };
}

#endif // !VRISKA_LIB_THREADS_INATIVEMUTEX_H_
