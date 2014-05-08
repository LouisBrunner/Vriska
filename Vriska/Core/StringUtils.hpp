
#ifndef VRISKA_LIB_CORE_STRINGUTILS_HPP_
# define VRISKA_LIB_CORE_STRINGUTILS_HPP_

# include <string>
# include <sstream>
# include <vector>
# include <iomanip>

namespace Vriska
{
  class StringUtils
  {
  private:
    StringUtils(); // static class

  public:
    static std::string	replaceAll(std::string const & subject, char search, char replace)
    {
        return replaceAll(subject, std::string(reinterpret_cast<char*>(&search), 1), std::string(reinterpret_cast<char*>(&replace), 1));
    }

    static std::string	replaceAll(std::string const & subject, std::string const & search, char replace)
    {
        return replaceAll(subject, search, std::string(reinterpret_cast<char*>(&replace), 1));
    }
    
    static std::string	replaceAll(std::string const & subject, char search, std::string const & replace)
    {
        return replaceAll(subject, std::string(reinterpret_cast<char*>(&search), 1), replace);
    }

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
      for (int c = 0; c <= 127; ++c)
	{
	  if (!isprint(c))
	    {
	      oss.str("");
	      oss << std::setfill('0') << std::setw(2)
		  << std::hex << c;
	      res = replaceAll(res, static_cast<char>(c), "\\" + oss.str());
	    }
	}
      return (res);
    }
    
    static std::vector<std::string>	split(std::string const & str, char delim = ' ', bool keepEmpty = true)
    {
        return split(str, std::string(reinterpret_cast<char*>(&delim), 1), keepEmpty);
    }

    static std::vector<std::string>	split(std::string const & str, std::string const & delims = " ", bool keepEmpty = true)
    {
      size_t				begin;
      size_t				end;
      std::vector<std::string>		tokens;
      std::string			token;

      begin = 0;
      while (begin != std::string::npos)
	{
	  end = str.find(delims, begin);
	  token = str.substr(begin, end - begin);
	  if (keepEmpty || token.size() > 0)
	    tokens.push_back(token);
	  begin = end;
	  if (begin != std::string::npos)
	    begin += delims.size();
	}
      return (tokens);
    }
    
    static std::vector<std::string>	splitDelims(std::string const & str, char delim = ' ', bool keepEmpty = true)
    {
        return splitDelims(str, std::string(reinterpret_cast<char*>(&delim), 1), keepEmpty);
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
    
    static std::string		join(std::vector<std::string> const & vec, char delim = ' ')
    {
        return join(vec, std::string(reinterpret_cast<char*>(&delim), 1));
    }

    static std::string		join(std::vector<std::string> const & vec, std::string const & delims = " ")
    {
      bool			first = true;
      std::string		res = "";
      
      for (std::vector<std::string>::const_iterator it = vec.begin(); it != vec.end(); ++it)
	{
	  if (first)
	    first = false;
	  else
	    res += delims;
	  res += (*it);
	}
      return (res);
    }
  };
}

#endif // !VRISKA_LIB_CORE_STRINGUTILS_HPP_
