
#ifndef VRISKA_LIB_THREADS_LINUXCONDVAR_H_
# define VRISKA_LIB_THREADS_LINUXCONDVAR_H_

# include <Vriska/Threads/INativeCondVar.h>

# include <pthread.h>
# include <errno.h>

# include <Vriska/Core/OS.h>

# ifdef VRISKA_MAC
#  include <mach/clock.h>
#  include <mach/mach.h>

#  define get_clocktime(ts) do { \
                clock_serv_t cclock; \
                mach_timespec_t mts; \
                host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock); \
                clock_get_time(cclock, &mts); \
                mach_port_deallocate(mach_task_self(), cclock); \
                (ts)->tv_sec = mts.tv_sec; \
                (ts)->tv_nsec = mts.tv_nsec; \
            } while (0)
# else // VRISKA_MAC
#  define get_clocktime(ts) clock_gettime(CLOCK_REALTIME, ts)
# endif // !VRISKA_MAC

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

#endif // !VRISKA_LIB_THREADS_LINUXCONDVAR_H_
