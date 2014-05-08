
#ifndef VRISKA_LIB_CORE_EXCEPTIONS_H_
# define VRISKA_LIB_CORE_EXCEPTIONS_H_

# include <exception>
# include <string>

# include <Vriska/Core/OS.h>

namespace Vriska
{
	class VRISKA_EXPORT Exception : public std::exception
	{
		public:
			Exception(std::string const & message = "An exception occured") : _message(message) {}
			virtual ~Exception() throw() {}

			const char* what() const throw()
			{
			  return (_message.c_str());
			}

		private:
			std::string	_message;
	};

	class VRISKA_EXPORT UnsupportedBehaviorException : public Exception
	{
		public:
			UnsupportedBehaviorException(std::string const & message = "An unsupported behavior occured");
	};

	class VRISKA_EXPORT UnrecoverableIncidentException : public Exception
	{
		public:
			UnrecoverableIncidentException(std::string const & message = "An unrecoverable incident occured");
	};
}

#endif // !VRISKA_LIB_CORE_EXCEPTIONS_H_
