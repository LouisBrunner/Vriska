
#ifndef VRISKA_LIB_NETWORK_SOCKET_H_
# define VRISKA_LIB_NETWORK_SOCKET_H_

# include <Vriska/Network/INativeSocket.h>

# include <Vriska/Threads/Mutex.h>
# include <Vriska/Threads/ScopedLock.h>

# include <Vriska/Network/Logger.h>

namespace Vriska
{
  class VRISKA_EXPORT Socket : public Logger
  {
  public:
    static const int	Error = INativeSocket::Error;
    static const int	Disconnected = INativeSocket::Disconnected;
    static const int	FreePort = 0;
    
    enum Protocol
      {
	TCP = 0,
	UDP
      };
    
  public:
    Socket();
    Socket(INativeSocket& sysSocket);
    virtual ~Socket();

  private:
    Socket(Socket const & other);
    Socket const &	operator=(Socket const & other);

  public:
    bool	isConnected() const;
    void	setProtocol(Protocol protocol);

    virtual unsigned int	getPort() const;
    
  protected:
    virtual Error::Code	disconnect() = 0;
    virtual Error::Code	sync(SocketSet& read, SocketSet& write,
			     Time* timeout = NULL, bool* hasStdin = NULL) = 0;

    static std::string		protocolToString(Protocol protocol);

  protected:
    Mutex		_mutex;
    bool		_connected;
    Protocol		_protocol;
    INativeSocket&	_socket;
  };
}

#endif // !VRISKA_LIB_NETWORK_SOCKET_H_
