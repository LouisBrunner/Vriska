
#ifndef VRISKA_LIB_THREADS_THREAD_HH_
# define VRISKA_LIB_THREADS_THREAD_HH_

# include <Vriska/Threads/INativeThread.hh>
# include <Vriska/Threads/Mutex.hh>
# include <Vriska/Threads/ScopedLock.hh>

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

#endif // !VRISKA_LIB_THREADS_THREAD_HH_
