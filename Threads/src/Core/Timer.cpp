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
        bool loop;
        Time elapsed;

        do
        {
            {
                ScopedLock lock(_mutex);

                _timeout.toTimeval(ntime);
                elapsed = _timeout;
            }

# ifdef VRISKA_WINDOWS
            Sleep(elapsed.toMilli());
# else
            int ret;

            ret = ::select(0, NULL, NULL, NULL, &ntime);

            if (ret == -1)
                return;
# endif // !VRISKA_WINDOWS

            loop = _callback.onTimeout(elapsed);
        } while (loop);
    }
}
