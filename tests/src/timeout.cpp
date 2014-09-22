#include <iostream>
#include <cstdlib>

#include <Vriska/Threads.h>

class MyTimeout : public Vriska::ITimeoutable
{
public:
  MyTimeout() : _longPhase(true)
  {
  }

  bool onTimeout(Vriska::Time const & elapsed)
  {
    std::cout << "Timeout: " << elapsed << std::endl;
    _timer->setTimeout(_longPhase ? Vriska::Time::fromSMilli(1000) : Vriska::Time::fromSMilli(500));
    _longPhase = !_longPhase;
    return true;
  }

  Vriska::Timer* _timer;

private:
  bool _longPhase;
};

int main()
{
  MyTimeout mt;

  Vriska::Timer timer(mt, Vriska::Time::fromSMilli(500));

  mt._timer = &timer;

  timer.launch();

  return (0);
}