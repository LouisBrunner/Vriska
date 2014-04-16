
#include <Vriska/Network/SocketClient.h>

namespace Vriska
{
  SocketClient::SocketClient() : _own(true)
  {
  }

  SocketClient::SocketClient(INativeSocket& sysSocket) : Socket(sysSocket), _own(false)
  {
  }

  SocketClient::~SocketClient()
  {
    disconnect();
  }

  VRISKA_ACCESSIBLE
  std::string		SocketClient::getIPAddress() const
  {
    ScopedLock		lock(_mutex);

    if (!_connected)
      return ("");
    return (_socket.getIPAddress());
  }

  VRISKA_ACCESSIBLE
  std::string		SocketClient::getIPConnectedOn() const
  {
    ScopedLock		lock(_mutex);

    if (!_connected)
      return ("");
    return (_socket.getIPConnectedOn());
  }

  Error::Code		SocketClient::connect(std::string const & host, unsigned int port)
  {
    ScopedLock		lock(_mutex);
    Error::Code		err;
    std::string		proto = Socket::protocolToString(_protocol);

    if (_connected)
      return (Error::AlreadyConnected);
    sysLog("Trying to connect on " + host + ":" + StringUtils::toString<unsigned int>(port) + " (" + proto + ") ...");
    if ((err = _socket.socket(proto)) == Error::NoError)
      if ((err = _socket.connect(host, port)) == Error::NoError)
	{
	  sysLog("Connected");
	  _connected = true;
	  return (Error::NoError);
	}
    _socket.close();
    sysLog("Error occured: " + Error::getMessage(err));
    return (err);
  }

  Error::Code		SocketClient::disconnect()
  {
    ScopedLock		lock(_mutex);

    return (internDisconnect());
  }

  Error::Code		SocketClient::internDisconnect()
  {
    if (!_connected)
      return (Error::NotConnected);
    _connected = false;
    sysLog("Disconnected");
    if (!_own && _protocol == UDP)
      return (Error::NoError);
    return (_socket.close());
  }

  Error::Code		SocketClient::sync(SocketSet& read, SocketSet& write, Time* timeout, bool* hasStdin)
  {
    ScopedLock		lock(_mutex);

    if (!_connected)
      return (Error::NotConnected);
    return (_socket.select(read, write, timeout, hasStdin));
  }

  int			SocketClient::recv(void *buffer, size_t size, bool log)
  {
    ScopedLock		lock(_mutex);
    int			ret;

    if (!_connected)
      return (Error::NotConnected);
    ret = _socket.recv(buffer, size);
    if (log && ret > 0)
      sysLog("Received: " + StringUtils::escape(std::string(static_cast<char *>(buffer), ret)));
    if (ret < 1)
      {
	if (_protocol != Socket::UDP)
	  {
	    internDisconnect();
	    return (Socket::Error);
	  }
	return (0);
      }
    return (ret);
  }

  int			SocketClient::send(void const *buffer, size_t size, bool log)
  {
    ScopedLock		lock(_mutex);
    int			ret = -1;

    if (!_connected)
      return (Error::NotConnected);
    ret = _socket.send(buffer, size);
    if (log && ret > 0)
      sysLog("Sent: " + StringUtils::escape(std::string(static_cast<char const *>(buffer), ret)));
    if (ret < 0)
      {
	if (_protocol != Socket::UDP)
	  {
	    internDisconnect();
	    return (Socket::Error);
	  }
	return (0);
      }
    return (ret);
  }
}
