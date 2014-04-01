
#ifndef ERROR_HH_
# define ERROR_HH_

# include <Vriska/Core/OS.hh>
# include <string>

namespace Vriska
{

	class VRISKA_EXPORT Error
	{
		public:
			enum Code
			{
				NoError = 0,
				SpecialValue,
				InvalidArgument,
				UnknownProtocol,
				UnknowHost,
				AlreadyConnected,
				NotConnected,
				NoSocket,
				CannotCreateSocket,
				CannotBindSocket,
				CannotConnectSocket,
				Disconnected,
				NoPlaceR,
				LastError
			};

			static std::string const &	getMessage(Code err);

		private:
			Error(); // Static class

			static std::string	_messages[];
	};
}

#endif // !ERROR_HH_
