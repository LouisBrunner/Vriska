
#ifndef VRISKA_LIB_CORE_OS_H_
# define VRISKA_LIB_CORE_OS_H_

# if defined(__MINGW32__) || defined(_WIN32)
#  define VRISKA_WINDOWS
#  define VRISKA_ACCESSIBLE
#  if defined(VRISKA_LIB_EXPORT)
#   define VRISKA_EXPORT __declspec(dllexport)
#  else
#   define VRISKA_EXPORT __declspec(dllimport)
#  endif // !VRISKA_LIB_CORE
# else
#  define VRISKA_LINUX
#  define VRISKA_ACCESSIBLE __attribute__ ((visibility("default")))
#  define VRISKA_EXPORT
# endif // !__MINGW32__ || _WIN32

# if defined(__MACH__) || defined(__APPLE__)
#  define VRISKA_MAC
# endif // !__MACH__ || __APPLE__

# ifdef VRISKA_WINDOWS
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
