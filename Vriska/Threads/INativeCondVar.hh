
#ifndef VRISKA_LIB_THREADS_INATIVECONDVAR_HH_
# define VRISKA_LIB_THREADS_INATIVECONDVAR_HH_

# include <Vriska/Core/OS.hh>
# include <Vriska/Core/Time.hh>
# include <Vriska/Threads/Mutex.hh>

namespace Vriska
{
  class INativeCondVar : public ILockable
  {
  public:
    typedef int Result;
    static const Result	Timeout = -1;
    static const Result	NoError = 0;
    static const Result	Failed = 1;

  public:
    INativeCondVar(INativeMutex* mutex = NULL);
    virtual ~INativeCondVar();

  private:
    INativeCondVar(INativeCondVar const & other);
    INativeCondVar const &	operator=(INativeCondVar const & other);

  public:
    virtual bool	notify(bool lock = true) = 0;
    virtual bool	notifyAll(bool lock = true) = 0;
    virtual Result	wait(Time const & timeout = Time::Zero, bool lock = true) = 0;

  public:
    static INativeCondVar*	create(INativeMutex* mutex = NULL);

  public:
    bool	lock();
    bool	tryLock();
    bool	unlock();

  protected:
    bool		ownLock();
    bool		ownUnlock();

  protected:
    INativeMutex&	_mutex;

  private:
    bool		_own;
  };
}

#endif // !VRISKA_LIB_THREADS_INATIVECONDVAR_HH_
