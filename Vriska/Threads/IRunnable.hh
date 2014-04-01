
#ifndef VRISKA_LIB_THREADS_IRUNNABLE_HH_
# define VRISKA_LIB_THREADS_IRUNNABLE_HH_

namespace Vriska
{
	class IRunnable
	{
		public:
			virtual ~IRunnable() {}

			virtual void	run() = 0;
	};
}

#endif // !VRISKA_LIB_THREADS_IRUNNABLE_HH_
