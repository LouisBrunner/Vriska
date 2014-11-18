#include <Vriska/Threads/Timer.h>

namespace Vriska
{
    VRISKA_ACCESSIBLE
    Timer::Timer(ITimeoutable& callback, Time const & timeout) : _callback(callback), _timeout(timeout)
    {
    }

    VRISKA_ACCESSIBLE
    Timer::~Timer()
    {
    }

    VRISKA_ACCESSIBLE
    void Timer::setTimeout(Time const & timeout)
    {
        ScopedLock lock(_mutex);

        _timeout = timeout;
    }

    VRISKA_ACCESSIBLE
    void Timer::launch()
    {
        struct timeval ntime;
        int ret;
        bool loop;
        Time elapsed;

        do
        {
            {
                ScopedLock lock(_mutex);

                _timeout.toTimeval(ntime);
                elapsed = _timeout;
            }

            ret = ::select(0, NULL, NULL, NULL, &ntime);

            if (ret == -1)
                return;
            loop = _callback.onTimeout(elapsed);
        } while (loop);
    }
}
