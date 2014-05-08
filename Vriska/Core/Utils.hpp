
#ifndef VRISKA_LIB_CORE_UTILS_HPP_
# define VRISKA_LIB_CORE_UTILS_HPP_

# include <string>
# include <Vriska/Core/StringUtils.hpp>

namespace Vriska
{
  class Utils
  {
  private:
    Utils(); // static class

  public:
    static char     getPathSeparator()
    {
# ifdef VRISKA_WINDOWS
        return '\\';
# else
        return '/';
# endif // !VRISKA_WINDOWS
    }

    static std::string  pathToNative(std::string const & path, char separator = '/')
    {
        char nativeSeparator = getPathSeparator();

        if (separator == nativeSeparator)
            return path;
        return StringUtils::replaceAll(path, separator, nativeSeparator);
    }

    static bool		isBigEndian()
    {
      int		i;
      char		*tab;

      i = 1;
      tab = (char *) &i;
      return (tab[0] == 0);
    }

    template <class Type>
    static std::string		toEndianess(Type n, bool toBig = true)
    {
      char			byte;
      std::string		bytes;
      unsigned int		max = sizeof(Type);

      for (unsigned int i = 0; i < max; ++i)
	{
	  if ((isBigEndian() && toBig) || (!isBigEndian() && !toBig))
	    byte = (n >> (i * 8)) & 0xFF;
	  else
	    byte = (n >> ((max - i - 1) * 8)) & 0xFF;
	  bytes += byte;
	}
      return (bytes);
    }

    template <class Type>
    static Type			fromEndianess(std::string const & bytes, bool fromBig = true)
    {
      Type			n = 0;
      unsigned int		max = sizeof(Type);
      
      for (unsigned int i = 0; i < max && i < bytes.size(); ++i)
	{
	  if ((isBigEndian() && fromBig) || (!isBigEndian() && !fromBig))
	    n |= (bytes[i] << (i * 8));
	  else
	    n |= ((bytes[i] & 0xFF) << ((max - i - 1) * 8));
	}
      return (n);
    }

    static char		toByte(int n)
    {
      return (static_cast<char>(n));
    }
  };
}

#endif // !VRISKA_LIB_CORE_UTILS_HPP_