
#ifndef VRISKA_LIB_NETWORK_SOCKETSERVER_H_
# define VRISKA_LIB_NETWORK_SOCKETSERVER_H_

# include <Vriska/Network/Socket.h>
# include <Vriska/Network/SimpleClient.h>

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

#endif // !VRISKA_LIB_NETWORK_SOCKETSERVER_H_
