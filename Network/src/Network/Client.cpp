
#include <Vriska/Network/Client.h>

namespace Vriska
{
  VRISKA_ACCESSIBLE
  Client::Client(Socket::Protocol protocol) :
    _logger(false), _isBlocking(false),
    _tried(false), _port(0), _timeExact(true),
    _callRead(NULL), _funcRead(NULL),
    _callWrite(NULL), _funcWrite(NULL),
    _callStdin(NULL), _funcStdin(NULL),
    _callTime(NULL), _funcTime(NULL)
  {
    setProtocol(protocol);
  }

  VRISKA_ACCESSIBLE
  Client::~Client()
  {
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
  void			Client::log(std::string const & info)
  {
    _logger.userLog(info);
  }

  void			Client::sysLog(std::string const & info)
  {
    _logger.log(info);
  }

  VRISKA_ACCESSIBLE
  void			Client::setLogging(bool val, std::ostream& os)
  {
    _logger.enable(val);
    _logger.setStream(os);
  }

  VRISKA_ACCESSIBLE
  void			Client::setBlocking(bool val)
  {
    if (_isBlocking != val)
      {
	_brRead.enable(val);
	_brWrite.enable(val);
      }
    _isBlocking = val;
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
  Error::Code		Client::waitUntilWritten(bool callBack, bool timeOut)
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
	    if ((err = doRead()) != Error::NoError)
	      {
		if (err != Error::NoPlaceR)
		  return (err);
	      }
	    if (callBack)
	      if (!callbackRead())
		return (Error::Disconnected);
	  }
	if (write.hasSocket(&_socket))
	  {
	    if ((err = doWrite()) != Error::NoError)
	      return (err);
	    if (callBack)
	      if (!callbackWrite())
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
  void			Client::registerOnRead(Function func)
  {
    _funcRead = func;
    _callRead = NULL;
  }

  VRISKA_ACCESSIBLE
  void			Client::registerOnRead(IClientCallable *call)
  {
    _funcRead = NULL;
    _callRead = call;
  }

  VRISKA_ACCESSIBLE
  void			Client::unregisterOnRead()
  {
    _funcRead = NULL;
    _callRead = NULL;
  }

  VRISKA_ACCESSIBLE
  void			Client::registerOnWrite(Function func)
  {
    _funcWrite = func;
    _callWrite = NULL;
  }

  VRISKA_ACCESSIBLE
  void			Client::registerOnWrite(IClientCallable *call)
  {
    _funcWrite = NULL;
    _callWrite = call;
  }

  VRISKA_ACCESSIBLE
  void			Client::unregisterOnWrite()
  {
    _funcWrite = NULL;
    _callWrite = NULL;
  }

  VRISKA_ACCESSIBLE
  void			Client::registerOnStdin(Function func)
  {
    _funcStdin = func;
    _callStdin = NULL;
  }

  VRISKA_ACCESSIBLE
  void			Client::registerOnStdin(IClientCallable *call)
  {
    _funcStdin = NULL;
    _callStdin = call;
  }

  VRISKA_ACCESSIBLE
  void			Client::unregisterOnStdin()
  {
    _funcStdin = NULL;
    _callStdin = NULL;
  }

  VRISKA_ACCESSIBLE
  void			Client::setTimeout(Time const & t, Function func, bool exact)
  {
    _timeTarget = t;
    _timeElapsed = Time::Zero;
    _timeExact = exact;
    _funcTime = func;
    _callTime = NULL;
  }

  VRISKA_ACCESSIBLE
  void			Client::setTimeout(Time const & t, IClientCallable *call, bool exact)
  {
    _timeTarget = t;
    _timeElapsed = Time::Zero;
    _timeExact = exact;
    _funcTime = NULL;
    _callTime = call;
  }

  VRISKA_ACCESSIBLE
  void			Client::unsetTimeout()
  {
    _funcTime = NULL;
    _callTime = NULL;
  }

  VRISKA_ACCESSIBLE
  Time const &	Client::getElapsedTime() const
  {
    return (_timeElapsed);
  }

  VRISKA_ACCESSIBLE
  int				Client::read(void *buffer, size_t size)
  {
    if (!_isBlocking)
      return (SimpleClient::read(buffer, size));
    else
      {
        char          *cbuff = static_cast<char *>(buffer);
        int           ret;
        size_t        idx = 0;

        do
          {
            ret = recv(&cbuff[idx], size - idx, true);
            if (ret > 0)
              idx += ret;
          } while (ret > 0 && idx < size);
        return (ret > 0 ? static_cast<int>(idx) : Socket::Error);
      }
  }

  VRISKA_ACCESSIBLE
  int 			Client::read(std::string& buffer)
  {
    if (!_isBlocking)
      return (SimpleClient::read(buffer));
    else
      return (read(buffer, buffer.size()));
  }

  VRISKA_ACCESSIBLE
  int 			Client::read(std::string& buffer, size_t size)
  {
    if (!_isBlocking)
      return (SimpleClient::read(buffer, size));
    else
      {
        int		ret;

        buffer.resize(size, 0);
        memset(&buffer[0], 0, size);
        ret = read(&buffer[0], size);
        if (ret > -1)
          buffer.resize(ret, 0);
        return (ret);
      }
  }

  VRISKA_ACCESSIBLE
  int		Client::falseRead(std::string& buffer, size_t size, unsigned int offset)
  {
    if (!_isBlocking)
      return (SimpleClient::falseRead(buffer, size, offset));
    else
      return (0);
  }

  VRISKA_ACCESSIBLE
  int		Client::falseRead(void *buffer, size_t size, unsigned int offset)
  {
    if (!_isBlocking)
      return (SimpleClient::falseRead(buffer, size, offset));
    else
      return (0);
  }

  VRISKA_ACCESSIBLE
  void		Client::shiftRead(unsigned int size)
  {
    if (!_isBlocking)
      SimpleClient::shiftRead(size);
  }

  VRISKA_ACCESSIBLE
  int 			Client::readUntil(std::string& buffer, std::string const & delim)
  {
    if (!_isBlocking)
      return (SimpleClient::readUntil(buffer, delim));
    else
      return (read(buffer));
  }

  VRISKA_ACCESSIBLE
  int 			Client::readLine(std::string& buffer)
  {
    if (!_isBlocking)
      return (SimpleClient::readLine(buffer));
    else
      return (readUntil(buffer, "\n"));
  }

  VRISKA_ACCESSIBLE
  unsigned int 	Client::sizeToRead() const
  {
    if (!_isBlocking)
      return (SimpleClient::sizeToRead());
    else
      return (0);
  }

  VRISKA_ACCESSIBLE
  unsigned int 	Client::sizeCanRead() const
  {
    if (!_isBlocking)
      return (SimpleClient::sizeCanRead());
    else
      return (0);
  }

  VRISKA_ACCESSIBLE
  int 			Client::write(char const *buffer)
  {
    if (!_isBlocking)
      return (SimpleClient::write(buffer));
    else
      return (write(buffer, strlen(buffer)));
  }

  VRISKA_ACCESSIBLE
  int 			Client::write(void const *buffer, size_t size)
  {
    if (!_isBlocking)
      return (SimpleClient::write(buffer, size));
    else
      {
        char const      *cbuff = static_cast<char const *>(buffer);
        int             ret;
        size_t          idx = 0;

        do
          {
            ret = send(&cbuff[idx], size - idx, true);
            if (ret > 0)
              idx += ret;
          } while (ret > 0 && idx < size);
        return (ret > 0 ? static_cast<int>(idx) : Socket::Error);
      }
  }

  VRISKA_ACCESSIBLE
  int 			Client::write(std::string const & buffer)
  {
    if (!_isBlocking)
      return (SimpleClient::write(buffer));
    else
      return (write(buffer.c_str(), buffer.size()));
  }

  VRISKA_ACCESSIBLE
  int 			Client::write(std::string const & buffer, size_t size)
  {
    if (!_isBlocking)
      return (SimpleClient::write(buffer, size));
    else
      return (write(buffer.c_str(), size));
  }

  VRISKA_ACCESSIBLE
  unsigned int 	Client::sizeToWrite() const
  {
    if (!_isBlocking)
      return (SimpleClient::sizeToWrite());
    else
      return (0);
  }

  VRISKA_ACCESSIBLE
  unsigned int 	Client::sizeCanWrite() const
  {
    if (!_isBlocking)
      return (SimpleClient::sizeCanWrite());
    else
      return (0);
  }

  bool			Client::hasTimeout() const
  {
    return (_funcTime != NULL || _callTime != NULL);
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
    return (_funcStdin != NULL || _callStdin != NULL);
  }

  bool			Client::callbackRead()
  {
    bool		ret = true;

    if (_funcRead != NULL)
      ret = (*_funcRead)(*this);
    else if (_callRead != NULL)
      ret = (*_callRead)(*this);
    if (!ret)
      disconnect();
    return (ret);
  }

  bool			Client::callbackWrite()
  {
    bool		ret = true;

    if (_funcWrite != NULL)
      ret = (*_funcWrite)(*this);
    else if (_callWrite != NULL)
      ret = (*_callWrite)(*this);
    if (!ret)
      disconnect();
    return (ret);
  }

  bool			Client::callbackStdin()
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

  bool			Client::callbackTimeout()
  {
    bool		ret = true;

    if (_funcTime != NULL)
      ret = (*_funcTime)(*this);
    else if (_callTime != NULL)
      ret = (*_callTime)(*this);
    _timeElapsed = Time::Zero;
    if (!ret)
      disconnect();
    return (ret);
  }
}
