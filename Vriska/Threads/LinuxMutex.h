
#ifndef VRISKA_LIB_THREADS_LINUXMUTEX_H_
# define VRISKA_LIB_THREADS_LINUXMUTEX_H_

# include <Vriska/Threads/INativeMutex.h>

# include <pthread.h>

namespace Vriska
{
  class LinuxMutex : public INativeMutex
  {
  public:
    LinuxMutex();
    ~LinuxMutex();

  private:
    LinuxMutex(INativeMutex const & other);
    LinuxMutex const &	operator=(INativeMutex const & other);

  public:
    bool	lock();
    bool	tryLock();
    bool	unlock();

  public:
    void*	getNative();

  private:
    pthread_mutex_t	_mutex;
  };
}

#endif // !VRISKA_LIB_THREADS_LINUXMUTEX_H_
