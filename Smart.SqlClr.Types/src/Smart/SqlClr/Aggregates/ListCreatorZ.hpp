
#ifndef _SMART_SQLCLR_AGGREGATES__LISTCREATORZ_H_
#define _SMART_SQLCLR_AGGREGATES__LISTCREATORZ_H_

#using <System.dll>
#using <System.Data.dll>

namespace Smart { namespace SqlClr { 
	namespace Types {
		ref class List;
	}

namespace Aggregates {
	namespace Internal {
		ref class ListCreatorImpl;
	}

	using namespace System;
	using namespace System::Data::SqlTypes;
	using namespace Microsoft::SqlServer::Server;

	/**
	 * @class ListCreatorZ
	 *
	 * @brief Can be used to create compressed Lists in SQL
	 *
	 * This aggregate function is used to quickly create
	 * a List from SQL data.
	 *
	 * @author Muhmud Ahmad
	 *
	 */
	[Serializable]
	[SqlUserDefinedAggregate(Format::UserDefined, IsInvariantToNulls = true, IsInvariantToDuplicates = false, IsInvariantToOrder = false, MaxByteSize = -1)]
	public ref class ListCreatorZ sealed : public IBinarySerialize {
	private:
		// Constants
		literal bool Sorted = false;
		literal bool Compressed = true;

		// Implementation
		Internal::ListCreatorImpl^ m_impl;

	public:
		/// Constructor
		ListCreatorZ();

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
		void Accumulate(Object^ value);

		/**
		 * Merges the current instance of the aggregate function with another
		 *
		 * @param value The other instance to merge with
		 *
		 */
		void Merge(ListCreatorZ^ value);

		/**
		 * Returns the result of the aggregation
		 *
		 * @returns An array
		 *
		 */
		Types::List^ Terminate();

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

#endif // _SMART_SQLCLR_AGGREGATES__LISTCREATORZ_H_
