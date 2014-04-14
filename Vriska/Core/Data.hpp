
#ifndef VRISKA_LIB_CORE_DATA_HPP_
# define VRISKA_LIB_CORE_DATA_HPP_

# include <Vriska/Core/Exceptions.hpp>

namespace Vriska
{
	class IData
	{
		public:
			virtual ~IData() {}
	};

	template <class Type>
	class Data : public IData
	{
		public:
			Data(Type data) : _data(data) {}
			~Data() {}

		public:
			Type	getData()
			{
				return (_data);
			}

		private:
			Type	_data;
	};

	class Datable
	{
		public:
			Datable() : _data(NULL) {}

			~Datable()
			{
				if (_data != NULL)
					delete _data;
			}

		public:
			template <class Type>
			Type		getData()
			{
				if (_data == NULL)
					throw UnsupportedBehaviorException("You cannot retrieve Data before setting it");
				return (reinterpret_cast<Data<Type> *>(_data)->getData());
			}

			template <class Type>
			void		setData(Type data)
			{
				if (_data != NULL)
					delete _data;
				_data = new Data<Type>(data);
			}

			bool		hasData() const
			{
				return (_data != NULL);
			}

		private:
			IData*			_data;
	};
}

#endif // !VRISKA_LIB_CORE_DATA_HPP_
