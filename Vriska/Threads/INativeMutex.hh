
#ifndef VRISKA_LIB_THREADS_INATIVEMUTEX_HH_
# define VRISKA_LIB_THREADS_INATIVEMUTEX_HH_

# include <Vriska/Core/OS.hh>
# include <Vriska/Threads/ILockable.hh>

namespace Vriska
{
  class INativeMutex : public ILockable
  {
  public:
    virtual ~INativeMutex() {}

  public:
    virtual void*	getNative() = 0;

  public:
    static INativeMutex*	create();
  };
}

#endif // !VRISKA_LIB_THREADS_INATIVEMUTEX_HH_
