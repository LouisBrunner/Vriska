
#ifndef VRISKA_LIB_THREADS_THREADPOOL_HH_
# define VRISKA_LIB_THREADS_THREADPOOL_HH_

# include <Vriska/Threads/Thread.hh>
# include <Vriska/Threads/Mutex.hh>
# include <Vriska/Threads/CondVar.hh>
# include <Vriska/Threads/SafeList.hpp>
# include <Vriska/Threads/ScopedLock.hh>
# include <vector>

namespace Vriska
{
  class VRISKA_EXPORT ThreadPool : public IRunnable
  {
  private:
    typedef std::vector<Thread *>::iterator Iter;

  public:
    ThreadPool(unsigned int);
    virtual ~ThreadPool();

    virtual bool	execute(IRunnable *run);
    virtual void	run();

  private:
    bool			_end;
    unsigned int		_number;
    CondVar			_cv;
    Mutex			_mutex;
    SafeList<IRunnable*>	_list;
    std::vector<Thread*>	_pool;
  };
}

#endif // !VRISKA_LIB_THREADS_THREADPOOL_HH_
