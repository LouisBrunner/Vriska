
#ifndef VRISKA_LIB_THREADS_INATIVETHREAD_H_
# define VRISKA_LIB_THREADS_INATIVETHREAD_H_

# include <Vriska/Core/OS.h>
# include <Vriska/Threads/IRunnable.h>

namespace Vriska
{
  class INativeThread
  {
  public:
    virtual ~INativeThread() {}

    virtual bool	operator==(INativeThread const & other) const = 0;
    virtual bool	operator!=(INativeThread const & other) const = 0;

  public:
    virtual bool	launch(IRunnable& run) = 0;
    virtual bool	join() = 0;
    virtual bool	terminate() = 0;

  public:
    static INativeThread*	create();
  };
}

#endif // !VRISKA_LIB_THREADS_INATIVETHREAD_H_
