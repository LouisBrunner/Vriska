
#include <Vriska/Threads/LinuxCondVar.hh>

namespace Vriska
{
  LinuxCondVar::LinuxCondVar(INativeMutex* mutex) : INativeCondVar(mutex)
  {
    pthread_cond_init(&_condvar, NULL);
  }

  LinuxCondVar::~LinuxCondVar()
  {
    pthread_cond_destroy(&_condvar);
  }

  bool			LinuxCondVar::notify(bool lock)
  {
    bool		res;

    if (lock)
      ownLock();
    res = (pthread_cond_signal(&_condvar) == 0);
    if (lock)
      ownUnlock();
    return (res);
  }

  bool			LinuxCondVar::notifyAll(bool lock)
  {
    bool		res;

    if (lock)
      ownLock();
    res = (pthread_cond_broadcast(&_condvar) == 0);
    if (lock)
      ownUnlock();
    return (res);
  }

  INativeCondVar::Result	LinuxCondVar::wait(Time const & timeout, bool lock)
  {
    pthread_mutex_t*		mutex = reinterpret_cast<pthread_mutex_t *>(_mutex.getNative());
    int				ret;
    timespec			ts;

    if (lock)
      ownLock();
    if (timeout != Time::Zero)
      {
  	clock_gettime(CLOCK_REALTIME, &ts);
  	ts.tv_nsec += timeout.getNano();
  	ts.tv_sec += timeout.getSeconds();
  	ret = pthread_cond_timedwait(&_condvar, mutex, &ts);
  	if (ret == ETIMEDOUT)
  	  return (INativeCondVar::Timeout);
      }
    else
      ret = pthread_cond_wait(&_condvar, mutex);
    if (lock)
      ownUnlock();
    if (ret == 0)
      return (INativeCondVar::NoError);
    return (INativeCondVar::Failed);
  }
}
