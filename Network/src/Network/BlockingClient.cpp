
#include <Vriska/Network/BlockingClient.h>

namespace Vriska
{
  VRISKA_ACCESSIBLE
  BlockingClient::BlockingClient(Socket::Protocol protocol) : Client(protocol), _isBlocking(false)
  {
    setLoggingTag("BlockingClient " + StringUtils::toString(this));
  }
  
  VRISKA_ACCESSIBLE
  BlockingClient::~BlockingClient()
  {
  }

  VRISKA_ACCESSIBLE
  void			BlockingClient::setBlocking(bool val)
  {
    if (_isBlocking != val)
      {
	_brRead.enable(val);
	_brWrite.enable(val);
      }
    _isBlocking = val;
  }
  
  VRISKA_ACCESSIBLE
  int				BlockingClient::read(void *buffer, size_t size)
  {
    if (!_isBlocking)
      return (SimpleClient::read(buffer, size));
    else
      {
        char          *cbuff = static_cast<char *>(buffer);
        int           ret;
        size_t        idx = 0;

        do
          {
            ret = recv(&cbuff[idx], size - idx, true);
            if (ret > 0)
              idx += ret;
          } while (ret > 0 && idx < size);
        return (ret > 0 ? static_cast<int>(idx) : Socket::Error);
      }
  }

  VRISKA_ACCESSIBLE
  int 			BlockingClient::read(std::string& buffer)
  {
    if (!_isBlocking)
      return (SimpleClient::read(buffer));
    else
      return (read(buffer, buffer.size()));
  }

  VRISKA_ACCESSIBLE
  int 			BlockingClient::read(std::string& buffer, size_t size)
  {
    if (!_isBlocking)
      return (SimpleClient::read(buffer, size));
    else
      {
        int		ret;

        buffer.resize(size, 0);
        memset(&buffer[0], 0, size);
        ret = read(&buffer[0], size);
        if (ret > -1)
          buffer.resize(ret, 0);
        return (ret);
      }
  }

  VRISKA_ACCESSIBLE
  int		BlockingClient::peek(std::string& buffer, size_t size, unsigned int offset)
  {
    if (!_isBlocking)
      return (SimpleClient::peek(buffer, size, offset));
    else
      return (0);
  }

  VRISKA_ACCESSIBLE
  int		BlockingClient::peek(void *buffer, size_t size, unsigned int offset)
  {
    if (!_isBlocking)
      return (SimpleClient::peek(buffer, size, offset));
    else
      return (0);
  }

  VRISKA_ACCESSIBLE
  void		BlockingClient::seek(unsigned int size)
  {
    if (!_isBlocking)
      SimpleClient::seek(size);
  }

  VRISKA_ACCESSIBLE
  int 			BlockingClient::readUntil(std::string& buffer, std::string const & delim)
  {
    if (!_isBlocking)
      return (SimpleClient::readUntil(buffer, delim));
    else
      return (readUntil(buffer, delim));
  }

  VRISKA_ACCESSIBLE
  int 			BlockingClient::readLine(std::string& buffer)
  {
    if (!_isBlocking)
      return (SimpleClient::readLine(buffer));
    else
      return (readUntil(buffer, "\n"));
  }

  VRISKA_ACCESSIBLE
  unsigned int 	BlockingClient::sizeToRead() const
  {
    if (!_isBlocking)
      return (SimpleClient::sizeToRead());
    else
      return (0);
  }

  VRISKA_ACCESSIBLE
  unsigned int 	BlockingClient::sizeCanRead() const
  {
    if (!_isBlocking)
      return (SimpleClient::sizeCanRead());
    else
      return (0);
  }

  VRISKA_ACCESSIBLE
  int 			BlockingClient::write(char const *buffer)
  {
    if (!_isBlocking)
      return (SimpleClient::write(buffer));
    else
      return (write(buffer, strlen(buffer)));
  }

  VRISKA_ACCESSIBLE
  int 			BlockingClient::write(void const *buffer, size_t size)
  {
    if (!_isBlocking)
      return (SimpleClient::write(buffer, size));
    else
      {
        char const      *cbuff = static_cast<char const *>(buffer);
        int             ret;
        size_t          idx = 0;

        do
          {
            ret = send(&cbuff[idx], size - idx, true);
            if (ret > 0)
              idx += ret;
          } while (ret > 0 && idx < size);
        return (ret > 0 ? static_cast<int>(idx) : Socket::Error);
      }
  }

  VRISKA_ACCESSIBLE
  int 			BlockingClient::write(std::string const & buffer)
  {
    if (!_isBlocking)
      return (SimpleClient::write(buffer));
    else
      return (write(buffer.c_str(), buffer.size()));
  }

  VRISKA_ACCESSIBLE
  int 			BlockingClient::write(std::string const & buffer, size_t size)
  {
    if (!_isBlocking)
      return (SimpleClient::write(buffer, size));
    else
      return (write(buffer.c_str(), size));
  }

  VRISKA_ACCESSIBLE
  unsigned int 	BlockingClient::sizeToWrite() const
  {
    if (!_isBlocking)
      return (SimpleClient::sizeToWrite());
    else
      return (0);
  }

  VRISKA_ACCESSIBLE
  unsigned int 	BlockingClient::sizeCanWrite() const
  {
    if (!_isBlocking)
      return (SimpleClient::sizeCanWrite());
    else
      return (0);
  }
}
