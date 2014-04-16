
#include <Vriska/Network/WindowsSocket.h>

namespace Vriska
{
  WindowsSocket::WindowsSocket() : _sock(INVALID_SOCKET)
  {
    memset(&_sa, 0, sizeof(_sa));
  }

  WindowsSocket::~WindowsSocket()
  {
  }

  bool				WindowsSocket::operator==(INativeSocket const & other) const
  {
    ScopedLock			lock(_mutex);
    WindowsSocket const &	ns = *reinterpret_cast<WindowsSocket const *>(&other);

    return (_sa.sin_addr.s_addr == ns._sa.sin_addr.s_addr
	    && _sa.sin_port == ns._sa.sin_port);
  }

  bool	WindowsSocket::operator!=(INativeSocket const & other) const
  {
    return (!(*this == other));
  }

  Error::Code		WindowsSocket::socket(std::string const & protocol)
  {
    char		val;
    unsigned int	size = sizeof(val);
    int			flags;

    close();

    ScopedLock		lock(_mutex);

    val = 1;
    flags = SOCK_STREAM;
    if (protocol == "UDP")
      flags = SOCK_DGRAM;
    if ((_sock = ::socket(AF_INET, flags, NULL)) == INVALID_SOCKET)
      return (Error::CannotCreateSocket);
    if (setsockopt(_sock, SOL_SOCKET, SO_REUSEADDR, &val, size) == SOCKET_ERROR)
      return (Error::CannotCreateSocket);
    return (Error::NoError);
  }

  Error::Code		WindowsSocket::connect(std::string const & host, unsigned int port)
  {
    ScopedLock		lock(_mutex);
    struct hostent	*he;

    if (_sock == INVALID_SOCKET)
      return (Error::NoSocket);
    memset(&_sa, 0, sizeof(_sa));
    _sa.sin_family = AF_INET;
    _sa.sin_port = htons(static_cast<u_short>(port));
    if ((he = gethostbyname(host.c_str())) == NULL || he->h_length < 1)
      return (Error::UnknowHost);
    memcpy(&_sa.sin_addr, he->h_addr, he->h_length);
    if (::connect(_sock, reinterpret_cast<SOCKADDR *>(&_sa), sizeof(_sa)) == SOCKET_ERROR)
      return (Error::CannotConnectSocket);
    return (Error::NoError);
  }

  Error::Code		WindowsSocket::bind(unsigned int port, std::string const & host)
  {
    ScopedLock		lock(_mutex);
    struct hostent	*he;

    if (_sock == INVALID_SOCKET)
      return (Error::NoSocket);
    memset(&_sa, 0, sizeof(_sa));
    _sa.sin_family = AF_INET;
    _sa.sin_port = htons(static_cast<u_short>(port));
    if (host == "")
      _sa.sin_addr.s_addr = htonl(INADDR_ANY);
    else
      {
	if ((he = gethostbyname(host.c_str())) == NULL || he->h_length < 1)
	  return (Error::UnknowHost);
	memcpy(&_sa.sin_addr, he->h_addr, he->h_length);
      }
    if (::bind(_sock, reinterpret_cast<SOCKADDR *>(&_sa), sizeof(_sa)) == SOCKET_ERROR)
      return (Error::CannotBindSocket);
    return (Error::NoError);
  }

  Error::Code	WindowsSocket::listen(unsigned int clients)
  {
    ScopedLock		lock(_mutex);

    if (_sock == INVALID_SOCKET)
      return (Error::NoSocket);
    if (::listen(_sock, clients) == SOCKET_ERROR)
      return (Error::CannotBindSocket);
    return (Error::NoError);
  }

  Error::Code	WindowsSocket::close()
  {
    ScopedLock		lock(_mutex);

    if (_sock != INVALID_SOCKET)
      {
	closesocket(_sock);
	_sock = INVALID_SOCKET;
	memset(&_sa, 0, sizeof(_sa));
      }
    return (Error::NoError);
  }
    
  void			WindowsSocket::setSockets(SocketSet& set, fd_set& nset, SOCKET& max)
  {
    SOCKET		sock;

    FD_ZERO(&nset);
    for (SocketSet::Iter it = set.begin(); it != set.end(); ++it)
      {
        sock = reinterpret_cast<WindowsSocket const *>(*it)->_sock;
        FD_SET(sock, &nset);
        max = std::max<SOCKET>(sock, max);
      }
  }

  void			WindowsSocket::removeSockets(SocketSet& set, fd_set& nset)
  {
    SOCKET		sock;

    for (SocketSet::Iter it = set.begin(); it != set.end();)
      {
	sock = reinterpret_cast<WindowsSocket const *>(*it)->_sock;
	if (!FD_ISSET(sock, &nset))
	  it = set.removeSocket(it);
	else
	  ++it;
      }
  }
    
  Error::Code       WindowsSocket::select(SocketSet& read, SocketSet& write, Time* timeout, bool* hasStdin)
  {
    ScopedLock      lock(_mutex);
    SOCKET          max = INVALID_SOCKET;
    fd_set          fread;
    fd_set          fwrite;
    struct timeval  ntime;
    int			ret;

    setSockets(read, fread, max);
    setSockets(write, fwrite, max);
    if (timeout != NULL)
      timeout->toTimeval(ntime);
    if (hasStdin != NULL)
      *hasStdin = false;
    ret = ::select(static_cast<int>(max) + 1, &fread, &fwrite, NULL, timeout != NULL ? &ntime : NULL);
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
    return (Error::NoError);
  }    

  Error::Code		WindowsSocket::accept(INativeSocket** client)
  {
    ScopedLock		lock(_mutex);
    WindowsSocket	*ns;
    SOCKET		sock;
    SOCKADDR_IN		sa;
    int			len = sizeof(_sa);

    *client = NULL;
    if (_sock == INVALID_SOCKET)
      return (Error::NoSocket);
    sock = ::accept(_sock, reinterpret_cast<SOCKADDR *>(&sa), &len);
    if (sock != SOCKET_ERROR)
      {
	ns = new WindowsSocket();
	ns->_sock = sock;
	memcpy(&ns->_sa, &sa, len);
	*client = ns;
	return (Error::NoError);
      }
    return (Error::CannotConnectSocket);
  }

  int			WindowsSocket::recv(void *buffer, size_t size)
  {
    ScopedLock		lock(_mutex);
    int			ret = 0;
    char		*cbuffer = static_cast<char *>(buffer);

    if (_sock == -1)
      return (Error::NoSocket);
    ret = ::recv(_sock, cbuffer, static_cast<int>(size), 0);
    if (ret <= -1)
      return (INativeSocket::Error);
    else if (ret == 0)
      return (INativeSocket::Disconnected);
    return (ret);
  }

  int			WindowsSocket::recvfrom(void *buffer, size_t size, INativeSocket** client)
  {
    ScopedLock		lock(_mutex);
    WindowsSocket*	ns;
    int			ret = 0;
    char		*cbuffer = static_cast<char *>(buffer);
    SOCKADDR_IN		sa;
    int			len = sizeof(sa);

    *client = NULL;
    if (_sock == -1)
      return (Error::NoSocket);
    memset(&sa, 0, sizeof(sa));
    ret = ::recvfrom(_sock, cbuffer, static_cast<int>(size), 0,
		     reinterpret_cast<SOCKADDR *>(&sa), &len);
    ns = new WindowsSocket();
    ns->_sock = _sock;
    memcpy(&ns->_sa, &sa, len);
    *client = ns;
    if (ret <= -1)
      return (INativeSocket::Error);
    else if (ret == 0)
      return (INativeSocket::Disconnected);
    return (ret);
  }

  int		WindowsSocket::send(void const *buffer, size_t size)
  {
    ScopedLock	lock(_mutex);
    int		ret = 0;
    char const	*cbuffer = static_cast<char const *>(buffer);

    if (_sock == INVALID_SOCKET)
      return (Error::NoSocket);
    ret = ::sendto(_sock, cbuffer, static_cast<int>(size), 0,
		   reinterpret_cast<SOCKADDR *>(&_sa), sizeof(_sa));
    if (ret <= -1)
      return (INativeSocket::Error);
    return (ret);
  }

  std::string	WindowsSocket::getIPAddress() const
  {
    ScopedLock	lock(_mutex);
    std::string	ret;

    if (_sock == INVALID_SOCKET)
      return ("");
    ret = inet_ntoa(_sa.sin_addr);
    return (ret);
  }

  std::string		WindowsSocket::getIPConnectedOn() const
  {
    ScopedLock		lock(_mutex);
    std::string		ret;
    SOCKADDR_IN		sa;
    int		 	len;

    if (_sock == INVALID_SOCKET)
      return ("");
    len = sizeof(sa);
    memset(&sa, 0, sizeof(sa));
    getsockname(_sock, reinterpret_cast<SOCKADDR *>(&sa), &len);
    ret = inet_ntoa(sa.sin_addr);
    return (ret);
  }

  unsigned int		WindowsSocket::getPort() const
  {
    ScopedLock		lock(_mutex);
    SOCKADDR_IN		sa;
    int			len;

    if (_sock == INVALID_SOCKET)
      return (0);
    len = sizeof(sa);
    memset(&sa, 0, sizeof(sa));
    getsockname(_sock, reinterpret_cast<SOCKADDR *>(&sa), &len);
    return (ntohs(sa.sin_port));
  }
}
