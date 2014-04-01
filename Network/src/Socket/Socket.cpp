
#include <Vriska/Network/Socket.hh>

namespace Vriska
{
  Socket::Socket() : _connected(false), _protocol(TCP), _socket(*INativeSocket::create())
  {
  }

  Socket::Socket(INativeSocket& sysSocket) : _connected(false), _protocol(TCP), _socket(sysSocket)
  {
  }

  Socket::~Socket()
  {
    delete &_socket;
  }

  VRISKA_ACCESSIBLE
  bool		Socket::isConnected() const
  {
    ScopedLock	lock(_mutex);

    return (_connected);
  }

  VRISKA_ACCESSIBLE
  void		Socket::setProtocol(Protocol protocol)
  {
    ScopedLock	lock(_mutex);

    _protocol = protocol;
  }

  unsigned int	Socket::getPort() const
  {
    return (_socket.getPort());
  }

  void	Socket::sysLog(std::string const & info)
  {
    static_cast<void>(info);
  }

  std::string		Socket::protocolToString(Protocol protocol)
  {
    if (protocol == UDP)
      return ("UDP");
    return ("TCP");
  }
}
