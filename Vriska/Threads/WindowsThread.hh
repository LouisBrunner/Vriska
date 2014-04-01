
#ifndef VRISKA_LIB_THREADS_WINDOWSTHREAD_HH_
# define VRISKA_LIB_THREADS_WINDOWSTHREAD_HH_

# include <Vriska/Threads/INativeThread.hh>

# include <Windows.h>

namespace Vriska
{
  class WindowsThread : public INativeThread
  {
  public:
    WindowsThread();
    ~WindowsThread();

    bool	operator==(INativeThread const & other) const;
    bool	operator!=(INativeThread const & other) const;

  private:
    WindowsThread(INativeThread const & other);
    WindowsThread const &	operator=(INativeThread const & other);

  public:
    bool	launch(IRunnable& run);
    bool	join();
    bool	terminate();

  private:
    static DWORD	start_thread(LPVOID arg);

  private:
    LPDWORD	_threadId;
    HANDLE	_handle;
  };
}

#endif // !VRISKA_LIB_THREADS_WINDOWSTHREAD_HH_

