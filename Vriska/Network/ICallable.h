
#ifndef VRISKA_LIB_NETWORK_ICALLABLE_H_
# define VRISKA_LIB_NETWORK_ICALLABLE_H_

# include <Vriska/Network/Server.h>
# include <Vriska/Core/OS.h>

namespace Vriska
{
	// Forward declaration of Client
	class Client;
    
	class IClientCallable
	{
		public:
			virtual ~IClientCallable() {}

        public:
            virtual bool    onReceive(Client& client) = 0;
            virtual bool    onSend(Client&)  { return true; }
	};

	class IServerCallable
	{
		public:
			virtual ~IServerCallable() {}

        public:
            virtual bool    onConnect(Server&, Server::Client&) { return true; }
            virtual bool    onReceive(Server& server, Server::Client& client) = 0;
            virtual bool    onSend(Server&, Server::Client&)  { return true; }
            virtual bool    onDisconnect(Server&, Server::Client&)  { return true; }
	};

    template<class Type>
    class INTimeoutable
    {
        public:
            virtual ~INTimeoutable() {}

        public:
            virtual bool    onTimeout(Type& type) = 0;
    };

    class IClientTimeoutable : public INTimeoutable<Client>
    {
        public:
            virtual ~IClientTimeoutable() {}
    };

    class IServerTimeoutable : public INTimeoutable<Server>
    {
        public:
            virtual ~IServerTimeoutable() {}
    };

    template<class Type>
    class IStdinWatcher
    {
        public:
            virtual ~IStdinWatcher() {}

        public:
            virtual bool    onStdin(Type& type) = 0;
    };

    class IClientStdinWatcher : public IStdinWatcher<Client>
    {
        public:
            virtual ~IClientStdinWatcher() {}
    };

    class IServerStdinWatcher : public IStdinWatcher<Server>
    {
        public:
            virtual ~IServerStdinWatcher() {}
    };
}

#endif // !VRISKA_LIB_NETWORK_ICALLABLE_H_
