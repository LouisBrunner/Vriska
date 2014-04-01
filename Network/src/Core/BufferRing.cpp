
#include <Vriska/Network/BufferRing.hh>

namespace Vriska
{
  BufferRing::BufferRing() : _numBuff(0)
  {
    enable(true);
  }

  BufferRing::~BufferRing()
  {
    destroy();
  }

  BufferRing::BufferRing(BufferRing const & other)
  {
    copyFrom(other);
  }

  BufferRing const &	BufferRing::operator=(BufferRing const & other)
  {
    ScopedLock		lock1(_mutex);
    ScopedLock		lock2(other._mutex);

    if (this != &other)
      copyFrom(other);
    return (*this);
  }

  int			BufferRing::read(void *buffer, size_t size)
  {
    ScopedLock		lock(_mutex);
    int			ret;

    ret = internFalseRead(buffer, size);
    if (ret > 0)
      internShiftRead(ret);
    return (ret);
  }

  int			BufferRing::read(std::string& buffer)
  {
    return (read(buffer, sizeUsed()));
  }

  int			BufferRing::read(std::string& buffer, size_t size)
  {
    int		ret;

    buffer.resize(size, 0);
    memset(&buffer[0], 0, size);
    ret = read(&buffer[0], size);
    if (ret > -1)
      buffer.resize(ret, 0);
    return (ret);
  }

  int			BufferRing::write(char const *buffer)
  {
    return (write(buffer, strlen(buffer)));
  }

  int					BufferRing::write(void const *buffer, size_t size)
  {
    ScopedLock		lock(_mutex);
    char const		*cbuff = static_cast<char const *>(buffer);
    unsigned int	newSize = static_cast<unsigned int>(size) + ChunkSize;
    unsigned int	currSize = internSizeLeft();
    unsigned int	i = 0;

    if (newSize > currSize)
      addBuffers((newSize - currSize) / ChunkSize);
    currSize = internSizeLeft();
    if (buffer)
      while (i < currSize && i < size)
	{
	  (*this)(i) = cbuff[i];
	  ++i;
	}
    (*this) += i;
    return (i);
  }

  int			BufferRing::write(std::string const & buffer)
  {
    return (write(buffer.c_str(), buffer.size()));
  }

  int			BufferRing::write(std::string const & buffer, size_t size)
  {
    return (write(buffer.c_str(), size));
  }

  unsigned int		BufferRing::sizeUsed() const
  {
    ScopedLock		lock(_mutex);

    return (internSizeUsed());
  }

  unsigned int		BufferRing::internSizeUsed() const
  {
    unsigned int	diff = 0;
    unsigned int	sizeR = 0;
    unsigned int	sizeW = 0;

    if (_chkR == _chkW && _posR <= _posW)
      sizeR =  _posW - _posR;
    else
      {
	if (_chkW > _chkR)
	  diff = _chkW - _chkR - 1;
	else
	  diff = _chkW + (_numBuff - _chkR) - 1;
	sizeR = ChunkSize - _posR;
	sizeW = _posW;
      }
    return (diff * ChunkSize + sizeR + sizeW);
  }

  unsigned int		BufferRing::sizeLeft(bool all) const
  {
    ScopedLock		lock(_mutex);

    return (internSizeLeft(all));
  }

  unsigned int		BufferRing::internSizeLeft(bool all) const
  {
    unsigned int	base;

    if (all)
      base = SizeMax;
    else
      base = currentSize();
    return (base - internSizeUsed() - 1);
  }

  int		BufferRing::search(std::string const & str) const
  {
    return (search(str.c_str(), str.size()));
  }

  int			BufferRing::search(void const *str, size_t size) const
  {
    ScopedLock		lock(_mutex);
    unsigned int	i = 0;
    unsigned int	j;
    char const		*cbuff = static_cast<char const *>(str);
    unsigned int	max = internSizeUsed();

    if (max > size)
      while (i <= max - size)
	{
	  j = 0;
	  while (j < size && cbuff[j] == (*this)[i + j])
	    ++j;
	  if (j == size)
	    return (i);
	  ++i;
	}
    return (Error);
  }

  int		BufferRing::search(char const * str) const
  {
    return (search(str, strlen(str)));
  }

  bool			BufferRing::enable(bool val)
  {
    ScopedLock		lock(_mutex);

    if (val != (_numBuff > 0))
      {
	_numBuff = 0;
	_chkR = 0;
	_chkW = 0;
	_posR = 0;
	_posW = 0;
	if (val)
	  addBuffers(ChunkBase);
	else
	  destroy();
      }
    return (true);
  }

  int		BufferRing::falseRead(std::string& buffer, unsigned int offset)
  {
    return (falseRead(buffer, sizeUsed(), offset));
  }

  int		BufferRing::falseRead(std::string& buffer, size_t size, unsigned int offset)
  {
    int		ret;

    buffer.resize(size, 0);
    memset(&buffer[0], 0, size);
    ret = falseRead(&buffer[0], size, offset);
    if (ret > -1)
      buffer.resize(ret, 0);
    return (ret);
  }

  int			BufferRing::falseRead(void *buffer, size_t size, unsigned int offset)
  {
    ScopedLock		lock(_mutex);

    return (internFalseRead(buffer, size, offset));
  }

  int			BufferRing::internFalseRead(void *buffer, size_t size, unsigned int offset)
  {
    char		*cbuff = static_cast<char *>(buffer);
    unsigned int	i = 0;
    unsigned int	max = internSizeUsed();

    if (buffer)
      while (i < size && i + offset < max)
	{
	  cbuff[i] = (*this)[i + offset];
	  ++i;
	}
    return (i);
  }

  void			BufferRing::shiftRead(unsigned int size)
  {
    ScopedLock		lock(_mutex);

    return (internShiftRead(size));
  }

  void	BufferRing::internShiftRead(unsigned int size)
  {
    (*this) -= size;
    removeBuffers();
  }

  void		BufferRing::destroy()
  {
    for (Iter it = _buffers.begin(); it != _buffers.end(); ++it)
      delete[] (*it);
    _buffers.clear();
  }

  void		BufferRing::copyFrom(BufferRing const & other)
  {
    _numBuff = other._numBuff;
    _chkR = other._chkR;
    _posR = other._posR;
    _chkW = other._chkW;
    _posW = other._posW;
    destroy();
    for (CIter it = other._buffers.begin(); it != other._buffers.end(); ++it)
      {
	char	*buff = new char[ChunkSize];

	memcpy(buff, (*it), ChunkSize);
	_buffers.push_back(buff);
      }
  }

  unsigned int 	BufferRing::currentSize() const
  {
    return (_numBuff * ChunkSize);
  }

  bool				BufferRing::addBuffers(unsigned int n)
  {
    Iter			it = _buffers.begin();
    unsigned int		i;

    n += (n % ChunkAlloc == 0) ? (0) : (ChunkAlloc - (n % ChunkAlloc));
    if (_numBuff >= ChunkMax)
      return (false);
    i = 0;
    while (i <= _chkW && it != _buffers.end())
      {
	++it;
	++i;
      }
    i = 0;
    while (i < n && _numBuff < ChunkMax)
      {
	char	*cbuff = new char[ChunkSize];

	memset(cbuff, 0, ChunkSize);
	if (it == _buffers.end())
	  {
	    _buffers.push_back(cbuff);
	    it = _buffers.end();
	  }
	else
	  {
	    it = _buffers.insert(it, cbuff) + 1;
	    if (_chkR > _chkW)
	      ++_chkR;
	  }
	++i;
	++_numBuff;
      }
    return (true);
  }

  void				BufferRing::removeBuffers()
  {
    unsigned int		n = 0;
    unsigned int		i;
    Iter			it = _buffers.begin();

    if (_chkR == _chkW && _posR <= _posW)
      n = _numBuff - 1;
    else if (_chkW > _chkR)
      n = _numBuff - (_chkW - _chkR + 1);
    else if (_chkW < _chkR)
      n = _chkR - _chkW - 1;
    i = 0;
    if (_numBuff - n < ChunkBase)
      n -= (ChunkBase - (_numBuff - n));
    n -= (n % ChunkAlloc == 0) ? (0) : (n % ChunkAlloc);
    if (n == 0)
      return;
    i = 0;
    while (i < _chkR - 1 && it != _buffers.end())
      {
	++it;
	++i;
      }
    i = 0;
    while (i < n && _numBuff > ChunkBase)
      {
	if (it == _buffers.begin())
	  {
	    delete[] _buffers.front();
	    _buffers.pop_front();
	    it = _buffers.end();
	    --_chkR;
	    --_chkW;
	  }
	else if (it == _buffers.end())
	  {
	    delete[] _buffers.back();
	    _buffers.pop_back();
	    it = _buffers.end();
	  }
	else
	  {
	    delete[] (*it);
	    it = _buffers.erase(it) - 1;
	    --_chkR;
	    if (_chkW > _chkR)
	      --_chkW;
	  }
	++i;
	--_numBuff;
      }
  }

  void			BufferRing::incPosition(unsigned int val, unsigned int max,
						unsigned int& pos, unsigned int& chunk) const
  {
    unsigned int	quotient;
    unsigned int	remainder;

    if (max > 0)
      val %= (max + 1);
    quotient = val / ChunkSize;
    remainder = val % ChunkSize;
    chunk = (chunk + quotient + ((pos + remainder) / ChunkSize)) % _numBuff;
    pos = (pos + remainder) % ChunkSize;
  }

  void				BufferRing::operator+=(unsigned int val)
  {
    incPosition(val, 0, _posW, _chkW);
  }

  void				BufferRing::operator-=(unsigned int val)
  {
    unsigned int		size = internSizeUsed();

    incPosition(val, size, _posR, _chkR);
  }

  char&			BufferRing::operator()(unsigned int idx)
  {
    unsigned int	chunk = _chkW;
    unsigned int	pos = _posW;

    incPosition(idx, internSizeLeft(), pos, chunk);
    return (_buffers[chunk][pos]);
  }

  char				BufferRing::operator[](unsigned int idx) const
  {
    unsigned int	chunk = _chkR;
    unsigned int	pos = _posR;

    incPosition(idx, internSizeUsed(), pos, chunk);
    return (_buffers[chunk][pos]);
  }
}
