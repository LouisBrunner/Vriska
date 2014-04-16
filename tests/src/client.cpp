
#include <Vriska/Network.h>
#include <Vriska/Network.h>

#include <Vriska/Threads.h>
#include <Vriska/Threads.h>

#define TEST_ERR(f, s) do { Vriska::Error::Code e = f; if (e != Vriska::Error::NoError) { std::cout << "Error at '" << s << "': " << Vriska::Error::getMessage(e) << std::endl; return (1); } } while(0);

#include <iostream>
#include <fstream>
#include <cstdlib>
#ifdef VRISKA_LINUX
# include <unistd.h>
#else // !VRISKA_LINUX
# include <Windows.h>
#endif // !VRISKA_LINUX

static void		printGeneral(Vriska::Client const & client)
{
	std::cout << "#General" << std::endl;
	std::cout << "Connected: " << client.isConnected() << std::endl;
	std::cout << "Target: " << client.getIPAddress() << std::endl;
	std::cout << "Connected as: " << client.getIPConnectedOn() << std::endl;
}

static void		printBuffers(Vriska::Client const & client)
{
	std::cout << "#Buffers" << std::endl;
	std::cout << "Size to read: " << client.sizeToRead() << std::endl;
	std::cout << "Size can read: " << client.sizeCanRead() << std::endl;
	std::cout << "Size to write: " << client.sizeToWrite() << std::endl;
	std::cout << "Size can write: " << client.sizeCanWrite() << std::endl;
	std::cout << "Elapsed time: " << client.getElapsedTime().toString() << std::endl;
}

static void				printTime()
{
	char				buffer[255];
	time_t				t;

	t = time(NULL);
	memset(buffer, 0, 255);
	strftime(buffer, 255, "%d/%m/%y %H:%M:%S", localtime(&t));
	std::cout << "[" << buffer << "]" << std::endl;
}

static int	testConnection(Vriska::Client & client, std::string const & host, unsigned int port)
{
	std::cout << "## Test connection" << std::endl;
	printGeneral(client);
	TEST_ERR(client.connect(host, port), "connect1");
	printGeneral(client);
	TEST_ERR(client.disconnect(), "disconnect1");
	printGeneral(client);
	TEST_ERR(client.reconnect(), "reconnect1");
	printGeneral(client);
	TEST_ERR(client.reconnect(), "reconnect2");
	printGeneral(client);
	TEST_ERR(client.disconnect(), "disconnect2");
	printGeneral(client);
	return (0);
}

static int	usage(std::string const & bin)
{
	std::cout << "Usage: " << bin << " [options]" << std::endl;
	std::cout << " Options:" << std::endl;
	std::cout << "  --help	display this message" << std::endl;
	std::cout << std::endl;
	std::cout << "  -h HOST	change host from localhost to HOST" << std::endl;
	std::cout << "  -p PORT	change port from 8888 to PORT" << std::endl;
	std::cout << "  -r PROTO	change protocol from TCP to PROTO" << std::endl;
	std::cout << "  -b		toggle the transmission blocking state" << std::endl;
	std::cout << "  -l		toggle the logging" << std::endl;
	std::cout << std::endl;
	std::cout << "  --conn	toggle the connection testing" << std::endl;
	std::cout << "  --callback	toggle the callback testing" << std::endl;
	std::cout << "  --notime	toggle the testing without timeout" << std::endl;
	std::cout << "  --notexact	toggle the testing with timeout not exact" << std::endl;
	std::cout << "  --every	toggle the testing with everything" << std::endl;
	std::cout << "  --nc		launch a netcat-like program" << std::endl;
	std::cout << "  --custom	custom file transfer program" << std::endl;
	std::cout << "  --threads	test threading" << std::endl;
	std::cout << "  --threads2	test threading 2" << std::endl;
	exit(1);
	return (1);
}

static int	parsing(int ac, char **av,
					std::string& host, unsigned int& port, std::string& protocol,
					bool& blocking, bool& logging,
					bool& testConn, bool& testCB, bool& testNoTime, bool& testTimeNE, bool& testET, bool& nc, bool& custom, bool &threads, bool &threads2)
{
	int			i = 0;
	std::string	arg;

	while (++i < ac)
	{
		arg = av[i];
		if (arg == "-h")
			if (i + 1 < ac)
				host = av[++i];
			else
				return (usage(av[0]));
		else if (arg == "-p")
			if (i + 1 < ac)
				port = atoi(av[++i]);
			else
				return (usage(av[0]));
		else if (arg == "-r")
			if (i + 1 < ac)
				protocol = av[++i];
			else
				return (usage(av[0]));
		else if (arg == "-b")
			blocking = !blocking;
		else if (arg == "-l")
			logging = !logging;
		else if (arg == "--conn")
			testConn = !testConn;
		else if (arg == "--callback")
			testCB = !testCB;
		else if (arg == "--notime")
			testNoTime = !testNoTime;
		else if (arg == "--notexact")
			testTimeNE = !testTimeNE;
		else if (arg == "--every")
			testET = !testET;
		else if (arg == "--nc")
			nc = !nc;
		else if (arg == "--custom")
			custom = !custom;
		else if (arg == "--threads")
			threads = !threads;
		else if (arg == "--threads2")
			threads2 = !threads2;
		else
			return (usage(av[0]));
		}
	return (0);
}

static int		testNoCallback(Vriska::Client& client, std::string const & host, unsigned int port)
{
	std::string	line;

	std::cout << "## Test NoCallback" << std::endl;
	TEST_ERR(client.connect(host, port), "connect");
	printGeneral(client);
	printBuffers(client);
	client.write("hello\n");
	printBuffers(client);
	client.waitUntilWritten();
	printBuffers(client);
	Vriska::Error::Code err = client.waitLine();
	if (err != Vriska::Error::NoError)
		std::cout << Vriska::Error::getMessage(err) << std::endl;
	printBuffers(client);
	while (client.readLine(line) != Vriska::BufferRing::Error)
	{
		client.log("Received: " + line);
		printBuffers(client);
	}
	client.write("how are you\n");
	printBuffers(client);
	client.waitLine();
	printBuffers(client);
	client.readLine(line);
	std::cout << "Received: " << line << std::endl;
	printBuffers(client);
	printGeneral(client);
	TEST_ERR(client.disconnect(), "disconnect");
	return (0);
}

static bool		stdinFunc(Vriska::Client& client)
{
	std::string	line;

	std::cout << "(): stdin callback (Data: " << client.getData<int&>() << ")" << std::endl;
	printBuffers(client);
	if (!getline(std::cin, line))
		return (false);
	++client.getData<int&>();
	client.write(line + "\n");
	printBuffers(client);
	return (true);
}

static bool		readFunc(Vriska::Client& client)
{
	std::string	req;
	std::string	line;

	std::cout << "(): read callback" << std::endl;
	printBuffers(client);
	while (client.readUntil(line, "\n") != Vriska::BufferRing::Error)
	{
		if (line == "fine and you ?")
			req = "fine\n";
		else if (line == "hi")
			req = "how are you ?\n";
		else if (line == "get out")
		{
			std::cout << "I quit." << std::endl;
			printBuffers(client);
			return (false);
		}
		else
			req = "what ?\n";
		std::cout << "Received: " << line << " | Sending: " << req;
		client.write(req);
		printBuffers(client);
	}
	return (true);
}

static bool		writeFunc(Vriska::Client& client)
{
	std::cout << "(): write callback" << std::endl;
	printBuffers(client);
	return (true);
}

static int	testNoTimeout(Vriska::Client& client, std::string const & host, unsigned int port)
{
	int 	val = 5;

	client.setData<int&>(val);
	std::cout << "## Test NoTimeout" << std::endl;
	std::cout << "(Data: " << client.getData<int>() << ")" << std::endl;
	client.registerOnRead(&readFunc);
	client.registerOnWrite(&writeFunc);
	client.registerOnStdin(&stdinFunc);
	TEST_ERR(client.connect(host, port), "connect");
	printGeneral(client);
	client.write("hello\n");
	client.launch();
	printGeneral(client);
	TEST_ERR(client.disconnect(), "disconnect");
	return (0);
}

class Info
{
	public:
		Info(Vriska::Time const & time, bool exact) : _time(time), _exact(exact) {}

		Vriska::Time const &	getTime() const
		{
			return (_time);
		}

		bool					getExact() const
		{
			return (_exact);
		}

	private:
		Vriska::Time	_time;
		bool			_exact;
};

static bool			timeFunc(Vriska::Client& client)
{
	Info&			info = client.getData<Info&>();

	static_cast<void>(info);
	std::cout << "(): timeout callback" << std::endl;
	std::cout << "Elapsed: " << client.getElapsedTime() << std::endl;
	printTime();
	return (true);
}

static int			testEverything(Vriska::Client& client, std::string const & host, unsigned int port, bool exact = false)
{
	Vriska::Time	time(3, 0);
	Info			info(time, exact);

	client.setData<Info&>(info);
	if (exact)
		std::cout << "## Test Everything" << std::endl;
	else
		std::cout << "## Test Time Not Exact" << std::endl;
	client.unregisterOnStdin();
	client.registerOnRead(&readFunc);
	client.registerOnWrite(&writeFunc);
	client.setTimeout(time, &timeFunc, exact);
	TEST_ERR(client.connect(host, port), "connect");
	printGeneral(client);
	client.write("hello\n");
	printTime();
	client.waitFor(Vriska::Time(2, 0));
	printTime();
	client.launch();
	printGeneral(client);
	TEST_ERR(client.disconnect(), "disconnect");
	return (0);
}

static bool			stdinNC(Vriska::Client& client)
{
	std::string	line;

	std::cin.clear();
	getline(std::cin, line);
	if (line.empty() && std::cin.eof())
		return (false);
	client.write(line + (std::cin.eof() ? "" : "\n"));
	return (true);
}

static bool		readNC(Vriska::Client& client)
{
	std::string	buffer;

	client.read(buffer);
	std::cout.write(buffer.c_str(), buffer.size());
	std::cout.flush();
	return (true);
}

static int	launchNC(Vriska::Client& client, std::string const & host, unsigned int port)
{
	client.registerOnStdin(&stdinNC);
	client.registerOnRead(&readNC);
	TEST_ERR(client.connect(host, port), "connect-nc");
	client.launch();
	return (0);
}

static void       sendFile(Vriska::Client& client, std::string const & path)
{
	std::string     line;
	std::streamsize ret;
	size_t          size = 50000;
	std::ifstream   ifs;

	std::cout << "sending " << path << " ..." << std::endl;
	ifs.open(path.c_str());
	if (ifs.is_open())
	{
		line.resize(size, 0);
		do
		{
			ifs.read(&line[0], size);
			ret = ifs.gcount();
			if (ret > 0)
				if (client.write(&line[0], static_cast<size_t>(ret)) != ret)
					break;
		} while (ifs.good() && !ifs.eof() && size == ret);
	}
	std::cout << "complete" << std::endl;
}

static bool		stdinCustom(Vriska::Client& client)
{
	std::string	line;

	if (!getline(std::cin, line))
		return (false);
	if (line == "verybig")
		sendFile(client, "/etc/passwd.bk");
	else if (line == "big")
		sendFile(client, "/var/log/wtmp");
	else
		std::cout << "nope" << std::endl;
	return (true);
}

static int	launchCustom(Vriska::Client& client, std::string const & host, unsigned int port)
{
	client.registerOnStdin(&stdinCustom);
	TEST_ERR(client.connect(host, port), "connect-custom");
	client.launch();
	return (0);
}

class MyRun : public Vriska::IRunnable
{
public:
  MyRun(unsigned int n, Vriska::Client& client) : _n(n), _client(client) {}

  void				run()
  {
    std::string		num = Vriska::StringUtils::toString<unsigned int>(_n);

    _client.log("Starting Thread " + num);
    for (unsigned int i = 0; i < 100000; ++i)
      _client.write(std::string("Hello " + Vriska::StringUtils::toString<unsigned int>(i) + " ! (Thread: " + num + ")\n"));
  }

private:
  unsigned int	_n;
  Vriska::Client&	_client;
};

static int				launchThreads(Vriska::Client& client, std::string const & host, unsigned int port)
{
  unsigned int		threads = 10;
  Vriska::ThreadPool	pool(threads);

  TEST_ERR(client.connect(host, port), "connect-threads");
  for (unsigned int i = 0; i < threads; ++i)
    pool.execute(new MyRun(i, client));
  client.launch();
  return (0);
}

class MyRun2 : public Vriska::IRunnable
{
public:
  MyRun2(Vriska::Client& client) : _client(client) {}

  void				run()
  {
    _client.log("Starting Thread");
    for (int i = 10; i >= 0; --i)
      {
	_client.write(std::string(Vriska::StringUtils::toString<int>(i)+ "\n"));
#ifdef VRISKA_LINUX
	sleep(1);
#else // !VRISKA_LINUX
	Sleep(1);
#endif // !VRISKA_LINUX
      }
  }

private:
  unsigned int	_n;
  Vriska::Client&	_client;
};

static bool lolz(Vriska::Client& client)
{
  printBuffers(client);
  return (true);
}

static int				launchThreads2(Vriska::Client& client, std::string const & host, unsigned int port)
{
  Vriska::Thread			t1;
  Vriska::Thread			t2;
  Vriska::Thread			t3;
  Vriska::IRunnable*			run = new MyRun2(client);

  TEST_ERR(client.connect(host, port), "connect-threads2");
  t1.launch(*run);
  t2.launch(*run);
  t3.launch(*run);
  client.setTimeout(Vriska::Time::fromSMilli(500), &lolz);
  client.launch();
  return (0);
}

static int	test(int ac, char **av)
{
	std::string		host = "localhost";
	unsigned int	port = 8888;
	std::string		protocol = "TCP";
	bool			blocking = false;
	bool			logging = true;
	bool			testConn = false;
	bool			testNoCB = false;
	bool			testNoTime = false;
	bool			testTimeNE = false;
	bool			testET = false;
	bool			nc = false;
	bool			custom = false;
	bool			threads = false;
	bool			threads2 = false;

	if (parsing(ac, av, host, port, protocol, blocking, logging, testConn, testNoCB, testNoTime, testTimeNE, testET, nc, custom, threads, threads2))
		return (1);

	Vriska::Client	client;
	
	if (protocol == "UDP")
	  client.setProtocol(Vriska::Socket::UDP);

	if (testConn)
		if (testConnection(client, host, port))
			return (1);

	client.setLogging(logging);

	if (blocking)
		client.setBlocking(blocking);

	if (testNoCB)
		if (testNoCallback(client, host, port))
			return (1);

	if (testNoTime)
		if (testNoTimeout(client, host, port))
			return (1);

	if (testTimeNE)
		if (testEverything(client, host, port))
			return (1);

	if (testET)
		if (testEverything(client, host, port, true))
			return (1);

	if (nc)
		if (launchNC(client, host, port))
			return (1);

	if (custom)
		if (launchCustom(client, host, port))
			return (1);

	if (threads)
		if (launchThreads(client, host, port))
			return (1);

	if (threads2)
		if (launchThreads2(client, host, port))
			return (1);

	return (0);
}

int		main(int ac, char **av)
{
	if (test(ac, av))
	{
		std::cout << "An error occured." << std::endl;
		return (1);
	}
	return (0);
}
