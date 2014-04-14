
#ifndef VRISKA_LIB_NETWORK_LOGGERS_H_
# define VRISKA_LIB_NETWORK_LOGGERS_H_

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
  class Logger
  {
  private:
    static const unsigned int	SizeBufferDate = 255;

  public:
    Logger(bool logging = true);
    virtual ~Logger();

    Logger(Logger const & other);
    Logger const &	operator=(Logger const & other);

  public:
    void	log(std::string const & info);
    void	userLog(std::string const & info);

    bool		isLogging() const;
    std::ostream&	getStream();

    void	enable(bool val);
    void	setStream(std::ostream& os);

  protected:
    virtual std::string const		getHeader() const;

  protected:
    Mutex		_mutex;

  private:
    bool		_isLogging;
    std::ostream*	_os;
  };

  class LoggerClient : public Logger
  {
  public:
    LoggerClient(unsigned int n, bool logging = true);
    ~LoggerClient();

    LoggerClient(LoggerClient const & other);
    LoggerClient const &	operator=(LoggerClient const & other);

  protected:
    std::string const	getHeader() const;

  private:
    std::string		_num;
  };

  class LoggerServer : public Logger
  {
  public:
    LoggerServer(bool logging = true);
    ~LoggerServer();

    LoggerServer(LoggerServer const & other);
    LoggerServer const &	operator=(LoggerServer const & other);

  protected:
    std::string const	getHeader() const;
  };
}

#endif // !VRISKA_LIB_NETWORK_LOGGERS_H_
