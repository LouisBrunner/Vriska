
#include <Vriska/Core/StringUtils.hpp>

namespace Vriska
{
    VRISKA_ACCESSIBLE
    std::string	StringUtils::replaceAll(std::string const & subject, char search, char replace)
    {
        return replaceAll(subject, std::string(reinterpret_cast<char*>(&search), 1), std::string(reinterpret_cast<char*>(&replace), 1));
    }
    
    VRISKA_ACCESSIBLE
    std::string	StringUtils::replaceAll(std::string const & subject, std::string const & search, char replace)
    {
        return replaceAll(subject, search, std::string(reinterpret_cast<char*>(&replace), 1));
    }
    
    VRISKA_ACCESSIBLE
    std::string	StringUtils::replaceAll(std::string const & subject, char search, std::string const & replace)
    {
        return replaceAll(subject, std::string(reinterpret_cast<char*>(&search), 1), replace);
    }
    
    VRISKA_ACCESSIBLE
    std::string	StringUtils::replaceAll(std::string subject, std::string const & search, std::string const & replace)
    {
      size_t  		pos = 0;

      while((pos = subject.find(search, pos)) != std::string::npos)
	  {
	    subject = subject.replace(pos, search.length(), replace);
	    pos += replace.length();
	  }
      return (subject);
    }
    
    VRISKA_ACCESSIBLE
    std::string const	StringUtils::escape(std::string const & str)
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
    
    VRISKA_ACCESSIBLE
    std::vector<std::string>	StringUtils::split(std::string const & str, char delim, bool keepEmpty)
    {
        return split(str, std::string(reinterpret_cast<char*>(&delim), 1), keepEmpty);
    }
    
    VRISKA_ACCESSIBLE
    std::vector<std::string>	StringUtils::split(std::string const & str, std::string const & delims, bool keepEmpty)
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
    
    VRISKA_ACCESSIBLE
    std::vector<std::string>	StringUtils::splitDelims(std::string const & str, char delim, bool keepEmpty)
    {
        return splitDelims(str, std::string(reinterpret_cast<char*>(&delim), 1), keepEmpty);
    }
    
    VRISKA_ACCESSIBLE
    std::vector<std::string>	StringUtils::splitDelims(std::string const & str, std::string const & delims, bool keepEmpty)
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
    
    VRISKA_ACCESSIBLE
    std::string		StringUtils::join(std::vector<std::string> const & vec, char delim)
    {
        return join(vec, std::string(reinterpret_cast<char*>(&delim), 1));
    }
    
    VRISKA_ACCESSIBLE
    std::string		StringUtils::join(std::vector<std::string> const & vec, std::string const & delims)
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
}
