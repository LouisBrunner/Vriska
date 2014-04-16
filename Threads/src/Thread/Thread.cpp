
#include <Vriska/Threads/Thread.h>

namespace Vriska
{
  VRISKA_ACCESSIBLE
  Thread::Thread() : _launched(false), _thread(*INativeThread::create())
  {
  }

  VRISKA_ACCESSIBLE
  Thread::~Thread()
  {
    terminate();
    delete &_thread;
  }

  VRISKA_ACCESSIBLE
  bool		Thread::operator==(Thread const & other) const
  {
    ScopedLock	lock(_mutex);

    if (_launched)
      return (false);
    return (_thread == other._thread);
  }

  VRISKA_ACCESSIBLE
  bool		Thread::operator!=(Thread const & other) const
  {
    {
      ScopedLock	lock(_mutex);

      if (_launched)
	return (false);
    }
    return (!(*this == other));
  }

  VRISKA_ACCESSIBLE
  bool			Thread::launch(IRunnable& run)
  {
    ScopedLock		lock(_mutex);
    bool		ret;

    if (_launched)
      return (false);
    ret = _thread.launch(run);
    if (ret)
      _launched = true;
    return (ret);
  }

  VRISKA_ACCESSIBLE
  bool		Thread::join()
  {
    ScopedLock	lock(_mutex);
    bool	ret;

    if (!_launched)
      return (false);
    ret = _thread.join();
    if (ret)
      _launched = false;
    return (ret);
  }

  VRISKA_ACCESSIBLE
  bool		Thread::isAlive()
  {
    ScopedLock	lock(_mutex);

    return (_launched);
  }

  VRISKA_ACCESSIBLE
  bool			Thread::terminate()
  {
    {
      ScopedLock	lock(_mutex);

      if (!_launched)
    	return (false);
      _thread.terminate();
    }
    return (join());
  }
}
