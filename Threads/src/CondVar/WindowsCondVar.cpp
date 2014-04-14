
#include <Vriska/Threads/WindowsCondVar.h>

#include <Windows.h>

namespace Vriska
{
  WindowsCondVar::WindowsCondVar(INativeMutex* mutex) : INativeCondVar(mutex)
  {
    InitializeConditionVariable(&_condvar);
  }

  WindowsCondVar::~WindowsCondVar()
  {
  }

  bool			WindowsCondVar::notify(bool lock)
  {
    if (lock)
      ownLock();
    WakeConditionVariable(&_condvar);
    if (lock)
      ownUnlock();
    return (true);
  }

  bool			WindowsCondVar::notifyAll(bool lock)
  {
    if (lock)
      ownLock();
    WakeAllConditionVariable(&_condvar);
    if (lock)
      ownUnlock();
    return (true);
  }

  INativeCondVar::Result	WindowsCondVar::wait(Time const & timeout, bool lock)
  {
    CRITICAL_SECTION*		mutex = reinterpret_cast<CRITICAL_SECTION *>(_mutex.getNative());
    int				ret;

    if (lock)
      ownLock();
    ret = SleepConditionVariableCS(&_condvar, mutex, timeout != Time::Zero ? timeout.toMilli() : INFINITE);
    if (lock)
      ownUnlock();
    if (ret != 0)
      return (INativeCondVar::NoError);
    else if (GetLastError() == ERROR_TIMEOUT)
      return (INativeCondVar::Timeout);      
    return (INativeCondVar::Failed);
  }
}
