
#ifndef VRISKA_LIB_NETWORK_BUFFERRING_H_
# define VRISKA_LIB_NETWORK_BUFFERRING_H_

# include <cstring>
# include <string>
# include <deque>

# include <Vriska/Network/Socket.h>
# include <Vriska/Threads/ScopedLock.h>
# include <Vriska/Threads/Mutex.h>

namespace Vriska
{
  class BufferRing
  {
  public:
    static const int	Error = Socket::Error;

  private:
    static const unsigned int 	ChunkSize = 4096;
    static const unsigned int 	ChunkBase = 10;
    static const unsigned int 	ChunkAlloc = 10;
    static const unsigned int 	ChunkMax = 10;
    static const unsigned int 	SizeMax = ChunkSize * ChunkMax;

    typedef std::deque<char *>::iterator 	Iter;
    typedef std::deque<char *>::const_iterator	CIter;

  public:
    BufferRing();
    ~BufferRing();

    BufferRing(BufferRing const & other);
    BufferRing const &	operator=(BufferRing const & other);

  public:
    int		read(void *buffer, size_t size);
    int		read(std::string& buffer);
    int		read(std::string& buffer, size_t size);

  public:
    int		write(char const *buffer);
    int		write(void const *buffer, size_t size);
    int		write(std::string const & buffer);
    int		write(std::string const & buffer, size_t size);

  public:
    unsigned int	sizeUsed() const;
    unsigned int	sizeLeft(bool all = false) const;

  public:
    int		search(std::string const & str) const;
    int		search(void const * str, size_t size) const;
    int		search(char const * str) const;

    bool	enable(bool val);

    int		falseRead(void *buffer, size_t size, unsigned int offset = 0);
    int		falseRead(std::string& buffer, unsigned int offset = 0);
    int		falseRead(std::string& buffer, size_t size, unsigned int offset = 0);
    void	shiftRead(unsigned int size);

  private:
    int			internFalseRead(void *buffer, size_t size, unsigned int offset = 0);
    void		internShiftRead(unsigned int size);
    unsigned int	internSizeUsed() const;
    unsigned int	internSizeLeft(bool all = false) const;    

    void		destroy();
    void		copyFrom(BufferRing const & other);
    unsigned int 	currentSize() const;
    bool		addBuffers(unsigned int n);
    void		removeBuffers();
    void		incPosition(unsigned int val, unsigned int max,
				    unsigned int& pos, unsigned int& chunk) const;
    void		operator+=(unsigned int val);
    void		operator-=(unsigned int val);
    char&		operator()(unsigned int idx);
    char		operator[](unsigned int idx) const;

  private:
    std::deque<char *>	_buffers;
    unsigned int		_numBuff;
    unsigned int		_chkR;
    unsigned int		_posR;
    unsigned int		_chkW;
    unsigned int		_posW;

    Mutex			_mutex;
  };
}

#endif // !VRISKA_LIB_NETWORK_BUFFERRING_H_
