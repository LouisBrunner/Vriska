
#ifndef VRISKA_LIB_NETWORK_SOCKETCLIENT_HH_
# define VRISKA_LIB_NETWORK_SOCKETCLIENT_HH_

# include <Vriska/Network/Socket.hh>

namespace Vriska
{
  class VRISKA_EXPORT SocketClient : public Socket
  {
  public:
    SocketClient();
    SocketClient(INativeSocket& sysSocket);
    virtual ~SocketClient();

  private:
    SocketClient(Socket const & other);
    Socket const &	operator=(Socket const & other);

  public:
    std::string		getIPAddress() const;
    std::string		getIPConnectedOn() const;

  protected:
    Error::Code		connect(std::string const & host, unsigned int port);
    Error::Code		disconnect();

    Error::Code		sync(SocketSet& read, SocketSet& write,
			     Time* timeout = NULL, bool* hasStdin = NULL);

    int			recv(void *buffer, size_t size, bool log = false);
    int			send(void const *buffer, size_t size, bool log = false);

  private:
    Error::Code		internDisconnect();

  private:
    bool	_own;
  };
}

#endif // !VRISKA_LIB_NETWORK_SOCKETCLIENT_HH
