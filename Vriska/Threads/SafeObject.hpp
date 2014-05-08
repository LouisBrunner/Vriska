
#ifndef VRISKA_LIB_THREADS_SAFEOBJECT_HPP_
# define VRISKA_LIB_THREADS_SAFEOBJECT_HPP_

# include <Vriska/Core/Time.h>
# include <Vriska/Threads/CondVar.h>
# include <Vriska/Threads/ILockable.h>
# include <Vriska/Threads/Mutex.h>

# include <vector>
# include <deque>
# include <list>
# include <stack>
# include <queue>
# include <set>
# include <map>

namespace Vriska
{
  template <class Type>
  class SafeObject : public ILockable
  {
  public:
    SafeObject() : _cv(_mutex) {}
    virtual ~SafeObject() {}

  public:
    Type& operator*()
	{
        if (_mutex.isRecursive())
        {
            ScopedLock  lock(_mutex);

	        return (_object);
        }
        
	    return (_object);
	}

    Type* operator->()
	{
        if (_mutex.isRecursive())
        {
            ScopedLock  lock(_mutex);

	        return (&_object);
        }
        
	    return (&_object);
	}

  public:
    virtual bool	lock()
    {
      return (_mutex.lock());
    }

    virtual bool	unlock()
    {
      return (_mutex.unlock());
    }

    virtual bool	tryLock()
    {
      return (_mutex.tryLock());
    }

  public:
    virtual bool	notify()
    {
      return (_cv.notify());
    }

    virtual bool	notifyAll()
    {
      return (_cv.notifyAll());
    }

    virtual CondVar::Result	wait(Time const & timeout = Time::Zero)
    {
      return (_cv.wait(timeout));
    }

  private:
    SafeObject(SafeObject const & other);
    SafeObject&		operator=(SafeObject const & other);

  private:
    Type	    _object;
    Mutex		_mutex;
    CondVar		_cv;
  };

  template <class Item>
  class SafeVector : public SafeObject< std::vector<Item> >
  {
  };

  template <class Item>
  class SafeDeque : public SafeObject< std::deque<Item> >
  {
  };

  template <class Item>
  class SafeList : public SafeObject< std::list<Item> >
  {
  };

  template <class Item>
  class SafeStack : public SafeObject< std::stack<Item> >
  {
  };

  template <class Item>
  class SafeQueue : public SafeObject< std::queue<Item> >
  {
  };

  template <class Item>
  class SafeSet : public SafeObject< std::set<Item> >
  {
  };

  template <class Key, class Value>
  class SafeMap : public SafeObject< std::map<Key, Value> >
  {
  };
}

#endif // !VRISKA_LIB_THREADS_SAFEOBJECT_HPP_
