
#ifndef VRISKA_LIB_THREADS_WINDOWSCONDVAR_H_
# define VRISKA_LIB_THREADS_WINDOWSCONDVAR_H_

# include <Vriska/Threads/INativeCondVar.h>

# include <Windows.h>

namespace Vriska
{
  class WindowsCondVar : public INativeCondVar
  {
  public:
    WindowsCondVar(INativeMutex* mutex = NULL);
    ~WindowsCondVar();

  private:
    WindowsCondVar(INativeCondVar const & other);
    WindowsCondVar const &	operator=(INativeCondVar const & other);

  public:
    bool			notify(bool lock = true);
    bool			notifyAll(bool lock = true);
    INativeCondVar::Result	wait(Time const & timeout = Time::Zero, bool lock = true);

  private:
    CONDITION_VARIABLE	_condvar;    
  };
}

#endif // !VRISKA_LIB_THREADS_WINDOWSCONDVAR_H_
