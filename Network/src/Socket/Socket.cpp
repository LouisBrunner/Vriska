
#include <Vriska/Network/Socket.h>

namespace Vriska
{
  Socket::Socket() : Logger(false, false), _connected(false), _protocol(TCP), _socket(*INativeSocket::create())
  {
  }

  Socket::Socket(INativeSocket& sysSocket) : Logger(false, false), _connected(false), _protocol(TCP), _socket(sysSocket)
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

  std::string		Socket::protocolToString(Protocol protocol)
  {
    if (protocol == UDP)
      return ("UDP");
    return ("TCP");
  }
}
