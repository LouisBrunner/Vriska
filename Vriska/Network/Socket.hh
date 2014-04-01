
#ifndef VRISKA_LIB_NETWORK_SOCKET_HH_
# define VRISKA_LIB_NETWORK_SOCKET_HH_

# include <Vriska/Network/INativeSocket.hh>

# include <Vriska/Threads/Mutex.hh>
# include <Vriska/Threads/ScopedLock.hh>

namespace Vriska
{
  class VRISKA_EXPORT Socket
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
    virtual void	sysLog(std::string const & info);

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

#endif // !VRISKA_LIB_NETWORK_SOCKET_HH
