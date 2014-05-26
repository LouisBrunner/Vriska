
#include <Vriska/Network/Client.h>

namespace Vriska
{
  VRISKA_ACCESSIBLE
  Client::Client(Socket::Protocol protocol) :
    _tried(false), _port(0), _timeExact(true),
    _callbacks(NULL), _stdinWatcher(NULL), _timeout(NULL)
  {
    setLoggingTag("Client " + StringUtils::toString(this));
    setProtocol(protocol);
  }

  VRISKA_ACCESSIBLE
  Client::~Client()
  {
    stopThread();
  }

  VRISKA_ACCESSIBLE
  Error::Code		Client::connect(std::string const & host, unsigned int port, bool force)
  {
    Error::Code	err;

    if (isConnected() && !force)
      return (Error::AlreadyConnected);
    else if (isConnected())
      if ((err = disconnect()) != Error::NoError)
	return (err);
    if ((err = SocketClient::connect(host, port)) == Error::NoError)
      {
	_host = host;
	_port = port;
	_tried = true;
      }
    return (err);
  }

  VRISKA_ACCESSIBLE
  Error::Code		Client::disconnect()
  {
    if (!isConnected())
      return (Error::NotConnected);
    return (SocketClient::disconnect());
  }

  VRISKA_ACCESSIBLE
  Error::Code		Client::reconnect()
  {
    if (!_tried)
      return (Error::NotConnected);
    return (connect(_host, _port, true));
  }
  
  VRISKA_ACCESSIBLE
  Error::Code		Client::launch()
  {
    Error::Code	err = Error::NoError;

    while (isConnected() && err == Error::NoError)
      err = manageIO(true, true);
    return (err);
  }

  VRISKA_ACCESSIBLE
  void		Client::runThread()
  {
    if (!_thd.isAlive())
        _thd.launch(*this);
  }

  VRISKA_ACCESSIBLE
  void		Client::stopThread()
  {
    _thd.join();
  }

  VRISKA_ACCESSIBLE
  Error::Code		Client::waitUntilSent(bool callBack, bool timeOut)
  {
    Error::Code	err = Error::NoError;

    while (isConnected() && err == Error::NoError && _brWrite.sizeUsed() > 0)
      err = manageIO(callBack, timeOut);
    return (err);
  }

  VRISKA_ACCESSIBLE
  Error::Code		Client::waitUntilDelim(std::string const & delim, bool callBack, bool timeOut)
  {
    Error::Code	err = Error::NoError;

    while (isConnected() && err == Error::NoError && _brRead.search(delim) == BufferRing::Error)
      err = manageIO(callBack, timeOut);
    return (err);
  }

  VRISKA_ACCESSIBLE
  Error::Code		Client::waitUntilSize(size_t n, bool callBack, bool timeOut)
  {
    Error::Code	err = Error::NoError;

    while (isConnected() && err == Error::NoError && _brRead.sizeUsed() >= n)
      err = manageIO(callBack, timeOut);
    return (err);
  }

  VRISKA_ACCESSIBLE
  Error::Code		Client::waitLine(bool callBack, bool timeOut)
  {
    return (waitUntilDelim("\n", callBack, timeOut));
  }

  VRISKA_ACCESSIBLE
  Error::Code		Client::waitFor(Time const & t, bool callBack, bool timeOut)
  {
    Error::Code	err = Error::NoError;

    _waitElapsed = Time::Zero;
    _waitTarget = t;
    while (isConnected() && err == Error::NoError && _waitElapsed < _waitTarget)
      err = manageIO(callBack, timeOut);
    _waitTarget = Time::Zero;
    return (err);
  }

  Error::Code	Client::manageIO(bool callBack, bool timeOut)
  {
    Error::Code	err;
    SocketSet	read;
    SocketSet	write;
    Time	timeout = getTimeout();
    bool	hasStdin = false;

    if (sizeCanRead() > 0)
      read.pushSocket(&_socket);
    if (sizeToWrite() > 0)
      write.pushSocket(&_socket);
    err = sync(read, write, hasTimeout() ? &timeout : NULL, watchStdin() ? &hasStdin : NULL);
    if (err == Error::NoError)
      {
	if (callBack && hasStdin)
	  if (!callbackStdin())
	    {
	      disconnect();
	      return (Error::Disconnected);
	    }

	if (read.hasSocket(&_socket))
	  {
	    if ((err = doReceive()) != Error::NoError)
	      {
		if (err != Error::NoPlaceR)
		  return (err);
	      }
	    if (callBack)
	      if (!callbackReceive())
		return (Error::Disconnected);
	  }
	if (write.hasSocket(&_socket))
	  {
	    if ((err = doSend()) != Error::NoError)
	      return (err);
	    if (callBack)
	      if (!callbackSend())
		return (Error::Disconnected);
	  }
	if (timeOut && hasTimeout())
	  {
	    addTimeout(timeout);
	    if ((_timeExact && _timeElapsed >= _timeTarget) || !_timeExact)
	      {
		bool	ret = callbackTimeout();

		_timeElapsed = Time::Zero;
		if (!ret)
		  return (Error::Disconnected);
	      }
	  }
	if (_waitTarget != Time::Zero)
	  addTime(timeout);
	return (Error::NoError);
      }
    return (err);
  }

  VRISKA_ACCESSIBLE
  void			Client::registerCallbacks(IClientCallable* callbacks)
  {
    _callbacks = callbacks;
  }

  VRISKA_ACCESSIBLE
  void			Client::unregisterCallbacks()
  {
    _callbacks = NULL;
  }
  
  VRISKA_ACCESSIBLE
  void			Client::registerStdinWatcher(IClientStdinWatcher* stdinWatcher)
  {
    _stdinWatcher = stdinWatcher;
  }

  VRISKA_ACCESSIBLE
  void          Client::unregisterStdinWatcher()
  {
    _stdinWatcher = NULL;
  }

  VRISKA_ACCESSIBLE
  void			Client::setTimeout(Time const & t, IClientTimeoutable *timeout, bool exact)
  {
    _timeTarget = t;
    _timeElapsed = Time::Zero;
    _timeExact = exact;
    _timeout = timeout;
  }

  VRISKA_ACCESSIBLE
  void			Client::unsetTimeout()
  {
    _timeout = NULL;
  }

  VRISKA_ACCESSIBLE
  Time const &	Client::getElapsedTime() const
  {
    return (_timeElapsed);
  }

  void			Client::run()
  {
    launch();
  }

  bool			Client::hasTimeout() const
  {
    return (_timeout != NULL);
  }

  Time			Client::getTimeout() const
  {
    if (_waitTarget != Time::Zero)
      return (_waitTarget - _waitElapsed);
    if (!_timeExact)
      return (_timeTarget);
    return (_timeTarget - _timeElapsed);
  }

  void			Client::addTime(Time const & elapsed)
  {
    _waitElapsed += elapsed;
  }

  void			Client::addTimeout(Time const & elapsed)
  {
    _timeElapsed += elapsed;
  }

  bool			Client::watchStdin() const
  {
    return (_stdinWatcher != NULL);
  }

  bool			Client::callbackReceive()
  {
    bool		ret = true;

    if (_callbacks != NULL)
      ret = _callbacks->onReceive(*this);
    if (!ret)
      disconnect();
    return (ret);
  }

  bool			Client::callbackSend()
  {
    bool		ret = true;

    if (_callbacks != NULL)
      ret = _callbacks->onSend(*this);
    if (!ret)
      disconnect();
    return (ret);
  }

  bool			Client::callbackStdin()
  {
    bool		ret = true;

    if (_stdinWatcher != NULL)
      ret = _stdinWatcher->onStdin(*this);
    if (!ret)
      disconnect();
    return (ret);
  }

  bool			Client::callbackTimeout()
  {
    bool		ret = true;

    if (_timeout != NULL)
      ret = _timeout->onTimeout(*this);
    _timeElapsed = Time::Zero;
    if (!ret)
      disconnect();
    return (ret);
  }
}
