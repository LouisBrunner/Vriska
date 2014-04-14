
#include <Vriska/Threads/CondVar.h>

namespace Vriska
{
  VRISKA_ACCESSIBLE
  CondVar::CondVar() : _condvar(*INativeCondVar::create())
  {
  }

  VRISKA_ACCESSIBLE
  CondVar::CondVar(Mutex& mutex) : _condvar(*INativeCondVar::create(&mutex.getNative()))
  {
  }

  VRISKA_ACCESSIBLE
  CondVar::~CondVar()
  {
    delete &_condvar;
  }

  VRISKA_ACCESSIBLE
  bool	CondVar::lock()
  {
    return (_condvar.lock());
  }

  VRISKA_ACCESSIBLE
  bool	CondVar::tryLock()
  {
    return (_condvar.tryLock());
  }

  VRISKA_ACCESSIBLE
  bool	CondVar::unlock()
  {
    return (_condvar.unlock());
  }

  VRISKA_ACCESSIBLE
  bool	CondVar::notify(bool lock)
  {
    return (_condvar.notify(lock));
  }

  VRISKA_ACCESSIBLE
  bool	CondVar::notifyAll(bool lock)
  {
    return (_condvar.notifyAll(lock));
  }

  VRISKA_ACCESSIBLE
  CondVar::Result		CondVar::wait(Time const & timeout, bool lock)
  {
    INativeCondVar::Result	res;

    res = _condvar.wait(timeout, lock);
    if (res == INativeCondVar::Failed)
      return (Failed);
    else if (res == INativeCondVar::Timeout)
      return (Timeout);
    return (NoError);
  }
}
