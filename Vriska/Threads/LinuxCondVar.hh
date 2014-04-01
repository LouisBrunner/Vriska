
#ifndef VRISKA_LIB_THREADS_LINUXCONDVAR_HH_
# define VRISKA_LIB_THREADS_LINUXCONDVAR_HH_

# include <Vriska/Threads/INativeCondVar.hh>

# include <pthread.h>
# include <errno.h>

namespace Vriska
{
  class LinuxCondVar : public INativeCondVar
  {
  public:
    LinuxCondVar(INativeMutex* mutex = NULL);
    ~LinuxCondVar();

  private:
    LinuxCondVar(INativeCondVar const & other);
    LinuxCondVar const &	operator=(INativeCondVar const & other);

  public:
    bool			notify(bool lock = true);
    bool			notifyAll(bool lock = true);
    INativeCondVar::Result	wait(Time const & timeout = Time::Zero, bool lock = true);

  private:
    pthread_cond_t	_condvar;
  };
}

#endif // !VRISKA_LIB_THREADS_LINUXCONDVAR_HH_
