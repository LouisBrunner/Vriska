
#ifndef VRISKA_LIB_CORE_OS_H_
# define VRISKA_LIB_CORE_OS_H_

# if defined(__MINGW32__) || defined(_WIN32)
#  define VRISKA_WINDOWS
/*#  define VRISKA_ACCESSIBLE __attribute__ ((dllexport))*/
#  define VRISKA_ACCESSIBLE
#  define VRISKA_EXPORT __declspec(dllexport)
# else
#  define VRISKA_LINUX
#  define VRISKA_ACCESSIBLE __attribute__ ((visibility("default")))
#  define VRISKA_EXPORT
# endif // !__MINGW32__ || _WIN32

# ifdef VRISKA_WINDOWS
#  pragma warning( push )
#  pragma warning( disable : 4127 )
#  pragma warning( disable : 4251 )
#  pragma warning( disable : 4275 )
# endif

namespace Vriska
{
  class OS
  {
  public:
    enum Name
      {
	Linux = 0,
	Windows
      };

    static Name	getOS();

  private:
    OS(); // Static class
  };
}

#endif // !VRISKA_LIB_CORE_OS_H_
