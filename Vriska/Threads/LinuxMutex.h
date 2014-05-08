
#ifndef VRISKA_LIB_THREADS_LINUXMUTEX_H_
# define VRISKA_LIB_THREADS_LINUXMUTEX_H_

# include <Vriska/Threads/INativeMutex.h>

# include <pthread.h>

namespace Vriska
{
  class LinuxMutex : public INativeMutex
  {
  public:
    LinuxMutex(bool recursive);
    ~LinuxMutex();

  private:
    LinuxMutex(INativeMutex const & other);
    LinuxMutex const &	operator=(INativeMutex const & other);

  public:
    bool	lock();
    bool	tryLock();
    bool	unlock();

  public:
    bool  isRecursive() const;

  public:
    void*	getNative();

  private:
    bool            _recursive;
    pthread_mutex_t	_mutex;
    pthread_mutexattr_t _attr;
  };
}

#endif // !VRISKA_LIB_THREADS_LINUXMUTEX_H_
