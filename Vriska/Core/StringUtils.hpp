
#ifndef VRISKA_LIB_CORE_STRINGUTILS_HPP_
# define VRISKA_LIB_CORE_STRINGUTILS_HPP_

# include <string>
# include <sstream>
# include <vector>
# include <iomanip>

# include <Vriska/Core/OS.h>

namespace Vriska
{
  class VRISKA_EXPORT StringUtils
  {
  private:
    StringUtils(); // static class

  public:
    static std::string	replaceAll(std::string const & subject, char search, char replace);
    static std::string	replaceAll(std::string const & subject, std::string const & search, char replace);
    static std::string	replaceAll(std::string const & subject, char search, std::string const & replace);
    static std::string	replaceAll(std::string subject, std::string const & search, std::string const & replace);

    static std::string const	escape(std::string const & str);
    
    static std::vector<std::string>	split(std::string const & str, char delim = ' ', bool keepEmpty = true);
    static std::vector<std::string>	split(std::string const & str, std::string const & delims = " ", bool keepEmpty = true);

    static std::vector<std::string>	splitDelims(std::string const & str, char delim = ' ', bool keepEmpty = true);
    static std::vector<std::string>	splitDelims(std::string const & str, std::string const & delims = " ", bool keepEmpty = true);
    
    static std::string		join(std::vector<std::string> const & vec, char delim = ' ');
    static std::string		join(std::vector<std::string> const & vec, std::string const & delims = " ");

    template <class Type>
    static std::string		toString(Type v)
    {
      std::ostringstream	oss;

      oss << v;
      return (oss.str());
    }

    template <class Type>
    static Type				fromString(std::string const & str)
    {
      Type				v;
      std::istringstream	iss(str);

      iss >> v;
      return (v);
    }
  };
}

#endif // !VRISKA_LIB_CORE_STRINGUTILS_HPP_
