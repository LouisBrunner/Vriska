
#ifndef VRISKA_LIB_NETWORK_SOCKETSET_HH_
# define VRISKA_LIB_NETWORK_SOCKETSET_HH_

//# include <Vriska/Network/INativeSocket.hh>

# include <list>

namespace Vriska
{
  // Forward declaration of INativeSocket
  class INativeSocket;

  class SocketSet
  {
  public:
    typedef std::list<INativeSocket const *>::iterator		Iter;
    typedef std::list<INativeSocket const *>::const_iterator	CIter;

  public:
    SocketSet();
    ~SocketSet();

    SocketSet(SocketSet const & other);
    SocketSet const & operator=(SocketSet const & other);

  public:
    void	pushSocket(INativeSocket const * socket);
    bool	hasSocket(INativeSocket const * socket) const;
    Iter	removeSocket(Iter& toDelete);
    void	removeSocket(INativeSocket const * toDelete);
    void	clear();

    Iter	begin();
    Iter	end();

  private:
    std::list<INativeSocket const *>	_list;
  };
}

#endif // !VRISKA_LIB_NETWORK_SOCKETSET_HH_
