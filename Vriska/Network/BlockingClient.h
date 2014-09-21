
#ifndef VRISKA_LIB_NETWORK_BLOCKINGCLIENT_H_
# define VRISKA_LIB_NETWORK_BLOCKINGCLIENT_H_

# include <Vriska/Network/Client.h>

namespace Vriska
{
  class VRISKA_EXPORT BlockingClient : public Client
  {
  public:
    BlockingClient(Socket::Protocol protocol = Socket::TCP);
    virtual ~BlockingClient();

  public:
    void			setBlocking(bool val);

  public:
    int				read(void *buffer, size_t size);
    int 			read(std::string& buffer);
    int 			read(std::string& buffer, size_t size);
    int				peek(std::string& buffer, size_t size, unsigned int offset = 0);
    int				peek(void *buffer, size_t size, unsigned int offset = 0);
    void			seek(unsigned int size);
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

  private:
    bool    _isBlocking;
  };
}

#endif // !VRISKA_LIB_NETWORK_BLOCKINGCLIENT_H_

