
#ifndef VRISKA_LIB_NETWORK_SERVER_H_
# define VRISKA_LIB_NETWORK_SERVER_H_

# include <Vriska/Core/Data.hpp>
# include <Vriska/Network/SocketServer.h>
# include <Vriska/Network/SimpleClient.h>
# include <Vriska/Threads/Thread.h>

# include <iostream>
# include <list>
# include <algorithm>

namespace Vriska
{
  // Forward declaration of IServerCallable
  class IServerCallable;
  // Forward declaration of IServerStdinWatcher
  class IServerStdinWatcher;
  // Forward declaration of IServerTimeoutable
  class IServerTimeoutable;

  class VRISKA_EXPORT Server: public SocketServer, public Datable, public IRunnable
  {
  public:
    class VRISKA_EXPORT Client : public SimpleClient
    {
        friend class Server;

    public:
      Client(INativeSocket& sysSocket, Server& server, unsigned int n);
      ~Client();

    private:
      Client(Client const & other);
      Client const & operator=(Client const & other);
      
    public:
      unsigned int	getIndex() const;

    private:
      void			    destroy();
      Error::Code		sync(bool send);

      int			        writeOnBuffer(char const *buffer, size_t size);
      INativeSocket const &	getNativeSocket() const;
      
    private:
      Server&		_server;
      unsigned int	_n;
        
    protected:
      using SimpleClient::sync;
    };

  private:
    friend class Client;
    
  public:
    typedef bool	(*FunctionC)(Server&, Client&);
    typedef bool	(*Function)(Server&);
    
    typedef std::list<Client *>::iterator	Iter;
    typedef std::list<Client *>::const_iterator	CIter;
    
  public:
    Server(Socket::Protocol protocol = Socket::TCP);
    virtual ~Server();
    
  private:
    Server(Server const & other);
    Server const & operator=(Server const & other);
    
  public:
    Error::Code		connect(unsigned int port = Socket::FreePort, std::string const & host = "", bool force = false);
    Error::Code		connect(unsigned int const *ports, std::string const & host = "", bool force = false);
    Error::Code		connect(std::list<unsigned int> const & ports, std::string const & host = "", bool force = false);
    Error::Code		connect(unsigned int port1, unsigned int port2, std::string const & host = "", bool force = false);
    Error::Code		reconnect();
    Error::Code		disconnect();
    Error::Code		launch();
    void            runThread();
    void            stopThread();
   
    void			setLimit(unsigned int limit);
    
    size_t    broadcast(void const * buffer, size_t size);

    template <class Func>
    size_t          broadcast(void const * buffer, size_t size, Func func)
    {
      size_t        i = 0;

      sysLog("Broadcast: " + StringUtils::escape(std::string(static_cast<char const *>(buffer), size)));
      for (Iter it = _clients.begin(); it != _clients.end(); ++it)
        if (func(*this, *(*it)))
          if ((*it)->write(buffer, size) == static_cast<int>(size))
            ++i;
      return (i);
    }
    
    template <class Func>
    void			forAll(Func func)
    {
      for (Iter it = _clients.begin(); it != _clients.end(); ++it)
        func(*this, *(*it));
    }

  public:
      void  enableLogging(bool logging = true);
      void  enableSysLogging(bool sysLogging = true);
      void  setLoggingStream(std::ostream& os);
      void  setLoggingTag(std::string const & tag);

  public:
    void			registerCallbacks(IServerCallable* callbacks);
    void			unregisterCallbacks();
    void			registerStdinWatcher(IServerStdinWatcher *stdinWatcher);
    void			unregisterStdinWatcher();

    void			setTimeout(Time const & t, IServerTimeoutable *timeout, bool exact = true);
    void			unsetTimeout();
    Time const &	getElapsedTime() const;

    void            run();
    
    unsigned int	getPort() const;
    
  private:
    void			disconnectMe(Client* client);

    bool			hasTimeout() const;
    Time			getTimeout() const;
    bool			watchStdin() const;

    void			addTimeout(Time const & elapsed);
    bool			callbackTimeout();
    
    bool			callbackConnect(Client& client);
    bool			callbackDisconnect(Client& client);
    bool			callbackReceive(Client& client);
    bool			callbackSend(Client& client);
    bool			callbackStdin();
    
    Client			*addNewClient(INativeSocket *sock);
    Client			*getFromSocket(INativeSocket const *sock);
    Error::Code	    manageIO();
    void			clearClients();
    void			manageClients(SocketSet& set, bool send);    
  
    static bool			yesMan(Server& server, Client& client);
    
  private:
    std::list<Client *>	_clients;
    unsigned int		_n;
    unsigned int		_limit;
    
    bool			_tried;
    std::string		_host;
    unsigned int	_port;
    
    Time			_timeTarget;
    Time			_timeElapsed;
    bool			_timeExact;
    
    IServerCallable     *_callbacks;
    IServerStdinWatcher *_stdinWatcher;
    IServerTimeoutable  *_timeout;

    Thread          _thd;
  };
}

#endif // !VRISKA_LIB_NETWORK_SERVER_H_
