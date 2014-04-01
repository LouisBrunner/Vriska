
#ifndef VRISKA_LIB_THREADS_LINUXTHREAD_HH_
# define VRISKA_LIB_THREADS_LINUXTHREAD_HH_

# include <Vriska/Threads/INativeThread.hh>

# include <pthread.h>

namespace Vriska
{
  class LinuxThread : public INativeThread
  {
  public:
    LinuxThread();
    ~LinuxThread();

    bool	operator==(INativeThread const & other) const;
    bool	operator!=(INativeThread const & other) const;

  private:
    LinuxThread(INativeThread const & other);
    LinuxThread const &	operator=(INativeThread const & other);

  public:
    bool	launch(IRunnable& run);
    bool	join();
    bool	terminate();

  private:
    static void	*start_thread(void *arg);

  private:
    pthread_t	_thread;
  };
}

#endif // !VRISKA_LIB_THREADS_LINUXTHREAD_HH_
