
#ifndef VRISKA_LIB_NETWORK_CLIENT_H_
# define VRISKA_LIB_NETWORK_CLIENT_H_

# include <Vriska/Core/Time.h>
# include <Vriska/Network/SimpleClient.h>
# include <Vriska/Network/Loggers.h>
# include <Vriska/Network/ICallable.h>

namespace Vriska
{
  class VRISKA_EXPORT Client : public SimpleClient
  {
  public:
    typedef bool	(*Function)(Client&);
    
  public:
    Client(Socket::Protocol protocol = Socket::TCP);
    virtual ~Client();
    
  private:
    Client(Socket const & other);
    Socket const & operator=(Socket const & other);
    
  public:
    Error::Code		connect(std::string const & host, unsigned int port, bool force = false);
    Error::Code		disconnect();
    Error::Code		reconnect();
    
    void			log(std::string const & info);
    void			setLogging(bool val, std::ostream& os = std::cerr);
    void			setBlocking(bool val);
    Error::Code		launch();
    Error::Code		waitUntilWritten(bool callBack = true, bool timeOut = true);
    Error::Code		waitUntilDelim(std::string const & delim, bool callBack = true, bool timeOut = true);
    Error::Code		waitUntilSize(size_t size, bool callBack = true, bool timeOut = true);
    Error::Code		waitLine(bool callBack = true, bool timeOut = true);
    Error::Code		waitFor(Time const & t, bool callBack = true, bool timeOut = false);
    
    void			registerOnRead(Function func);
    void			registerOnRead(IClientCallable *call);
    void			unregisterOnRead();
    void			registerOnWrite(Function func);
    void			registerOnWrite(IClientCallable *call);
    void			unregisterOnWrite();
    void			registerOnStdin(Function func);
    void			registerOnStdin(IClientCallable *call);
    void			unregisterOnStdin();
    
    void			setTimeout(Time const & t, Function func, bool exact = true);
    void			setTimeout(Time const & t, IClientCallable *call, bool exact = true);
    void			unsetTimeout();
    Time const &	getElapsedTime() const;

  public:
    int				read(void *buffer, size_t size);
    int 			read(std::string& buffer);
    int 			read(std::string& buffer, size_t size);
    int				falseRead(std::string& buffer, unsigned int offset = 0);
    int				falseRead(std::string& buffer, size_t size, unsigned int offset = 0);
    int				falseRead(void *buffer, size_t size, unsigned int offset = 0);
    void			shiftRead(unsigned int size);
    int 			readUntil(std::string& buffer, std::string const & delim);
    int 			readLine(std::string& buffer);
    unsigned int 	sizeToRead() const;
    unsigned int 	sizeCanRead() const;
    
  public:
    int 			write(char const *buffer);
    int 			write(void const *buffer, size_t size);
    int 			write(std::string const & buffer);
    int 			write(std::string const & buffer, size_t size);
    unsigned int 	sizeToWrite() const;
    unsigned int 	sizeCanWrite() const;
    
  protected:
    virtual	void	sysLog(std::string const & info);
    
  private:
    bool			hasTimeout() const;
    Time			getTimeout() const;
    bool			watchStdin() const;

    void			addTimeout(Time const & elapsed);
    void			addTime(Time const & elapsed);
    bool			callbackTimeout();
    
    bool			callbackRead();
    bool			callbackWrite();
    bool			callbackStdin();
    
    Error::Code			manageIO(bool callBack, bool timeOut);
    
  private:
    Logger			_logger;
    bool			_isBlocking;
    
    bool			_tried;
    std::string			_host;
    unsigned int		_port;
    
    Time			_timeTarget;
    Time			_timeElapsed;
    bool			_timeExact;
    
    Time			_waitTarget;
    Time			_waitElapsed;
    
    IClientCallable	*_callRead;
    Function		_funcRead;
    IClientCallable	*_callWrite;
    Function		_funcWrite;
    IClientCallable	*_callStdin;
    Function		_funcStdin;
    IClientCallable	*_callTime;
    Function		_funcTime;
  };
}

#endif // !VRISKA_LIB_NETWORK_CLIENT_H_
