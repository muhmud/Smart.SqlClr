
#ifndef _SMART_SQLCLR_AGGREGATES__DICTIONARYCREATOR_H_
#define _SMART_SQLCLR_AGGREGATES__DICTIONARYCREATOR_H_

#using <System.dll>
#using <System.Data.dll>

namespace Smart { namespace SqlClr { 
	namespace Types {
		ref class Dictionary;
	}

namespace Aggregates {
	namespace Internal {
		ref class DictionaryCreatorImpl;
	}

	using namespace System;
	using namespace System::Data::SqlTypes;
	using namespace Microsoft::SqlServer::Server;

	/**
	 * @class DictionaryCreator
	 *
	 * @brief Can be used to create Dictionary objects in SQL
	 *
	 * This aggregate function is used to quickly create
	 * a dictionary from SQL data.
	 *
	 * @author Muhmud Ahmad
	 *
	 */
	[Serializable]
	[SqlUserDefinedAggregate(Format::UserDefined, IsInvariantToNulls = true, IsInvariantToDuplicates = false, IsInvariantToOrder = false, MaxByteSize = -1)]
	public ref class DictionaryCreator sealed : public IBinarySerialize {
	private:
		// Constants
		literal bool Sorted = false;
		literal bool Compressed = false;

		// Implementation
		Internal::DictionaryCreatorImpl^ m_impl;

	public:
		/// Constructor
		DictionaryCreator();

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
		void Merge(DictionaryCreator^ value);

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
}}}

#endif // _SMART_SQLCLR_AGGREGATES__DICTIONARYCREATOR_H_
