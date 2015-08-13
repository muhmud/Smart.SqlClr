
#ifndef _SMART_SQLCLR_AGGREGATES_INTERNAL__DICTIONARYCREATORIMPL_H_
#define _SMART_SQLCLR_AGGREGATES_INTERNAL__DICTIONARYCREATORIMPL_H_

#using <System.dll>
#using <System.Data.dll>

namespace Smart { namespace SqlClr { 
	namespace Types {
		ref class Dictionary;
	}

namespace Aggregates { namespace Internal {
	using namespace System;
	using namespace System::Data::SqlTypes;
	using namespace Microsoft::SqlServer::Server;

	/**
	 * @class DictionaryCreatorImpl
	 *
	 * @brief Implementation for the different types of Dictionary Creator
	 *
	 * The DictionaryCreator aggregate functions are laid out as separate
	 * classes to fulfil the requirements of Sql Server. The implementation
	 * for each of these operations is essentially the same with some
	 * minor settings changes. Hance this implementation class.
	 *
	 * @author Muhmud Ahmad
	 *
	 */
	ref class DictionaryCreatorImpl : public IBinarySerialize {
	private:
		// Settings
		bool m_compressed;
		bool m_sorted;

		// The resulting array
		Types::Dictionary^ m_result;

	public:
		/**
		 * Constructor
		 *
		 * @param compressed Whether or not the resulting Dictionary is compressed
		 * @param sorted Whether or not the resulting Dictionary is sorted
		 *
		 */
		DictionaryCreatorImpl(bool compressed, bool sorted);

		/**
		 * Performs initialization for the aggregate function
		 *
		 */
		void Init();

		/**
		 * Processes incoming values to be aggregated
		 *
		 * @param value The next value to be aggregated
		 *
		 */
		void Accumulate(Object^ key, Object^ value);

		/**
		 * Merges the current instance of the aggregate function with another
		 *
		 * @param value The other instance to merge with
		 *
		 */
		void Merge(DictionaryCreatorImpl^ value);

		/**
		 * Returns the result of the aggregation
		 *
		 * @returns An array
		 *
		 */
		Types::Dictionary^ Terminate();

		/**
		 * Reads in data for a type instance from a binary stream
		 *
		 * @param reader The binary reader to get data from
		 *
		 */
		virtual void Read(IO::BinaryReader^ reader);

		/**
		 * Writes out data for a type instance to a binary stream
		 *
		 * @param writer The binary writer to write data out to
		 *
		 */
		virtual void Write(IO::BinaryWriter^ writer);
	};
}}}}

#endif // _SMART_SQLCLR_AGGREGATES_INTERNAL__DICTIONARYCREATORIMPL_H_
