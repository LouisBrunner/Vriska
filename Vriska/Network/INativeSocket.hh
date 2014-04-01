
#ifndef VRISKA_LIB_NETWORK_INATIVESOCKET_HH_
# define VRISKA_LIB_NETWORK_INATIVESOCKET_HH_

# include <Vriska/Core/OS.hh>
# include <Vriska/Core/Error.hh>
# include <Vriska/Core/Time.hh>
# include <Vriska/Core/StringUtils.hpp>

# include <Vriska/Network/SocketSet.hh>

# include <Vriska/Threads/Mutex.hh>
# include <Vriska/Threads/ScopedLock.hh>

# include <string>

namespace Vriska
{
  class INativeSocket
  {
  public:
    static const int		Error = -1;
    static const int		Disconnected = 0;

  protected:
    static const unsigned int	MaxClients = 5;

  public:
    virtual ~INativeSocket() {}

    virtual bool	operator==(INativeSocket const &) const = 0;
    virtual bool	operator!=(INativeSocket const &) const = 0;

  public:
    virtual Error::Code	socket(std::string const & protocol = "TCP") = 0;
    virtual Error::Code	connect(std::string const & host, unsigned int port) = 0;
    virtual Error::Code	bind(unsigned int port, std::string const & host = "") = 0;
    virtual Error::Code	listen(unsigned int clients = MaxClients) = 0;
    virtual Error::Code	close() = 0;
    
    virtual Error::Code	select(SocketSet& read, SocketSet& write, Time* timeout = NULL, bool* hasStdin = NULL) = 0;
    virtual Error::Code	accept(INativeSocket** client) = 0;

    virtual int		recv(void *buffer, size_t size) = 0;
    virtual int		recvfrom(void *buffer, size_t size, INativeSocket** client) = 0;
    virtual int		send(void const *buffer, size_t size) = 0;

  public:
    virtual std::string	getIPAddress() const = 0;
    virtual std::string	getIPConnectedOn() const = 0;
    virtual unsigned int	getPort() const = 0;

  public:
    static INativeSocket*	create();
  };
}

#endif // !VRISKA_LIB_NETWORK_INATIVESOCKET_HH
