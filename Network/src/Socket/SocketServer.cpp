
#include <Vriska/Network/SocketServer.h>
#include <Vriska/Network/Server.h>

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
    bool            connected = false;

    if (_connected)
      return (Error::AlreadyConnected);
    if ((err = _socket.socket(proto)) == Error::NoError)
      if ((err = _socket.bind(port, host)) == Error::NoError)
	    if (_protocol == UDP || (err = _socket.listen()) == Error::NoError)
	    {
            connected = true;
	    }

    {
        unsigned short  truePort = port;
        std::string     portString = "a dynamic port";

        if (connected && truePort == 0)
            truePort = getPort();
        if (truePort != 0)
            portString = "port " + StringUtils::toString<unsigned int>(truePort);

        sysLog("Trying to listen on " + portString
	            + (host != "" ? " (limited to " + host + ")" : "") + " (" + proto + ") ...");
    }
    if (connected)
    {
	    sysLog("Connected");
	    _connected = true;
        err = Error::NoError;
    }
    else
    {
        sysLog("Error occured: " + Error::getMessage(err));
        _socket.close();
    }
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
