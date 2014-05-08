
#include <Vriska/Core/Exceptions.h>

namespace Vriska
{    
    VRISKA_ACCESSIBLE
    UnsupportedBehaviorException::UnsupportedBehaviorException(std::string const & message) : Exception(message)
    {
    }
    
    VRISKA_ACCESSIBLE
    UnrecoverableIncidentException::UnrecoverableIncidentException(std::string const & message) : Exception(message)
    {
    }
}
