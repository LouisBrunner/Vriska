
#include <Vriska/Core/Exceptions.h>

namespace Vriska
{
    VRISKA_ACCESSIBLE
    Exception::Exception(std::string const & message) : _message(message)
    {
    }
    
    VRISKA_ACCESSIBLE
    Exception::~Exception() throw()   
    {
    }

    VRISKA_ACCESSIBLE
    const char* Exception::what() const throw()
	{
		return (_message.c_str());
	}
    
    VRISKA_ACCESSIBLE
    UnsupportedBehaviorException::UnsupportedBehaviorException(std::string const & message) : Exception(message)
    {
    }
    
    VRISKA_ACCESSIBLE
    UnrecoverableIncidentException::UnrecoverableIncidentException(std::string const & message) : Exception(message)
    {
    }
}