
#include <Vriska/Threads/WindowsThread.h>

namespace Vriska
{
  WindowsThread::WindowsThread() : _threadId(0)
  {
  }

  WindowsThread::~WindowsThread()
  {
  }

  bool			WindowsThread::operator==(INativeThread const & other) const
  {
    WindowsThread const &	nt = reinterpret_cast<WindowsThread const &>(other);
    
    return (_threadId == nt._threadId);
  }

  bool			WindowsThread::operator!=(INativeThread const & other) const
  {
    return (!(*this == other));
  }

  bool			WindowsThread::launch(IRunnable& run)
  {
    return ((_handle = CreateThread(NULL, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(&start_thread), &run, 0, _threadId)) != NULL);
  }

  bool			WindowsThread::join()
  {
    return (WaitForSingleObject(_handle, INFINITE) != WAIT_FAILED);
  }

  bool			WindowsThread::terminate()
  {
    return (TerminateThread(_handle, 0) != 0);
  }

  DWORD		WindowsThread::start_thread(LPVOID arg)
  {
    IRunnable	*run = reinterpret_cast<IRunnable *>(arg);

    run->run();
    return (0);
  }
}
