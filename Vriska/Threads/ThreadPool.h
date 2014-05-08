
#ifndef VRISKA_LIB_THREADS_THREADPOOL_H_
# define VRISKA_LIB_THREADS_THREADPOOL_H_

# include <Vriska/Threads/Thread.h>
# include <Vriska/Threads/Mutex.h>
# include <Vriska/Threads/CondVar.h>
# include <Vriska/Threads/SafeObject.hpp>
# include <Vriska/Threads/ScopedLock.h>
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

#endif // !VRISKA_LIB_THREADS_THREADPOOL_H_
