
#include <Vriska/Network/Logger.h>

namespace Vriska
{
  Logger::Logger(bool logging, bool sysLogging) : _logging(logging), _sysLogging(sysLogging), _os(&std::cerr)
  {
  }

  Logger::Logger(std::string const & tag, bool sysLogging) : _logging(true), _sysLogging(sysLogging), _tag(tag), _os(&std::cerr)
  {
  }

  Logger::~Logger()
  {
  }

  Logger::Logger(Logger const & other)
  {
    ScopedLock	lock(other._mutexLogging);
    
    _logging = other._logging;
    _sysLogging = other._sysLogging;
    _tag = other._tag;
    _os = other._os;
  }

  Logger const &	Logger::operator=(Logger const & other)
  {
    ScopedLock	lock(other._mutexLogging);
    ScopedLock	lock2(_mutexLogging);

    if (this != &other)
      {
        _logging = other._logging;
        _sysLogging = other._sysLogging;
        _tag = other._tag;
        _os = other._os;
      }
    return (*this);
  }

  void	Logger::sysLog(std::string const & info)
  {
    ScopedLock	lock(_mutexLogging);

    if (_sysLogging)
      (*_os) << getHeader() << "[Vriska] " << info << std::endl;
  }
  
  VRISKA_ACCESSIBLE
  void	Logger::log(std::string const & info)
  {
    ScopedLock	lock(_mutexLogging);

    if (_logging)
      (*_os) << getHeader() << " " << info << std::endl;
  }
  
  VRISKA_ACCESSIBLE
    void	Logger::enableLogging(bool logging)
    {
        ScopedLock	lock(_mutexLogging);

        _logging = logging;
    }

    VRISKA_ACCESSIBLE
    bool	Logger::isLogging() const
    {
        ScopedLock	lock(_mutexLogging);

        return (_logging);
    }
  
  VRISKA_ACCESSIBLE
    void    Logger::enableSysLogging(bool sysLogging)
    {
        _sysLogging = sysLogging;
    }
    
  VRISKA_ACCESSIBLE
    bool    Logger::isSysLogging() const
    {
        return (_sysLogging);
    }
    
  VRISKA_ACCESSIBLE
    void                Logger::setLoggingTag(std::string const & tag)
    {
        _tag = tag;
    }
    
  VRISKA_ACCESSIBLE
    std::string const & Logger::getLoggingTag() const
    {
        return (_tag);
    }
    
  VRISKA_ACCESSIBLE
  void	Logger::setLoggingStream(std::ostream& os)
  {
    ScopedLock	lock(_mutexLogging);

    _os = &os;
  }

  VRISKA_ACCESSIBLE
  std::ostream&	Logger::getLoggingStream()
  {
    ScopedLock	lock(_mutexLogging);

    return (*_os);
  }

  std::string const	Logger::getHeader() const
  {
    std::stringstream	oss;
    char		buffer[SizeBufferDate];
    time_t		t;
	struct tm	tm;

    t = time(NULL);
    memset(buffer, 0, SizeBufferDate);
#ifdef VRISKA_WINDOWS
	localtime_s(&tm, &t);
#else // !VRISKA_WINDOWS
	localtime_r(&t, &tm);
#endif // !VRISKA_WINDOWS
    strftime(buffer, SizeBufferDate, "%d/%m/%y %H:%M:%S", &tm);
    oss << "[" << buffer << "]";
    if (!_tag.empty())
        oss << "[" << _tag << "]";
    return (oss.str());
  }
}
