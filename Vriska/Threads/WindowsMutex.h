
#ifndef VRISKA_LIB_THREADS_WINDOWSMUTEX_H_
# define VRISKA_LIB_THREADS_WINDOWSMUTEX_H_

# include <Vriska/Threads/INativeMutex.h>

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
    bool  isRecursive() const;

  public:
    void*	getNative();

  private:
    CRITICAL_SECTION	_mutex;
  };
}

#endif // !VRISKA_LIB_THREADS_WINDOWSMUTEX_H_
