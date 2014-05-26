
#ifndef VRISKA_LIB_NETWORK_LOGGER_H_
# define VRISKA_LIB_NETWORK_LOGGER_H_

# include <iostream>
# include <sstream>
# include <iomanip>

# include <ctime>
# include <cstring>

# include <Vriska/Core/OS.h>
# include <Vriska/Threads/ScopedLock.h>
# include <Vriska/Threads/Mutex.h>

namespace Vriska
{
  class VRISKA_EXPORT Logger
  {
  private:
    static const unsigned int	SizeBufferDate = 255;

  public:
    Logger(bool logging = true, bool sysLogging = true);
    Logger(std::string const & tag, bool sysLogging = true);
    virtual ~Logger();

    Logger(Logger const & other);
    Logger const &	operator=(Logger const & other);

  public:
    void	sysLog(std::string const & info);
    void	log(std::string const & info);

    void	enableLogging(bool logging = true);
    bool	isLogging() const;

    void    enableSysLogging(bool sysLogging = true);
    bool    isSysLogging() const;

    void                setLoggingTag(std::string const & tag);
    std::string const & getLoggingTag() const;

    void	        setLoggingStream(std::ostream& os);
    std::ostream&	getLoggingStream();

  private:
    virtual std::string const		getHeader() const;

  private:
    Mutex		_mutexLogging;
    bool		_logging;
    bool        _sysLogging;
    std::string _tag;
    std::ostream*	_os;
  };
}

#endif // !VRISKA_LIB_NETWORK_LOGGER_H_
