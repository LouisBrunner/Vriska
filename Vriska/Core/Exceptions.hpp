
#ifndef VRISKA_LIB_CORE_EXCEPTIONS_HPP_
# define VRISKA_LIB_CORE_EXCEPTIONS_HPP_

# include <exception>
# include <string>

namespace Vriska
{
	class Exception : public std::exception
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

	class UnsupportedBehaviorException : public Exception
	{
		public:
			UnsupportedBehaviorException(std::string const & message = "An unsupported behavior occured") : Exception(message) {}
	};

	class UnrecoverableIncidentException : public Exception
	{
		public:
			UnrecoverableIncidentException(std::string const & message = "An unrecoverable incident occured") : Exception(message) {}
	};
}

#endif // !VRISKA_LIB_CORE_EXCEPTIONS_HPP_
