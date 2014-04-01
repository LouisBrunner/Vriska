
#include <Vriska/Core/Time.hh>

namespace Vriska
{
  VRISKA_ACCESSIBLE
  const Time	Time::Zero = Time();


  VRISKA_ACCESSIBLE
  Time::Time(unsigned int sec, unsigned int micro)
  {
    setAll(sec, micro);
  }

  VRISKA_ACCESSIBLE
  Time::Time(Time const & other)
  {
    setAll(other._sec, other._micro);
  }

  VRISKA_ACCESSIBLE
  Time const &	Time::operator=(Time const & other)
  {
    if (this != &other)
      setAll(other._sec, other._micro);
    return (*this);
  }

  VRISKA_ACCESSIBLE
  Time::~Time()
  {
  }

  VRISKA_ACCESSIBLE
  unsigned int 	Time::getSeconds() const
  {
    return (_sec);
  }

  VRISKA_ACCESSIBLE
  unsigned int 	Time::getMicro() const
  {
    return (_micro);
  }

  VRISKA_ACCESSIBLE
  unsigned int 	Time::getNano() const
  {
    return (_micro * 1000);
  }

  VRISKA_ACCESSIBLE
  void	Time::setSeconds(unsigned int sec)
  {
    _sec = sec;
    refreshString();
  }

  VRISKA_ACCESSIBLE
  void	Time::setMicro(unsigned int micro)
  {
    _micro = micro % LimitMicro;
    _sec += micro / LimitMicro;
    refreshString();
  }

  VRISKA_ACCESSIBLE
  void	Time::setAll(unsigned int sec, unsigned int micro)
  {
    _sec = sec;
    _micro = micro % LimitMicro;
    _sec += micro / LimitMicro;
    refreshString();
  }

  VRISKA_ACCESSIBLE
  std::string const &	Time::toString() const
  {
    return (_str);
  }

  void					Time::refreshString()
  {
    std::ostringstream	oss;

    if (_sec > 0 || _micro == 0)
      oss << _sec << "sec";
    if (_sec > 0 && _micro > 0)
      oss << " and ";
    if (_micro > 0)
      oss << _micro << "usec";
    _str = oss.str();
  }

  VRISKA_ACCESSIBLE
  Time			Time::operator+(Time const & other) const
  {
    Time		add(*this);

    add += other;
    return (add);
  }

  VRISKA_ACCESSIBLE
  Time			Time::operator-(Time const & other) const
  {
    Time		sub(*this);

    sub -= other;
    return (sub);
  }

  VRISKA_ACCESSIBLE
  Time const &	Time::operator+=(Time const & other)
  {
    unsigned int micro = (_micro + other._micro);

    _micro = micro % LimitMicro;
    _sec += micro / LimitMicro + other._sec;
    refreshString();
    return (*this);
  }

  VRISKA_ACCESSIBLE
  Time const &	Time::operator-=(Time const & other)
  {
    bool		minus = false;

    if (other._sec > _sec)
      {
	_sec = 0;
	_micro = 0;
      }
    else
      {
	if (other._micro > _micro)
	  {
	    _micro = LimitMicro - other._micro;
	    minus = true;
	  }
	else
	  _micro -= other._micro;
	if (minus && _sec == 0)
	  _micro = 0;
	else
	  {
	    if (minus)
	      --_sec;
	    _sec -= other._sec;
	  }
      }
    refreshString();
    return (*this);
  }

  VRISKA_ACCESSIBLE
  bool			Time::operator==(Time const & other) const
  {
    return (_sec == other._sec && _micro == other._micro);
  }

  VRISKA_ACCESSIBLE
  bool			Time::operator!=(Time const & other) const
  {
    return (!(*this == other));
  }

  VRISKA_ACCESSIBLE
  bool			Time::operator<(Time const & other) const
  {
    if (_sec < other._sec)
      return (true);
    else if (other._sec == _sec && _micro < other._micro)
      return (true);
    return (false);
  }

  VRISKA_ACCESSIBLE
  bool			Time::operator>=(Time const & other) const
  {
    return (!(*this < other));
  }

  VRISKA_ACCESSIBLE
  bool			Time::operator>(Time const & other) const
  {
    if (_sec > other._sec)
      return (true);
    else if (other._sec == _sec && _micro > other._micro)
      return (true);
    return (false);
  }

  VRISKA_ACCESSIBLE
  bool			Time::operator<=(Time const & other) const
  {
    return (!(*this > other));
  }

  VRISKA_ACCESSIBLE
  void	Time::toTimeval(struct timeval & tval) const
  {
    tval.tv_sec = _sec;
    tval.tv_usec = _micro;
  }

  VRISKA_ACCESSIBLE
  void	Time::fromTimeval(struct timeval const & tval)
  {
    _sec = tval.tv_sec;
    _micro = tval.tv_usec;
    refreshString();
  }

  VRISKA_ACCESSIBLE
  unsigned int	Time::toMilli() const
  {
    return (_sec * 1000 + _micro / 1000);
  }

  VRISKA_ACCESSIBLE
  void		Time::fromMilli(unsigned int milli)
  {
    setAll(milli / 1000, (milli % 1000) * 1000);
  }

  VRISKA_ACCESSIBLE
  Time		Time::fromSMilli(unsigned int milli)
  {
    Time	t;

    t.fromMilli(milli);
    return (t);
  }

  VRISKA_ACCESSIBLE
  Time		Time::fromSTimeval(struct timeval const & tval)
  {
    Time	t;

    t.fromTimeval(tval);
    return (t);
  }

  VRISKA_ACCESSIBLE
  std::ostream&	operator<<(std::ostream& os, Time const & t)
  {
    os << t.toString();
    return (os);
  }
}
