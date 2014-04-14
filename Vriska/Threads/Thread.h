
#ifndef VRISKA_LIB_THREADS_THREAD_H_
# define VRISKA_LIB_THREADS_THREAD_H_

# include <Vriska/Threads/INativeThread.h>
# include <Vriska/Threads/Mutex.h>
# include <Vriska/Threads/ScopedLock.h>

namespace Vriska
{
  class VRISKA_EXPORT Thread
  {
  public:
    Thread();
    virtual ~Thread();

    virtual bool	operator==(Thread const & other) const;
    virtual bool	operator!=(Thread const & other) const;
    
  private:
    Thread(Thread const & other);
    Thread const &	operator=(Thread const & other);

  public:
    virtual bool	launch(IRunnable& run);
    virtual bool	join();
    virtual bool	isAlive();
    virtual bool	terminate();

  private:
    bool		_launched;
    INativeThread&	_thread;
    Mutex		_mutex;
  };
}

#endif // !VRISKA_LIB_THREADS_THREAD_H_
