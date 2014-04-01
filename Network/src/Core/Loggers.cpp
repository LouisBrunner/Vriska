
#include <Vriska/Network/Loggers.hh>

namespace Vriska
{
  Logger::Logger(bool logging) : _isLogging(logging), _os(&std::cerr)
  {
  }

  Logger::~Logger()
  {
  }

  Logger::Logger(Logger const & other)
  {
    ScopedLock	lock(other._mutex);
    
    _isLogging = other._isLogging;
    _os = other._os;
  }

  Logger const &	Logger::operator=(Logger const & other)
  {
    ScopedLock	lock(other._mutex);
    ScopedLock	lock2(_mutex);

    if (this != &other)
      {
	_isLogging = other._isLogging;
	_os = other._os;
      }
    return (*this);
  }

  void	Logger::log(std::string const & info)
  {
    ScopedLock	lock(_mutex);

    if (_isLogging)
      (*_os) << getHeader() << " " << info << std::endl;
  }

  void	Logger::userLog(std::string const & info)
  {
    ScopedLock	lock(_mutex);

    if (_isLogging)
      (*_os) << getHeader() << " User log: " << info << std::endl;
  }

  VRISKA_ACCESSIBLE
  bool	Logger::isLogging() const
  {
    ScopedLock	lock(_mutex);

    return (_isLogging);
  }

  void	Logger::enable(bool val)
  {
    ScopedLock	lock(_mutex);

    _isLogging = val;
  }

  void	Logger::setStream(std::ostream& os)
  {
    ScopedLock	lock(_mutex);

    _os = &os;
  }

  VRISKA_ACCESSIBLE
  std::ostream&	Logger::getStream()
  {
    ScopedLock	lock(_mutex);

    return (*_os);
  }

  std::string const	Logger::getHeader() const
  {
    std::stringstream	oss;
    char		buffer[SizeBufferDate];
    time_t		t;

    t = time(NULL);
    memset(buffer, 0, SizeBufferDate);
    strftime(buffer, SizeBufferDate, "%d/%m/%y %H:%M:%S", localtime(&t));
    oss << "[" << buffer << "]";
    return (oss.str());
  }

  LoggerClient::LoggerClient(unsigned int n, bool logging) : Logger(logging)
  {
    std::ostringstream	oss;

    oss << std::setfill('0') << std::setw(10) << n;
    _num = "[" + oss.str() + "]";
  }

  LoggerClient::~LoggerClient()
  {
  }

  LoggerClient::LoggerClient(LoggerClient const & other) : Logger(other)
  {
    _num = other._num;
  }

  LoggerClient const &	LoggerClient::operator=(LoggerClient const & other)
  {
    if (this != &other)
      {
	Logger::operator=(other);
	{
	  ScopedLock	lock(_mutex);

	  _num = other._num;
	}
      }
    return (*this);
  }

  std::string const	LoggerClient::getHeader() const
  {
    return (Logger::getHeader() + _num);
  }

  LoggerServer::LoggerServer(bool logging) : Logger(logging)
  {
  }

  LoggerServer::~LoggerServer()
  {
  }

  std::string const	LoggerServer::getHeader() const
  {
    return (Logger::getHeader() + "[Server]");
  }

  LoggerServer::LoggerServer(LoggerServer const & other) : Logger(other)
  {
  }

  LoggerServer const &	LoggerServer::operator=(LoggerServer const & other)
  {
    if (this != &other)
      Logger::operator=(other);
    return (*this);
  }
}
