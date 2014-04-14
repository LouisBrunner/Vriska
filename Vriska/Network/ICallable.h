
#ifndef VRISKA_LIB_NETWORK_ICALLABLE_H_
# define VRISKA_LIB_NETWORK_ICALLABLE_H_

# include <Vriska/Network/Server.h>

namespace Vriska
{
	// Forward declaration of Client
	class Client;

	template <class Type>
	class ICallable
	{
		public:
			virtual ~ICallable() {};

			virtual bool	operator()(Type& elem) = 0;
	};

	template <class Type1, class Type2>
	class ICallableD
	{
		public:
			virtual ~ICallableD() {};

			virtual bool	operator()(Type1& elem1, Type2& elem2) = 0;
	};

	class IClientCallable : public ICallable<Client>
	{
		public:
			virtual ~IClientCallable() {};
	};

	class IServerCallable : public ICallable<Server>
	{
		public:
			virtual ~IServerCallable() {};
	};

	class IServerCCallable : public ICallableD<Server, Server::Client>
	{
		public:
			virtual ~IServerCCallable() {};
	};
}

#endif // !VRISKA_LIB_NETWORK_ICALLABLE_H_
