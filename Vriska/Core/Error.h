
#ifndef VRISKA_LIB_CORE_ERROR_H_
# define VRISKA_LIB_CORE_ERROR_H_

# include <Vriska/Core/OS.h>
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

#endif // !VRISKA_LIB_CORE_ERROR_H_
