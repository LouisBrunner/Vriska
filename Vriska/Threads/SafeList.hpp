
#ifndef SAFELIST_HPP_
# define SAFELIST_HPP_

# include <Vriska/Core/Time.hh>
# include <Vriska/Threads/CondVar.hh>
# include <Vriska/Threads/ILockable.hh>
# include <Vriska/Threads/Mutex.hh>
# include <list>

namespace Vriska
{
  template <class Item>
  class SafeList : public ILockable
  {
  public:
    typedef typename std::list<Item>::iterator		Iter;
    typedef typename std::list<Item>::const_iterator	CIter;
    
  public:
    SafeList() : _cv(_mutex) {}
    virtual ~SafeList() {}

  public:
    virtual bool	empty() const
    {
      return (_list.empty());
    }

    virtual unsigned int	size() const
    {
      return (_list.size());
    }

  public:
    virtual Item&	front()
    {
      return (_list.front());
    }

    virtual Item	pop_front()
    {
      Item		it = _list.front();

      _list.pop_front();
      return (it);
    }

    virtual void	push_front(Item it)
    {
      _list.push_front(it);
      _cv.notify();
    }

    virtual Item&	back()
    {
      return (_list.back());
    }

    virtual Item	pop_back()
    {
      Item		it = _list.back();

      _list.pop_back();
      return (it);
    }

    virtual void	push_back(Item it)
    {
      _list.push_back(it);
      _cv.notify();
    }

  public:
    virtual Iter	begin()
    {
      return (_list.begin());
    }

    virtual CIter	begin() const
    {
      return (_list.begin());
    }

    virtual Iter	end()
    {
      return (_list.end());
    }

    virtual CIter	end() const
    {
      return (_list.end());
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
    SafeList(SafeList const & other);
    SafeList&		operator=(SafeList const & other);

  private:
    std::list<Item>	_list;
    Mutex		_mutex;
    CondVar		_cv;
  };
}

#endif // !SAFELIST_HPP_
