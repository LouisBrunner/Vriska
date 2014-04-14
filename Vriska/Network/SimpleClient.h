
#ifndef VRISKA_LIB_NETWORK_SIMPLECLIENT_H_
# define VRISKA_LIB_NETWORK_SIMPLECLIENT_H_

# include <Vriska/Core/Data.hpp>
# include <Vriska/Core/Exceptions.hpp>
# include <Vriska/Network/SocketClient.h>
# include <Vriska/Network/BufferRing.h>

namespace Vriska
{
  class SimpleClient : public SocketClient, public Datable
  {
  public:
    static const unsigned int	SizeBuffer = 4096;
    
  public:
    SimpleClient();
    SimpleClient(INativeSocket& sysSocket);
    virtual ~SimpleClient();

  private:
    SimpleClient(Socket const & other);
    Socket const & operator=(Socket const & other);

  public:
    virtual int			read(void *buffer, size_t size);
    virtual int			read(std::string& buffer);
    virtual int			read(std::string& buffer, size_t size);
    virtual int			falseRead(std::string& buffer, size_t size, unsigned int offset = 0);
    virtual int			falseRead(void *buffer, size_t size, unsigned int offset = 0);
    virtual void		shiftRead(unsigned int size);
    virtual int			readUntil(std::string& buffer, std::string const & delim);
    virtual int			readLine(std::string& buffer);
    virtual unsigned int 	sizeToRead() const;
    virtual unsigned int 	sizeCanRead() const;
    
  public:
    virtual int			write(char const *buffer);
    virtual int			write(void const *buffer, size_t size);
    virtual int			write(std::string const & buffer);
    virtual int			write(std::string const & buffer, size_t size);
    virtual unsigned int 	sizeToWrite() const;
    virtual unsigned int 	sizeCanWrite() const;

  protected:
    Error::Code		doRead();
    Error::Code		doWrite();
    
  protected:
    BufferRing		_brRead;
    BufferRing		_brWrite;
  };
}

#endif // !VRISKA_LIB_NETWORK_SIMPLECLIENT_H_
