
#ifndef _SMART_SQLCLR_TYPES__TUPLESET_HPP_
#define _SMART_SQLCLR_TYPES__TUPLESET_HPP_

#ifndef _SMART_SQLCLR_TYPES__TUPLE_HPP_
#include <Smart/SqlClr/Types/Tuple.hpp>
#endif // _SMART_SQLCLR_TYPES__TUPLE_HPP_

#using <System.dll>
#using <System.Data.dll>

namespace Smart { namespace SqlClr { 
	namespace Internal {
		ref class SqlObject;
	}

namespace Types {
	using namespace System;
	using namespace System::Data::SqlTypes;
	using namespace Microsoft::SqlServer::Server;

	ref class Enumerator;
	ref class List;

	/**
	 * @class TupleSet
	 *
	 * @brief Represents a collection of Tuples
	 *
	 * A tuple set can be treated very much like a table,
	 * providing the ability to store rows, create indexes, etc..
	 *
	 * @author Muhmud Ahmad
	 *
	 */
	[Serializable]
	[SqlUserDefinedType(Format::UserDefined, IsByteOrdered=false, MaxByteSize=-1, IsFixedLength=false)]
	public ref class TupleSet sealed : public Data::SqlTypes::INullable, IBinarySerialize {
	internal:
		typedef Collections::Generic::IList<Tuple^> SqlTuples;
		typedef Collections::Generic::IDictionary<Object^, int> SqlTupleUniqueIndex;
		typedef Collections::Generic::IList<int> SqlTupleIndexEntries;
		typedef Collections::Generic::IDictionary<Object^, SqlTupleIndexEntries^> SqlTupleIndex;
		typedef Collections::Generic::IDictionary<String^, SqlTupleUniqueIndex^> SqlTupleUniqueIndexes;
		typedef Collections::Generic::IDictionary<String^, SqlTupleIndex^> SqlTupleIndexes;

		TupleSet^ Copy(bool includeItems);

	private:
		// Literals
		literal System::String^ XML_ROOT_ELEMENT = "TupleSet";
		literal System::String^ XML_TUPLES_ROOT_ELEMENT = "Tuples";
		literal System::String^ XML_ISCOMPRESSED_ATTRIBUTE = "isCompressed";
		literal System::String^ XML_COLUMNS_ELEMENT = "Columns";
		literal System::String^ XML_COLUMN_ELEMENT = "Column";
		literal System::String^ XML_COLUMN_NAME_ATTRIBUTE = "name";
		literal System::String^ XML_COLUMN_TYPE_ATTRIBUTE = "type";
		literal System::String^ XML_COLUMN_KEY_ATTRIBUTE = "isKey";
		literal System::String^ XML_COLUMN_INDEX_ATTRIBUTE = "isIndex";

		// Static value for a null
		static initonly TupleSet^ NullTupleSet = create();

		// Typedefs
		typedef Collections::Generic::List<Tuple^> SqlTuplesImpl;
		typedef Collections::Generic::Dictionary<Object^, int> SqlTupleUniqueIndexImpl;
		typedef Collections::Generic::List<int> SqlTupleIndexEntriesImpl;
		typedef Collections::Generic::Dictionary<Object^, SqlTupleIndexEntries^> SqlTupleIndexImpl;
		typedef Collections::Generic::Dictionary<String^, SqlTupleUniqueIndex^> SqlTupleUniqueIndexesImpl;
		typedef Collections::Generic::Dictionary<String^, SqlTupleIndex^> SqlTupleIndexesImpl;

		typedef Collections::Generic::KeyValuePair<String^, SqlTupleUniqueIndex^> SqlTupleUniqueIndexesEntry;
		typedef Collections::Generic::KeyValuePair<String^, SqlTupleIndex^> SqlTupleIndexesEntry;

		// Internal Implementation
		Tuple::SqlTupleItems^ m_items;
		Tuple::SqlTupleTypes^ m_types;
		SqlTuples^ m_tuples;
		SqlTupleUniqueIndexes^ m_uniqueIndexes;
		SqlTupleIndexes^ m_indexes;
		Byte m_isNull;
		Byte m_isCompressed;
		Byte m_isColumnarCompressed;

		// Factory method(s)
		static TupleSet^ create();

		// Check functions
		void checkIndex(SqlInt32 position);
		void checkKeyValue(SqlString name, Object^ value);
		void checkIndexValue(SqlString name, Object^ value);
		void checkValue(String^ column, Object^ value);
		void checkTuple(Tuple^ tuple);

		// Methods to read header & body from stream
		void readHeader(IO::BinaryReader^ reader);
		void readBody(IO::BinaryReader^ reader);

		// Tuple Management
		void removeTuples(cli::array<int>^ positions);
		void removeTuple(int position);

	public:
		/**
		 * Gets a tuple based on a position value
		 *
		 * @param position The position to lookup the value on
		 *
		 * @returns The tuple associated with the specified position
		 *
		 */
		[SqlMethod(IsDeterministic=true, OnNullCall=false, IsPrecise=true, DataAccess=DataAccessKind::None)]
		Tuple^ Get(SqlInt32 position);

		/**
		 * Gets a tuple based on an unique key value
		 *
		 * @param keyName The name of the key to lookup the value on
		 * @param keyValue The key to lookup the value on
		 *
		 * @returns The tuple associated with the specified key
		 *
		 */
		[SqlMethod(IsDeterministic=true, OnNullCall=false, IsPrecise=true, DataAccess=DataAccessKind::None)]
		Tuple^ GetByKey(SqlString keyName, Object^ keyValue);

		/**
		 * Gets a tuple based on an index value
		 *
		 * @param indexName The name of the index to lookup the value on
		 * @param indexValue The key to lookup the value on
		 *
		 * @returns The tuples associated with the specified key
		 *
		 */
		[SqlMethod(IsDeterministic=true, OnNullCall=false, IsPrecise=true, DataAccess=DataAccessKind::None)]
		TupleSet^ GetByIndex(SqlString indexName, Object^ indexValue);

		/**
		 * Adds a tuple to the set
		 *
		 * @param tuple The tuple to be added
		 *
		 */
		[SqlMethod(IsDeterministic=true, OnNullCall=false, IsPrecise=true, DataAccess=DataAccessKind::None, IsMutator=true)]
		void AddTuple(Tuple^ tuple);

		/**
		 * Removes a tuple from the set based on a position value
		 *
		 * @param position The position value to search on
		 *
		 */
		[SqlMethod(IsDeterministic=true, OnNullCall=false, IsPrecise=true, DataAccess=DataAccessKind::None, IsMutator=true)]
		void Remove(SqlInt32 position);

		/**
		 * Removes a tuple from the set based on an unique index key value
		 *
		 * @param keyName Name of the key to lookup on
		 * @param keyValue The value to search on
		 *
		 */
		[SqlMethod(IsDeterministic=true, IsPrecise=true, DataAccess=DataAccessKind::None, IsMutator=true)]
		void RemoveByKey(SqlString keyName, Object^ KeyValue);

		/**
		 * Removes all tuples from the set based on an index value
		 *
		 * @param indexName Name of the key to lookup on
		 * @param indexValue The value to search on
		 *
		 */
		[SqlMethod(IsDeterministic=true, IsPrecise=true, DataAccess=DataAccessKind::None, IsMutator=true)]
		void RemoveByIndex(SqlString indexName, Object^ indexValue);

		/**
		 * Sets whether the TupleSet is compressed or not
		 *
		 * @param compressed True if the TupleSet should be compressed, otherwise false
		 *
		 */
		[SqlMethod(IsDeterministic=true, IsPrecise=true, DataAccess=DataAccessKind::None, IsMutator=true)]
		void SetCompression(SqlBoolean compressed);
		
		/**
		 * Sets whether the TupleSet is compressed with columnar compression
		 *
		 * @param compressed True if the TupleSet should be columnar compressed, otherwise false
		 *
		 */
		[SqlMethod(IsDeterministic=true, IsPrecise=true, DataAccess=DataAccessKind::None, IsMutator=true)]
		void SetColumnarCompression(SqlBoolean compressed);


		// Whether or not the TupleSet is compressed
		property SqlBoolean IsCompressed {
			SqlBoolean get();
		}

		// Whether or not the TupleSet is using Columnar Compression
		property SqlBoolean IsColumnarCompressed {
			SqlBoolean get();
		}

		/// Returns the number of tuples in the collection
		property SqlInt32 Count {
			SqlInt32 get();
		}

		/// Returns an Enumerator for the tuples in the collection
		property Enumerator^ Tuples {
			Enumerator^ get();
		}

		/// Returns a List for the columns in the Tuples
		property List^ Columns {
			List^ get();
		}

		/// Returns a List for the types in the Tuples
		property List^ Types {
			List^ get();
		}

		/// The IsNull property; necessary for SqlClr types
		property bool IsNull {
			virtual bool get();
		}

		/// A static property for the null value; necessary for SqlClr types
		static property TupleSet^ Null {
			TupleSet^ get();
		}

		/// Converts the current instance to a string representation; necessary for SqlClr types
		virtual String^ ToString() override;

		/**
		 * Parses a string value to create an instance of the type
		 *
		 * Necessary for SqlClr types
		 *
		 * @param sqlValue The value to be parsed
		 *
		 * @returns A List object
		 *
		 */
		[SqlMethod(IsDeterministic=true, OnNullCall = false, DataAccess=DataAccessKind::None)]
		static TupleSet^ Parse(SqlString sqlValue);

		/**
		 * Parses a string value to create an instance of the type
		 *
		 * Necessary for SqlClr types
		 *
		 * @param sqlValue The value to be parsed
		 *
		 * @returns A List object
		 *
		 */
		[SqlMethod(IsDeterministic=true, OnNullCall = false, DataAccess=DataAccessKind::None)]
		static TupleSet^ ParseTuples(SqlString sqlValue);

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

#endif // _SMART_SQLCLR_TYPES__TUPLESET_HPP_
