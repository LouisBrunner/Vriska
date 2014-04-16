
#include <Vriska/Network/Server.h>

// Forward declaration completion
#include <Vriska/Network/ICallable.h>

namespace Vriska
{
  VRISKA_ACCESSIBLE
  Server::Client::Client(INativeSocket& sysSocket, Server& server, unsigned int n) : SimpleClient(sysSocket), _server(server), _n(n), _logger(n, false)
  {
    _connected = true;
  }

  VRISKA_ACCESSIBLE
  Server::Client::~Client()
  {
  }

  VRISKA_ACCESSIBLE
  unsigned int	Server::Client::getIndex() const
  {
    return (_n);
  }

  VRISKA_ACCESSIBLE
  void			Server::Client::setLogging(bool val, std::ostream& os)
  {
    _logger.enable(val);
    _logger.setStream(os);
  }

  VRISKA_ACCESSIBLE
  void	Server::Client::destroy()
  {
    SocketClient::disconnect();
    _server.disconnectMe(this);
  }

  Error::Code		Server::Client::sync(bool write)
  {
    Error::Code	err = Error::NoError;

    if (write)
      err = doWrite();
    else if (_protocol != UDP)
      err = doRead();
    return (err);
  }

  void			Server::Client::sysLog(std::string const & info)
  {
    _logger.log(info);
  }

  VRISKA_ACCESSIBLE
  void			Server::Client::log(std::string const & info)
  {
    _logger.userLog(info);
  }

  int		Server::Client::writeOnBuffer(char const *buffer, size_t size)
  {
    sysLog("Received: " + StringUtils::escape(std::string(buffer, size)));
    return (_brRead.write(buffer, size));
  }

  INativeSocket const &	Server::Client::getNativeSocket() const
  {
    return (_socket);
  }

  VRISKA_ACCESSIBLE
  Server::Server(Socket::Protocol protocol) : _n(0), _limit(0), _logger(false),
					      _tried(false), _port(0),
					      _timeExact(true),
					      _callConn(NULL), _funcConn(NULL), _callDisc(NULL), _funcDisc(NULL),
					      _callRead(NULL), _funcRead(NULL), _callWrite(NULL), _funcWrite(NULL),
					      _callStdin(NULL), _funcStdin(NULL), _callTime(NULL), _funcTime(NULL)
  {
    setProtocol(protocol);
  }

  VRISKA_ACCESSIBLE
  Server::~Server()
  {
    clearClients();
  }

  VRISKA_ACCESSIBLE
  Error::Code		Server::connect(unsigned int port, std::string const & host, bool force)
  {
    Error::Code	err;

    if (isConnected() && !force)
      return (Error::AlreadyConnected);
    else if (isConnected())
      if ((err = disconnect()) != Error::NoError)
	return (err);
    if ((err = SocketServer::connect(port, host)) == Error::NoError)
      {
	_host = host;
	_port = Socket::getPort();
	_tried = true;
      }
    return (err);
  }

  VRISKA_ACCESSIBLE
  Error::Code			Server::connect(unsigned int const *ports, std::string const & host, bool force)
  {
    Error::Code		err = Error::NoError;

    if (ports[0] > ports[1])
      return (Error::InvalidArgument);
    for (unsigned int i = ports[0]; i <= ports[1]; ++i)
      if ((err = connect(i, host, force)) == Error::NoError)
	break;
    return (err);
  }

  VRISKA_ACCESSIBLE
  Error::Code		Server::connect(std::list<unsigned int> const & ports, std::string const & host, bool force)
  {
    Error::Code		err = Error::NoError;

    for (std::list<unsigned int>::const_iterator it = ports.begin(); it != ports.end(); ++it)
      if ((err = connect(*it, host, force)) == Error::NoError)
	break;
    return (err);
  }

  VRISKA_ACCESSIBLE
  Error::Code			Server::connect(unsigned int port1, unsigned int port2, std::string const & host, bool force)
  {
    Error::Code		err = Error::NoError;

    if (port1 > port2)
      return (Error::InvalidArgument);
    for (unsigned int i = port1; i <= port2; ++i)
      if ((err = connect(i, host, force)) == Error::NoError)
	break;
    return (err);
  }

  VRISKA_ACCESSIBLE
  Error::Code		Server::reconnect()
  {
    if (!_tried)
      return (Error::NotConnected);
    return (connect(_port, _host, true));
  }

  VRISKA_ACCESSIBLE
  Error::Code		Server::disconnect()
  {
    if (!isConnected())
      return (Error::NotConnected);
    _mutex.lock();
    _n = 0;
    clearClients();
    _mutex.unlock();
    return (SocketServer::disconnect());
  }

  VRISKA_ACCESSIBLE
  Error::Code		Server::launch()
  {
    Error::Code	err = Error::NoError;

    while (isConnected() && err == Error::NoError)
      err = manageIO();
    return (err);
  }

  VRISKA_ACCESSIBLE
  void			Server::log(std::string const & info)
  {
    _logger.userLog(info);
  }

  void			Server::sysLog(std::string const & info)
  {
    _logger.log(info);
  }

  VRISKA_ACCESSIBLE
  void			Server::setLogging(bool val, std::ostream& os)
  {
    _logger.enable(val);
    _logger.setStream(os);
    for (Iter it = _clients.begin(); it != _clients.end(); ++it)
      (*it)->setLogging(val, os);
  }

  VRISKA_ACCESSIBLE
  void			Server::setLimit(unsigned int limit)
  {
    _limit = limit;
  }

  VRISKA_ACCESSIBLE
  void			Server::registerOnRead(FunctionC func)
  {
    _funcRead = func;
    _callRead = NULL;
  }

  VRISKA_ACCESSIBLE
  void			Server::registerOnRead(IServerCCallable *call)
  {
    _funcRead = NULL;
    _callRead = call;
  }

  VRISKA_ACCESSIBLE
  void			Server::unregisterOnRead()
  {
    _funcRead = NULL;
    _callRead = NULL;
  }

  VRISKA_ACCESSIBLE
  void			Server::registerOnWrite(FunctionC func)
  {
    _funcWrite = func;
    _callWrite = NULL;
  }

  VRISKA_ACCESSIBLE
  void			Server::registerOnWrite(IServerCCallable *call)
  {
    _funcWrite = NULL;
    _callWrite = call;
  }

  VRISKA_ACCESSIBLE
  void			Server::unregisterOnWrite()
  {
    _funcWrite = NULL;
    _callWrite = NULL;
  }

  VRISKA_ACCESSIBLE
  void			Server::registerOnConnect(FunctionC func)
  {
    _funcConn = func;
    _callConn = NULL;
  }

  VRISKA_ACCESSIBLE
  void			Server::registerOnConnect(IServerCCallable *call)
  {
    _funcConn = NULL;
    _callConn = call;
  }

  VRISKA_ACCESSIBLE
  void			Server::unregisterOnConnect()
  {
    _funcConn = NULL;
    _callConn = NULL;
  }

  VRISKA_ACCESSIBLE
  void			Server::registerOnDisconnect(FunctionC func)
  {
    _funcDisc = func;
    _callDisc = NULL;
  }

  VRISKA_ACCESSIBLE
  void			Server::registerOnDisconnect(IServerCCallable *call)
  {
    _funcDisc = NULL;
    _callDisc = call;
  }

  VRISKA_ACCESSIBLE
  void			Server::unregisterOnDisconnect()
  {
    _funcDisc = NULL;
    _callDisc = NULL;
  }

  VRISKA_ACCESSIBLE
  void			Server::registerOnStdin(Function func)
  {
    _funcStdin = func;
    _callStdin = NULL;
  }

  VRISKA_ACCESSIBLE
  void			Server::registerOnStdin(IServerCallable *call)
  {
    _funcStdin = NULL;
    _callStdin = call;
  }

  VRISKA_ACCESSIBLE
  void			Server::unregisterOnStdin()
  {
    _funcStdin = NULL;
    _callStdin = NULL;
  }

  VRISKA_ACCESSIBLE
  void			Server::setTimeout(Time const & t, Function func, bool exact)
  {
    _timeTarget = t;
    _timeElapsed = Time::Zero;
    _timeExact = exact;
    _funcTime = func;
    _callTime = NULL;
  }

  VRISKA_ACCESSIBLE
  void			Server::setTimeout(Time const & t, IServerCallable *call, bool exact)
  {
    _timeTarget = t;
    _timeElapsed = Time::Zero;
    _timeExact = exact;
    _funcTime = NULL;
    _callTime = call;
  }

  VRISKA_ACCESSIBLE
  void			Server::unsetTimeout()
  {
    _funcTime = NULL;
    _callTime = NULL;
  }

  VRISKA_ACCESSIBLE
  Time const &	Server::getElapsedTime() const
  {
    return (_timeElapsed);
  }

  void			Server::disconnectMe(Client* client)
  {
    Iter		it = std::find(_clients.begin(), _clients.end(), client);

    if (it != _clients.end())
      _clients.erase(it);
    delete client;
  }

  bool			Server::hasTimeout() const
  {
    return (_callTime != NULL || _funcTime != NULL);
  }

  Time			Server::getTimeout() const
  {
    if (!_timeExact)
      return (_timeTarget);
    return (_timeTarget - _timeElapsed);
  }

  bool			Server::watchStdin() const
  {
    return (_funcStdin != NULL || _callStdin != NULL);
  }

  VRISKA_ACCESSIBLE
  unsigned int	Server::getPort() const
  {
    return (_port);
  }

  void			Server::addTimeout(Time const & elapsed)
  {
    _timeElapsed += elapsed;
  }

  bool			Server::callbackTimeout()
  {
    bool		ret = true;

    if (_funcTime != NULL)
      ret = (*_funcTime)(*this);
    else if (_callTime != NULL)
      ret = (*_callTime)(*this);
    if (!ret)
      disconnect();
    return (ret);
  }

  bool			Server::callbackConnect(Client& client)
  {
    bool		ret = true;

    if (_funcConn != NULL)
      ret = (*_funcConn)(*this, client);
    else if (_callConn != NULL)
      ret = (*_callConn)(*this, client);
    return (ret);
  }

  bool			Server::callbackDisconnect(Client& client)
  {
    bool		ret = true;

    if (_funcDisc != NULL)
      ret = (*_funcDisc)(*this, client);
    else if (_callDisc != NULL)
      ret = (*_callDisc)(*this, client);
    return (ret);
  }

  bool			Server::callbackRead(Client& client)
  {
    bool		ret = true;

    if (_funcRead != NULL)
      ret = (*_funcRead)(*this, client);
    else if (_callRead != NULL)
      ret = (*_callRead)(*this, client);
    return (ret);
  }

  bool			Server::callbackWrite(Client& client)
  {
    bool		ret = true;

    if (_funcWrite != NULL)
      ret = (*_funcWrite)(*this, client);
    else if (_callWrite != NULL)
      ret = (*_callWrite)(*this, client);
    return (ret);
  }

  bool			Server::callbackStdin()
  {
    bool		ret = true;

    if (_funcStdin != NULL)
      ret = (*_funcStdin)(*this);
    else if (_callStdin != NULL)
      ret = (*_callStdin)(*this);
    if (!ret)
      disconnect();
    return (ret);
  }

  Server::Client	*Server::addNewClient(INativeSocket *sock)
  {
    Client		*client = new Client(*sock, *this, _n);

    if (_limit && _clients.size() > _limit)
      {
	sysLog("Client " + StringUtils::toString<unsigned int>(_n) + ": rejected, too much connections (limit " + StringUtils::toString<unsigned int>(_limit) + ")");
	delete client;
      }
    sysLog("Client " + StringUtils::toString<unsigned int>(_n) + " connected from " + client->getIPAddress() + " to " + client->getIPConnectedOn());
    ++_n;
    client->setLogging(_logger.isLogging(), _logger.getStream());
    client->setProtocol(_protocol);
    if (!callbackConnect(*client))
      {
	disconnectMe(client);
	return (NULL);
      }
    else
      _clients.push_back(client);
    return (client);
  }

  Server::Client	*Server::getFromSocket(INativeSocket const *sock)
  {
    for (Iter it = _clients.begin(); it != _clients.end(); ++it)
      if ((*it)->getNativeSocket() == *sock)
	return (*it);
    return (NULL);
  }

  void			Server::manageClients(SocketSet& set, bool mode)
  {
    for (SocketSet::Iter it = set.begin(); it != set.end(); ++it)
      {
	Client	*client = getFromSocket(*it);

	if (client != NULL)
	  if (client->sync(mode) != Error::NoError
	      || !(mode ? callbackWrite(*client) : callbackRead(*client)))
	    {
	      _clients.remove(client);
	      callbackDisconnect(*client);
	      delete client;
	    }
      }
  }

  Error::Code		Server::manageIO()
  {
    Error::Code		err;
    SocketSet		read;
    SocketSet		write;
    Time		timeout = getTimeout();
    bool		hasStdin = false;

    _mutex.lock();
    read.pushSocket(&_socket);
    for (Iter it = _clients.begin(); it != _clients.end(); ++it)
      {
	if ((*it)->sizeCanRead() > 0)
	  read.pushSocket(&(*it)->getNativeSocket());
	if ((*it)->sizeToWrite() > 0)
	  write.pushSocket(&(*it)->getNativeSocket());
      }
    _mutex.unlock();

    err = sync(read, write, hasTimeout() ? &timeout : NULL, watchStdin() ? &hasStdin : NULL);
    if (err == Error::NoError && isConnected())
      {
	if (hasStdin)
	  if (!callbackStdin())
	    {
	      disconnect();
	      return (Error::Disconnected);
	    }

	ScopedLock	lock(_mutex);

	if (read.hasSocket(&_socket))
	  {
	    INativeSocket*	sock;
	    char		*buffer = NULL;
	    unsigned int	size = 0;

	    if ((sock = newClient(&buffer, &size)) != NULL)
	      {
		if (buffer == NULL)
		  addNewClient(sock);
		else
		  {
		    Client	*client = getFromSocket(sock);
		    
		    if (client == NULL)
		      client = addNewClient(sock);
		    else if (size == 0)
		      {
			_clients.remove(client);
			callbackDisconnect(*client);
			delete client;
			client = NULL;
		      }
		    if (client != NULL)
		      client->writeOnBuffer(buffer, size);
		    delete[] buffer;
		    if (client != NULL && !callbackRead(*client))
		      {
			_clients.remove(client);
			callbackDisconnect(*client);
			delete client;
		      }
		  }
	      }
	    read.removeSocket(&_socket);
	  }

	if (hasTimeout())
	  {
	    addTimeout(timeout);
	    if ((_timeExact && _timeElapsed >= _timeTarget) || !_timeExact)
	      {
		bool ret = callbackTimeout();

		_timeElapsed = Time::Zero;
		if (!ret)
		  return (Error::Disconnected);
	      }
	  }

	manageClients(read, false);
	manageClients(write, true);
	return (Error::NoError);
      }
    return (err);
  }

  void		Server::clearClients()
  {
    for (Iter it = _clients.begin(); it != _clients.end(); ++it)
      delete (*it);
    _clients.clear();
  }

  VRISKA_ACCESSIBLE
  size_t  Server::broadcast(void const * buffer, size_t size)
  {
    return (broadcast(buffer, size, &yesMan));
  }

  bool		Server::yesMan(Server& server, Server::Client& client)
  {
    static_cast<void>(server);
    static_cast<void>(client);
    return (true);
  }
}
