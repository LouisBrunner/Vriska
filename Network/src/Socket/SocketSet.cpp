
#include <Vriska/Network/SocketSet.hh>

// Forward declaration completion
#include <Vriska/Network/INativeSocket.hh>

namespace Vriska
{
  SocketSet::SocketSet()
  {
  }

  SocketSet::~SocketSet()
  {
  }

  SocketSet::SocketSet(SocketSet const & other)
  {
    _list = std::list<INativeSocket const *>(other._list.begin(), other._list.end());
  }

  SocketSet const & SocketSet::operator=(SocketSet const & other)
  {
    if (this != &other)
      _list = std::list<INativeSocket const *>(other._list.begin(), other._list.end());
    return (*this);
  }

  void	SocketSet::pushSocket(INativeSocket const * socket)
  {
    _list.push_back(socket);
  }

  bool	SocketSet::hasSocket(INativeSocket const * socket) const
  {
    for (CIter it = _list.begin(); it != _list.end(); ++it)
      if (*it == socket)
	return (true);
    return (false);
  }

  SocketSet::Iter	SocketSet::removeSocket(SocketSet::Iter& toDelete)
  {
    return (_list.erase(toDelete));
  }

  void			SocketSet::removeSocket(INativeSocket const * toDelete)
  {
    _list.remove(toDelete);
  }

  void	SocketSet::clear()
  {
    _list.clear();
  }

  SocketSet::Iter	SocketSet::begin()
  {
    return (_list.begin());
  }

  SocketSet::Iter	SocketSet::end()
  {
    return (_list.end());
  }
}
