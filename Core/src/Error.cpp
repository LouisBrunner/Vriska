
#include <Vriska/Core/Error.hh>

namespace Vriska
{
  std::string Error::_messages[] =
    {
      "No error",
      "Special value for development purpose",
      "Invalid argument",
      "Unknown protocol",
      "Unknown host",
      "Already connected",
      "Not connected",
      "No socket available",
      "Cannot create socket",
      "Cannot bind socket",
      "Cannot connect socket",
      "Disconnected",
      "No place left on the read ring buffer",
      "Unknown error"
    };

  VRISKA_ACCESSIBLE
  std::string const &	Error::getMessage(Error::Code err)
  {
    if (err > LastError)
      err = LastError;
    return (_messages[err]);
  }
}
