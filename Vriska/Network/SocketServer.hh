
#ifndef VRISKA_LIB_NETWORK_SOCKETSERVER_HH_
# define VRISKA_LIB_NETWORK_SOCKETSERVER_HH_

# include <Vriska/Network/Socket.hh>
# include <Vriska/Network/SimpleClient.hh>

namespace Vriska
{
  class SocketServer : public Socket
  {
  public:
    SocketServer();
    virtual ~SocketServer();

  private:
    SocketServer(Socket const & other);
    Socket const &	operator=(Socket const & other);

  public:
    std::string		getIPAddress() const;

  protected:
    Error::Code		connect(unsigned int port, std::string const & host = "");
    Error::Code		disconnect();

    Error::Code		sync(SocketSet& read, SocketSet& write,
			     Time* timeout = NULL, bool* hasStdin = NULL);
    INativeSocket	*newClient(char **buffer, unsigned int *size);
  };
}

#endif // !VRISKA_LIB_NETWORK_SOCKETSERVER_HH
