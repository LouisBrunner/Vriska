
#include <Vriska/Core/Utils.hpp>

namespace Vriska
{
    VRISKA_ACCESSIBLE
    char     Utils::getPathSeparator()
    {
# ifdef VRISKA_WINDOWS
        return '\\';
# else
        return '/';
# endif // !VRISKA_WINDOWS
    }
    
    VRISKA_ACCESSIBLE
    std::string  Utils::pathToNative(std::string const & path, char separator)
    {
        char nativeSeparator = getPathSeparator();

        if (separator == nativeSeparator)
            return path;
        return StringUtils::replaceAll(path, separator, nativeSeparator);
    }
    
    VRISKA_ACCESSIBLE
    bool		Utils::isBigEndian()
    {
      int		i;
      char		*tab;

      i = 1;
      tab = (char *) &i;
      return (tab[0] == 0);
    }
    
    VRISKA_ACCESSIBLE
    char		Utils::toByte(int n)
    {
      return (static_cast<char>(n));
    }
}