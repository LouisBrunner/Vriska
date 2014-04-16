
#include <Vriska/Network/LinuxSocket.h>

namespace Vriska
{
  LinuxSocket::LinuxSocket() : _sock(-1)
  {
    memset(&_sa, 0, sizeof(_sa));
  }

  LinuxSocket::~LinuxSocket()
  {
  }

  bool			LinuxSocket::operator==(INativeSocket const & other) const
  {
    ScopedLock		lock(_mutex);
    LinuxSocket const &	ns = *reinterpret_cast<LinuxSocket const *>(&other);

    return (_sa.sin_addr.s_addr == ns._sa.sin_addr.s_addr
	    && _sa.sin_port == ns._sa.sin_port);
  }

  bool	LinuxSocket::operator!=(INativeSocket const & other) const
  {
    return (!(*this == other));
  }

  Error::Code		LinuxSocket::socket(std::string const & protocol)
  {
    struct protoent	*pe = NULL;
    int			val;
    unsigned int	size = sizeof(val);
    int			flags;

    close();

    ScopedLock		lock(_mutex);

    val = 1;
    if ((pe = getprotobyname(protocol.c_str())) == NULL)
      return (Error::UnknownProtocol);
    flags = SOCK_STREAM;
    if (protocol == "UDP")
      flags = SOCK_DGRAM;
    if ((_sock = ::socket(AF_INET, flags, pe ? pe->p_proto : 0)) == -1)
      return (Error::CannotCreateSocket);
    if (setsockopt(_sock, SOL_SOCKET, SO_REUSEADDR, &val, size) == -1)
      return (Error::CannotCreateSocket);
    return (Error::NoError);
  }

  Error::Code		LinuxSocket::connect(std::string const & host, unsigned int port)
  {
    ScopedLock		lock(_mutex);
    struct hostent	*he;

    if (_sock == -1)
      return (Error::NoSocket);
    memset(&_sa, 0, sizeof(_sa));
    _sa.sin_family = AF_INET;
    _sa.sin_port = htons(port);
    if ((he = gethostbyname(host.c_str())) == NULL || he->h_length < 1)
      return (Error::UnknowHost);
    memcpy(&_sa.sin_addr, he->h_addr, he->h_length);
    if (::connect(_sock, reinterpret_cast<struct sockaddr *>(&_sa), sizeof(_sa)) == -1)
      return (Error::CannotConnectSocket);
    return (Error::NoError);
  }

  Error::Code		LinuxSocket::bind(unsigned int port, std::string const & host)
  {
    ScopedLock		lock(_mutex);
    struct hostent	*he;

    if (_sock == -1)
      return (Error::NoSocket);
    memset(&_sa, 0, sizeof(_sa));
    _sa.sin_family = AF_INET;
    _sa.sin_port = htons(port);
    if (host == "")
      _sa.sin_addr.s_addr = htonl(INADDR_ANY);
    else
      {
	if ((he = gethostbyname(host.c_str())) == NULL || he->h_length < 1)
	  return (Error::UnknowHost);
	memcpy(&_sa.sin_addr, he->h_addr, he->h_length);
      }
    if (::bind(_sock, reinterpret_cast<struct sockaddr *>(&_sa), sizeof(_sa)) == -1)
      return (Error::CannotBindSocket);
    return (Error::NoError);
  }

  Error::Code	LinuxSocket::listen(unsigned int clients)
  {
    ScopedLock		lock(_mutex);

    if (_sock == -1)
      return (Error::NoSocket);
    if (::listen(_sock, clients) == -1)
      return (Error::CannotBindSocket);
    return (Error::NoError);
  }

  Error::Code	LinuxSocket::close()
  {
    ScopedLock		lock(_mutex);

    if (_sock != -1)
      {
	::close(_sock);
	_sock = -1;
	memset(&_sa, 0, sizeof(_sa));
      }
    return (Error::NoError);
  }

  void			LinuxSocket::setSockets(SocketSet& set, fd_set& nset, int& max)
  {
    int			sock;

    FD_ZERO(&nset);
    for (SocketSet::Iter it = set.begin(); it != set.end(); ++it)
      {
	sock = reinterpret_cast<LinuxSocket const *>(*it)->_sock;
	FD_SET(sock, &nset);
	max = std::max<int>(sock, max);
      }
  }

  void			LinuxSocket::removeSockets(SocketSet& set, fd_set& nset)
  {
    int			sock;

    for (SocketSet::Iter it = set.begin(); it != set.end();)
      {
	sock = reinterpret_cast<LinuxSocket const *>(*it)->_sock;
	if (!FD_ISSET(sock, &nset))
	  it = set.removeSocket(it);
	else
	  ++it;
      }
  }
    
  Error::Code		LinuxSocket::select(SocketSet& read, SocketSet& write, Time* timeout, bool* hasStdin)
  {
    ScopedLock		lock(_mutex);
    int			max = -1;
    fd_set		fread;
    fd_set		fwrite;
    struct timeval	ntime;
    int			ret;

    setSockets(read, fread, max);
    setSockets(write, fwrite, max);
    if (timeout != NULL)
      timeout->toTimeval(ntime);
    if (hasStdin != NULL)
      {
	*hasStdin = false;
	FD_SET(0, &fread);
      }
    ret = ::select(max + 1, &fread, &fwrite, NULL, timeout != NULL ? &ntime : NULL);
    if (ret == -1)
      {
	read.clear();
	write.clear();
	if (timeout != NULL)
	  timeout->setAll(0, 0);
      }
    else
      {
	if (timeout != NULL && ret != 0)
	  *timeout -= Time::fromSTimeval(ntime);
	removeSockets(read, fread);
	removeSockets(write, fwrite);
      }
    if (hasStdin != NULL && FD_ISSET(0, &fread))
      *hasStdin = true;
    return (Error::NoError);
  }

  Error::Code		LinuxSocket::accept(INativeSocket** client)
  {
    ScopedLock		lock(_mutex);
    LinuxSocket		*ns;
    int			sock;
    struct sockaddr_in	sa;
    unsigned int	len = sizeof(_sa);

    *client = NULL;
    if (_sock == -1)
      return (Error::NoSocket);
    sock = ::accept(_sock, reinterpret_cast<struct sockaddr *>(&sa), &len);
    if (sock != -1)
      {
	ns = new LinuxSocket();
	ns->_sock = sock;
	memcpy(&ns->_sa, &sa, len);
	*client = ns;
	return (Error::NoError);
      }
    return (Error::CannotConnectSocket);
  }

  int			LinuxSocket::recv(void *buffer, size_t size)
  {
    ScopedLock		lock(_mutex);
    int			ret = 0;
    char		*cbuffer = static_cast<char *>(buffer);

    if (_sock == -1)
      return (Error::NoSocket);
    ret = ::recv(_sock, cbuffer, size, 0);
    if (ret <= -1)
      return (INativeSocket::Error);
    else if (ret == 0)
      return (INativeSocket::Disconnected);
    return (ret);
  }

  int			LinuxSocket::recvfrom(void *buffer, size_t size, INativeSocket** client)
  {
    ScopedLock		lock(_mutex);
    LinuxSocket*	ns;
    int			ret = 0;
    char		*cbuffer = static_cast<char *>(buffer);
    struct sockaddr_in	sa;
    socklen_t		len = sizeof(sa);

    *client = NULL;
    if (_sock == -1)
      return (Error::NoSocket);
    memset(&sa, 0, sizeof(sa));
    ret = ::recvfrom(_sock, cbuffer, size, 0,
		     reinterpret_cast<struct sockaddr *>(&sa), &len);
    ns = new LinuxSocket();
    ns->_sock = _sock;
    memcpy(&ns->_sa, &sa, len);
    *client = ns;
    if (ret <= -1)
      return (INativeSocket::Error);
    else if (ret == 0)
      return (INativeSocket::Disconnected);
    return (ret);
  }

  int			LinuxSocket::send(void const *buffer, size_t size)
  {
    ScopedLock		lock(_mutex);
    int			ret = 0;
    char const		*cbuffer = static_cast<char const *>(buffer);

    if (_sock == -1)
      return (Error::NoSocket);
    ret = ::sendto(_sock, cbuffer, size, 0,
		   reinterpret_cast<struct sockaddr *>(&_sa), sizeof(_sa));
    if (ret <= -1)
      return (INativeSocket::Error);
    return (ret);
  }

  std::string	LinuxSocket::getIPAddress() const
  {
    ScopedLock	lock(_mutex);
    std::string	ret;

    if (_sock == -1)
      return ("");
    ret = inet_ntoa(_sa.sin_addr);
    return (ret);
  }

  std::string		LinuxSocket::getIPConnectedOn() const
  {
    ScopedLock		lock(_mutex);
    std::string		ret;
    struct sockaddr_in	sa;
    unsigned int 	len;

    if (_sock == -1)
      return ("");
    len = sizeof(sa);
    memset(&sa, 0, sizeof(sa));
    getsockname(_sock, reinterpret_cast<struct sockaddr *>(&sa), &len);
    ret = inet_ntoa(sa.sin_addr);
    return (ret);
  }

  unsigned int	LinuxSocket::getPort() const
  {
    ScopedLock		lock(_mutex);
    struct sockaddr_in	sa;
    unsigned int	len;

    if (_sock == -1)
      return (0);
    len = sizeof(sa);
    memset(&sa, 0, sizeof(sa));
    getsockname(_sock, reinterpret_cast<struct sockaddr *>(&sa), &len);
    return (ntohs(sa.sin_port));
  }
}
