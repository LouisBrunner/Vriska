
#ifndef VRISKA_LIB_NETWORK_WINDOWSSOCKET_H_
# define VRISKA_LIB_NETWORK_WINDOWSSOCKET_H_

# include <Vriska/Network/INativeSocket.h>

# include <winsock2.h>

namespace Vriska
{
  class WindowsSocket : public INativeSocket
  {
  public:
    WindowsSocket();
    ~WindowsSocket();

    bool	operator==(INativeSocket const & other) const;
    bool	operator!=(INativeSocket const & other) const;

  private:
    WindowsSocket(INativeSocket const & other);
    WindowsSocket const &	operator=(INativeSocket const & other);

  public:
    Error::Code	socket(std::string const & protocol = "TCP");
    Error::Code	connect(std::string const & host, unsigned int port);
    Error::Code	bind(unsigned int port, std::string const & host = "");
    Error::Code listen(unsigned int clients = INativeSocket::MaxClients);
    Error::Code	close();
    
    Error::Code	select(SocketSet& read, SocketSet& write, Time* timeout = NULL, bool* hasStdin = NULL);
    Error::Code	accept(INativeSocket** client);

    int		recv(void *buffer, size_t size);
    int		recvfrom(void *buffer, size_t size, INativeSocket** client);
    int		send(void const *buffer, size_t size);

  public:
    std::string	getIPAddress() const;
    std::string	getIPConnectedOn() const;
    unsigned int	getPort() const;

  private:
    static void	setSockets(SocketSet& set, fd_set& nset, SOCKET& max);
    static void	removeSockets(SocketSet& set, fd_set& nset);

  private:
    Mutex	_mutex;
    SOCKET	_sock;
    SOCKADDR_IN	_sa;
  };
}

#endif // !VRISKA_LIB_NETWORK_WINDOWSSOCKET_H_
