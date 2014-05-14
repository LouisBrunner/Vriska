
#include <Vriska/Threads.h>
#include <Vriska/Threads.h>

#include <Vriska/Network.h>
#include <Vriska/Network.h>

#include <iostream>

class Handler : public Vriska::IServerCallable, public Vriska::IServerStdinWatcher
{
public:
    bool	onConnect(Vriska::Server& server, Vriska::Server::Client& client)
    {
	    static_cast<void>(server);
	    std::cout << "CONNECT " << client.getIndex() << std::endl;
	    return (true);
    }

    bool	onDisconnect(Vriska::Server& server, Vriska::Server::Client& client)
    {
	    static_cast<void>(server);
	    std::cout << "DISCONNECT " << client.getIndex() << std::endl;
	    return (true);
    }

    bool		onStdin(Vriska::Server& server)
    {
	    std::string	line;

	    std::cout << "STDIN" << std::endl;
	    if (!getline(std::cin, line))
		    return (false);
	    line += "\n";
	    server.broadcast(line.c_str(), line.size());
	    return (true);
    }

    bool		onReceive(Vriska::Server& server, Vriska::Server::Client& client)
    {
	    std::string	line;

	    static_cast<void>(server);
	    while (client.readLine(line) != Vriska::BufferRing::Error)
		    std::cout << "CLIENT " << client.getIndex() << ": " << line << std::endl;
	    return (true);
    }
};

int		main(int ac, char **av)
{
    Handler handler;
	Vriska::Server	server;

	static_cast<void>(av);
	server.setProtocol(ac > 1 ? Vriska::Socket::UDP : Vriska::Socket::TCP);
	server.enableLogging(true);
	server.enableSysLogging(true);
	server.registerCallbacks(&handler);
	server.registerStdinWatcher(&handler);
	server.connect(8888);
	server.launch();
	return (0);
}
