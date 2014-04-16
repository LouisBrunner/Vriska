
#ifndef VRISKA_LIB_CORE_STRINGUTILS_HPP_
# define VRISKA_LIB_CORE_STRINGUTILS_HPP_

# include <string>
# include <sstream>
# include <vector>
# include <iomanip>
# include <limits>

namespace Vriska
{
  class StringUtils
  {
  private:
    StringUtils(); // static class

  public:
    static std::string	replaceAll(std::string subject, std::string const & search, std::string const & replace)
    {
      size_t  		pos = 0;

      while((pos = subject.find(search, pos)) != std::string::npos)
	{
	  subject = subject.replace(pos, search.length(), replace);
	  pos += replace.length();
	}
      return (subject);
    }

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

    static std::string const	escape(std::string const & str)
    {
      std::ostringstream		oss;
      std::string				res = str;

      res = replaceAll(res, "\\", "\\\\");
      res = replaceAll(res, "\n", "\\n");
      res = replaceAll(res, "\t", "\\t");
      for (int c = 0; c < std::numeric_limits<char>::max(); ++c)
	{
	  if (!isprint(c))
	    {
	      oss.str("");
	      oss << std::setfill('0') << std::setw(2)
		  << std::hex << c;
        res = replaceAll(res, std::string(reinterpret_cast<char*>(&c), 1), "\\" + oss.str());
	    }
	}
      return (res);
    }

    static std::vector<std::string>	split(std::string const & str, std::string const & delim = " ", bool keepEmpty = true)
    {
      size_t				begin;
      size_t				end;
      std::vector<std::string>		tokens;
      std::string			token;

      begin = 0;
      while (begin != std::string::npos)
	{
	  end = str.find(delim, begin);
	  token = str.substr(begin, end - begin);
	  if (keepEmpty || token.size() > 0)
	    tokens.push_back(token);
	  begin = end;
	  if (begin != std::string::npos)
	    begin += delim.size();
	}
      return (tokens);
    }

    static std::vector<std::string>	splitDelims(std::string const & str, std::string const & delims = " ", bool keepEmpty = true)
    {
      size_t				begin;
      size_t				end;
      std::vector<std::string>		tokens;
      std::string			token;

      begin = 0;
      while (begin != std::string::npos)
	{
	  end = str.find_first_of(delims, begin);
	  token = str.substr(begin, end - begin);
	  if (keepEmpty || token.size() > 0)
	    tokens.push_back(token);
	  begin = end;
	  if (begin != std::string::npos)
	    begin += 1;
	}
      return (tokens);
    }

    static std::string		join(std::vector<std::string> const & vec, std::string const & delim = " ")
    {
      bool			first = true;
      std::string		res = "";
      
      for (std::vector<std::string>::const_iterator it = vec.begin(); it != vec.end(); ++it)
	{
	  if (first)
	    first = false;
	  else
	    res += delim;
	  res += (*it);
	}
      return (res);
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

#endif // !VRISKA_LIB_CORE_STRINGUTILS_HPP_
