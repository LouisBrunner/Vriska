
#ifndef VRISKA_LIB_THREADS_IRUNNABLE_H_
# define VRISKA_LIB_THREADS_IRUNNABLE_H_

namespace Vriska
{
	class IRunnable
	{
		public:
			virtual ~IRunnable() {}

			virtual void	run() = 0;
	};
}

#endif // !VRISKA_LIB_THREADS_IRUNNABLE_H_
