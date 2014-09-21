
#include <Vriska/Threads/LinuxThread.h>

#include <iostream>

namespace Vriska
{
  LinuxThread::LinuxThread()
  {
  }

  LinuxThread::~LinuxThread()
  {
  }

  bool			LinuxThread::operator==(INativeThread const & other) const
  {
    LinuxThread const &	nt = reinterpret_cast<LinuxThread const &>(other);

    return (pthread_equal(_thread, nt._thread) != 0);
  }

  bool			LinuxThread::operator!=(INativeThread const & other) const
  {
    return (!(*this == other));
  }

  bool			LinuxThread::launch(IRunnable& run)
  {
    return (pthread_create(&_thread, NULL, &start_thread, &run) == 0);
  }

  bool			LinuxThread::join()
  {
    return (pthread_join(_thread, NULL) == 0);
  }

  bool			LinuxThread::terminate()
  {
    return (pthread_cancel(_thread) == 0);
  }

  void*		LinuxThread::start_thread(void *arg)
  {
    IRunnable	*run = reinterpret_cast<IRunnable *>(arg);

    // pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    // pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    run->run();
    pthread_exit(NULL);
    return (NULL);
  }
}
