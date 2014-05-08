
#ifndef VRISKA_LIB_THREADS_CONDVAR_H_
# define VRISKA_LIB_THREADS_CONDVAR_H_

# include <Vriska/Threads/ILockable.h>
# include <Vriska/Threads/INativeCondVar.h>
# include <Vriska/Threads/Mutex.h>

namespace Vriska
{
  class VRISKA_EXPORT CondVar : public ILockable
  {
  public:
    enum Result
      {
	NoError,
	Failed,
	Timeout
      };

  public:
    CondVar();
    CondVar(Mutex& mutex);
    virtual ~CondVar();

  private:
    CondVar(CondVar const & other);
    CondVar const &	operator=(CondVar const & other);

  public:
    virtual bool	lock();
    virtual bool	tryLock();
    virtual bool	unlock();

    virtual bool    isRecursive() const;

    virtual bool	notify(bool lock = true);
    virtual bool	notifyAll(bool lock = true);
    virtual Result	wait(Time const & timeout = Time::Zero, bool lock = true);

  private:
    INativeCondVar&	_condvar;
  };
}

#endif // !VRISKA_LIB_THREADS_CONDVAR_H_
