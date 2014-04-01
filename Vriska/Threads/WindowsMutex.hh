
#ifndef VRISKA_LIB_THREADS_WINDOWSMUTEX_HH_
# define VRISKA_LIB_THREADS_WINDOWSMUTEX_HH_

# include <Vriska/Threads/INativeMutex.hh>

# include <Windows.h>

namespace Vriska
{
  class WindowsMutex : public INativeMutex
  {
  public:
    WindowsMutex();
    ~WindowsMutex();

  private:
    WindowsMutex(INativeMutex const & other);
    WindowsMutex const &	operator=(INativeMutex const & other);

  public:
    bool	lock();
    bool	tryLock();
    bool	unlock();

  public:
    void*	getNative();

  private:
    CRITICAL_SECTION	_mutex;
  };
}

#endif // !VRISKA_LIB_THREADS_WINDOWSMUTEX_HH_
