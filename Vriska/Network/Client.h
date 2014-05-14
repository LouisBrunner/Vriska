
#ifndef VRISKA_LIB_NETWORK_CLIENT_H_
# define VRISKA_LIB_NETWORK_CLIENT_H_

# include <Vriska/Core/Time.h>
# include <Vriska/Network/SimpleClient.h>
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
    
    Error::Code		launch();
    Error::Code		waitUntilSent(bool callBack = true, bool timeOut = true);
    Error::Code		waitUntilDelim(std::string const & delim, bool callBack = true, bool timeOut = true);
    Error::Code		waitUntilSize(size_t size, bool callBack = true, bool timeOut = true);
    Error::Code		waitLine(bool callBack = true, bool timeOut = true);
    Error::Code		waitFor(Time const & t, bool callBack = true, bool timeOut = false);
    
    void			registerCallbacks(IClientCallable* callbacks);
    void            unregisterCallbacks();
    void			registerStdinWatcher(IClientStdinWatcher* stdinWatcher);
    void            unregisterStdinWatcher();
    
    void			setTimeout(Time const & t, IClientTimeoutable* timeout, bool exact = true);
    void			unsetTimeout();
    Time const &	getElapsedTime() const;
    
  private:
    bool			hasTimeout() const;
    Time			getTimeout() const;
    bool			watchStdin() const;
    
    bool			callbackReceive();
    bool			callbackSend();
    bool			callbackStdin();

    void			addTimeout(Time const & elapsed);
    void			addTime(Time const & elapsed);
    bool			callbackTimeout();
    
    Error::Code			manageIO(bool callBack, bool timeOut);
    
  private:    
    bool			    _tried;
    std::string			_host;
    unsigned int		_port;
    
    Time			_timeTarget;
    Time			_timeElapsed;
    bool			_timeExact;
    
    Time			_waitTarget;
    Time			_waitElapsed;
    
    IClientCallable     *_callbacks;
    IClientStdinWatcher *_stdinWatcher;
    IClientTimeoutable	*_timeout;
  };
}

#endif // !VRISKA_LIB_NETWORK_CLIENT_H_
