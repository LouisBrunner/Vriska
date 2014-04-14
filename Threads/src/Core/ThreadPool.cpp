
#include <Vriska/Threads/ThreadPool.h>

#include <iostream>

namespace Vriska
{
  VRISKA_ACCESSIBLE
  ThreadPool::ThreadPool(unsigned int n) : _end(false), _number(n)
  {
    for (unsigned int i = 0; i < n; ++i)
      _pool.push_back(new Thread);
    for (Iter it = _pool.begin(); it != _pool.end(); ++it)
      {
	_cv.lock();
	(*it)->launch(*this);
	_cv.wait(Time::Zero, false);
	_cv.unlock();
      }
  }

  VRISKA_ACCESSIBLE
  ThreadPool::~ThreadPool()
  {
    _mutex.lock();
    _end = true;
    _mutex.unlock();
    _list.lock();
    _list.notifyAll();
    _list.unlock();
    for (Iter it = _pool.begin(); it != _pool.end(); ++it)
      delete *it;
    _pool.clear();
  }

  VRISKA_ACCESSIBLE
  bool			ThreadPool::execute(IRunnable* run)
  {
    ScopedLock		lock(_list);

    _list.push_back(run);
    return (true);
  }

  VRISKA_ACCESSIBLE
  void			ThreadPool::run()
  {
    IRunnable		*run;

    _cv.notify();
    _mutex.lock();
    while (!_end)
      {
	_mutex.unlock();
	_list.lock();
	if (_list.empty())
	  {
	    _list.wait();
	    _list.unlock();
	  }
	else
	  {
	    run = _list.pop_front();
	    _list.unlock();
	    run->run();
	  }
	_mutex.lock();
      }
    _mutex.unlock();
  }
}
