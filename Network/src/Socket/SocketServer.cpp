
#include <Vriska/Network/SocketServer.hh>
#include <Vriska/Network/Server.hh>

namespace Vriska
{
  SocketServer::SocketServer()
  {
  }

  SocketServer::~SocketServer()
  {
    disconnect();
  }

  std::string		SocketServer::getIPAddress() const
  {
    ScopedLock		lock(_mutex);

    if (!_connected)
      return ("");
    return (_socket.getIPAddress());
  }

  Error::Code		SocketServer::connect(unsigned int port, std::string const & host)
  {
    ScopedLock		lock(_mutex);
    Error::Code		err;
    std::string		proto = Socket::protocolToString(_protocol);

    if (_connected)
      return (Error::AlreadyConnected);
    sysLog("Trying to listen on port " + StringUtils::toString<unsigned int>(port)
	   + (host != "" ? " (limited to " + host + ")" : "") + " (" + proto + ") ...");
    if ((err = _socket.socket(proto)) == Error::NoError)
      if ((err = _socket.bind(port, host)) == Error::NoError)
	if (_protocol == UDP || (err = _socket.listen()) == Error::NoError)
	  {
	    sysLog("Connected");
	    _connected = true;
	    return (Error::NoError);
	  }
    _socket.close();
    sysLog("Error occured: " + Error::getMessage(err));
    return (err);
  }

  Error::Code		SocketServer::disconnect()
  {
    ScopedLock		lock(_mutex);

    if (!_connected)
      return (Error::NotConnected);
    _connected = false;
    sysLog("Disconnected");
    return (_socket.close());
  }

  Error::Code		SocketServer::sync(SocketSet& read, SocketSet& write, Time* timeout, bool* hasStdin)
  {
    ScopedLock		lock(_mutex);

    if (!_connected)
      return (Error::NotConnected);
    return (_socket.select(read, write, timeout, hasStdin));
  }

  INativeSocket		*SocketServer::newClient(char **buffer, unsigned int *size)
  {
    INativeSocket	*sock;

    *buffer = NULL;
    *size = 0;
    if (_protocol == Socket::UDP)
      {
	int		ret;
	char		*cbuffer = new char[SimpleClient::SizeBuffer];

	ret = _socket.recvfrom(cbuffer, SimpleClient::SizeBuffer, &sock);
	if (ret != Socket::Error && sock != NULL)
	  {
	    *buffer = cbuffer;
	    *size = ret;
	  }
	else
	  sock = NULL;
      }
    else if (_socket.accept(&sock) != Error::NoError)
      return (NULL);
    return (sock);
  }
}
