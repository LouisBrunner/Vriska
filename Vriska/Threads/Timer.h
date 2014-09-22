#ifndef VRISKA_LIB_THREADS_TIMER_H_
# define VRISKA_LIB_THREADS_TIMER_H_

# include <Vriska/Core/OS.h>
# include <Vriska/Core/Time.h>

# include <Vriska/Threads/Mutex.h>
# include <Vriska/Threads/ScopedLock.h>

# ifdef VRISKA_WINDOWS
#  include <winsock2.h>
# else
#  include <sys/select.h>
# endif // !VRISKA_WINDOWS

namespace Vriska
{
    class VRISKA_EXPORT ITimeoutable
    {
        public:
            virtual ~ITimeoutable() {}

        public:
            virtual bool    onTimeout(Time const & elapsed) = 0;
    };

    class VRISKA_EXPORT Timer
    {
    public:
        Timer(ITimeoutable& callback, Time const & timeout);
        virtual ~Timer();

        void setTimeout(Time const & timeout);
        void launch();

    private:
        Mutex _mutex;
        ITimeoutable& _callback;
        Time _timeout;
    };
}

#endif // !VRISKA_LIB_THREADS_TIMER_H_
