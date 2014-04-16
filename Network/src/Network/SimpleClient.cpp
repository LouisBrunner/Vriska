
#include <Vriska/Network/SimpleClient.h>

namespace Vriska
{
  VRISKA_ACCESSIBLE
  SimpleClient::SimpleClient()
  {
  }

  VRISKA_ACCESSIBLE
  SimpleClient::SimpleClient(INativeSocket& sysSocket) : SocketClient(sysSocket)
  {
  }

  VRISKA_ACCESSIBLE
  SimpleClient::~SimpleClient()
  {
  }

  VRISKA_ACCESSIBLE
  int				SimpleClient::read(void *buffer, size_t size)
  {
    return (_brRead.read(buffer, size));
  }

  VRISKA_ACCESSIBLE
  int 			SimpleClient::read(std::string& buffer)
  {
    return (_brRead.read(buffer));
  }

  VRISKA_ACCESSIBLE
  int 			SimpleClient::read(std::string& buffer, size_t size)
  {
    return (_brRead.read(buffer, size));
  }

  VRISKA_ACCESSIBLE
  int		SimpleClient::falseRead(std::string& buffer, size_t size, unsigned int offset)
  {
    return (_brRead.falseRead(buffer, size, offset));
  }

  VRISKA_ACCESSIBLE
  int		SimpleClient::falseRead(void *buffer, size_t size, unsigned int offset)
  {
    return (_brRead.falseRead(buffer, size, offset));
  }

  VRISKA_ACCESSIBLE
  void		SimpleClient::shiftRead(unsigned int size)
  {
    _brRead.shiftRead(size);
  }

  VRISKA_ACCESSIBLE
  int 			SimpleClient::readUntil(std::string& buffer, std::string const & delim)
  {
    int			pos = _brRead.search(delim);
    std::string		d;
    int 		ret;

    if (pos == BufferRing::Error)
      return (pos);
    ret = _brRead.read(buffer, pos);
    _brRead.read(d, delim.size());
    return (ret);
  }

  VRISKA_ACCESSIBLE
  int 			SimpleClient::readLine(std::string& buffer)
  {
    return (readUntil(buffer, "\n"));
  }

  VRISKA_ACCESSIBLE
  unsigned int 	SimpleClient::sizeToRead() const
  {
    return (_brRead.sizeUsed());
  }

  VRISKA_ACCESSIBLE
  unsigned int 	SimpleClient::sizeCanRead() const
  {
    return (_brRead.sizeLeft(true));
  }

  VRISKA_ACCESSIBLE
  int 			SimpleClient::write(char const *buffer)
  {
    return (_brWrite.write(buffer));
  }

  VRISKA_ACCESSIBLE
  int 			SimpleClient::write(void const *buffer, size_t size)
  {
    return (_brWrite.write(buffer, size));
  }

  VRISKA_ACCESSIBLE
  int 			SimpleClient::write(std::string const & buffer)
  {
    return (_brWrite.write(buffer));
  }

  VRISKA_ACCESSIBLE
  int 			SimpleClient::write(std::string const & buffer, size_t size)
  {
    return (_brWrite.write(buffer, size));
  }

  VRISKA_ACCESSIBLE
  unsigned int 	SimpleClient::sizeToWrite() const
  {
    return (_brWrite.sizeUsed());
  }

  VRISKA_ACCESSIBLE
  unsigned int 	SimpleClient::sizeCanWrite() const
  {
    return (_brWrite.sizeLeft(true));
  }

  Error::Code			SimpleClient::doRead()
  {
    Error::Code		err = Error::NoError;
    std::string		all;
    char		*buffer = new char[SizeBuffer];
    int			ret = -1;
    unsigned int	size;
    unsigned int	left;

    do
      {
	size = SizeBuffer;
	left = _brRead.sizeLeft(true);
	if (left == 0)
	  {
	    err = Error::NoPlaceR;
	    break;
	  }
	if (size > left)
	  size = left;
	memset(buffer, 0, SizeBuffer);
	ret = recv(buffer, size);
	if (ret > 0)
	  {
	    _brRead.write(buffer, ret);
	    all += std::string(buffer, ret);
	  }
      } while (static_cast<int>(size) == ret);
    delete[] buffer;
    if (all != "")
      sysLog("Received: " + StringUtils::escape(all));
    if (ret == Socket::Error)
      return (Error::Disconnected);
    return (err);
  }

  Error::Code		SimpleClient::doWrite()
  {
    std::string		all;
    char			*buffer = new char[SizeBuffer];
    int				ret = -1;
    unsigned int	size;
    unsigned int	used;

    do
      {
	size = SizeBuffer;
	used = _brWrite.sizeUsed();
	if (used == 0)
	  break;
	if (size > used)
	  size = used;
	memset(buffer, 0, SizeBuffer);
	_brWrite.falseRead(buffer, size);
	ret = send(buffer, size);
	if (ret > 0)
	  {
	    _brWrite.shiftRead(ret);
	    all += std::string(buffer, ret);
	  }
      } while (static_cast<int>(size) == ret);
    delete[] buffer;
    if (all != "")
      sysLog("Sent: " + StringUtils::escape(all));
    if (ret == Socket::Error)
      return (Error::Disconnected);
    return (Error::NoError);
  }
}
