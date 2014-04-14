
#ifndef VRISKA_LIB_CORE_TIME_H_
# define VRISKA_LIB_CORE_TIME_H_

# include <Vriska/Core/OS.h>

# ifdef VRISKA_WINDOWS
#  include <winsock2.h>
# else
#  include <sys/time.h>
# endif // !VRISKA_WINDOWS

# include <string>
# include <sstream>

namespace Vriska
{
  class VRISKA_EXPORT Time
  {
  public:
    static const Time Zero;

  private: 
    static const unsigned int LimitMicro = 1000000;

  public:
    Time(unsigned int sec = 0, unsigned int micro = 0);
    Time(Time const & other);
    Time const &	operator=(Time const & other);
    virtual ~Time();

  public:
    unsigned int 	getSeconds() const;
    unsigned int 	getMicro() const;
    unsigned int 	getNano() const;

    void	setSeconds(unsigned int sec);
    void	setMicro(unsigned int micro);
    void	setAll(unsigned int sec, unsigned int micro);

    std::string const &	toString() const;

  private:
    void				refreshString();

  public:
    Time			operator+(Time const & other) const;
    Time			operator-(Time const & other) const;
    Time const &	operator+=(Time const & other);
    Time const &	operator-=(Time const & other);

    bool			operator==(Time const & other) const;
    bool			operator!=(Time const & other) const;
    bool			operator>=(Time const & other) const;
    bool			operator<(Time const & other) const;
    bool			operator<=(Time const & other) const;
    bool			operator>(Time const & other) const;

  public:
    void		toTimeval(struct timeval & tval) const;
    void		fromTimeval(struct timeval const & tval);

    unsigned int	toMilli() const;
    void		fromMilli(unsigned int milli);

    static Time		fromSMilli(unsigned int milli);
    static Time		fromSTimeval(struct timeval const & tval);

  private:
    unsigned int 	_sec;
    unsigned int	_micro;
    std::string		_str;
  };

  VRISKA_EXPORT std::ostream&	operator<<(std::ostream& os, Time const & t);
}

#endif // !VRISKA_LIB_CORE_TIME_H_
