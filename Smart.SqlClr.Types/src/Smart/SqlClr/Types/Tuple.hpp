
#ifndef _SMART_SQLCLR_TYPES__TUPLE_HPP_
#define _SMART_SQLCLR_TYPES__TUPLE_HPP_

#using <System.dll>
#using <System.Data.dll>
#using <System.Xml.dll>

namespace Smart { namespace SqlClr { 
	namespace Internal {
		ref class SqlObject;
	}

namespace Types {
	using namespace System;
	using namespace System::Data::SqlTypes;
	using namespace Microsoft::SqlServer::Server;

	ref class List;

	/**
	 * @class Tuple
	 *
	 * @brief Represents a Tuple of sql_variant compatible types
	 *
	 * This data type can be used to store a row of data
	 * within a SqlClr type.
	 *
	 * @author Muhmud Ahmad
	 *
	 */
	[Serializable]
	[SqlUserDefinedType(Format::UserDefined, IsByteOrdered=false, MaxByteSize=-1, IsFixedLength=false)]
	public ref class Tuple sealed : public Data::SqlTypes::INullable, IBinarySerialize {
	internal:
		typedef Collections::IList SqlTupleValues;
		typedef System::Int64 SqlTupleNulls;
		typedef Collections::Generic::IDictionary<String^, int> SqlTupleItems;
		typedef Collections::Generic::KeyValuePair<String^, int> SqlTupleItemEntry;
		typedef Collections::Generic::IList<Internal::SqlObject^> SqlTupleTypes;

		typedef Collections::Generic::Dictionary<String^, int> SqlTupleItemsImpl;
		typedef Collections::Generic::List<Internal::SqlObject^> SqlTupleTypesImpl;

		static Tuple^ Create(SqlTupleItems^ items, SqlTupleTypes^ types, IO::BinaryReader^ reader);
		static Tuple^ Create(SqlTupleItems^ items, SqlTupleTypes^ types, Xml::XmlReader^ reader);
		static Tuple^ Create(SqlTupleItems^ items, SqlTupleTypes^ types);
		static Tuple^ Create();

		Tuple^ Copy();

		void ReadXml(Xml::XmlReader^ reader, bool includeMetadata);
		void WriteXml(Xml::XmlWriter^ writer, bool includeMetadata);

		// Access functions
		SqlTupleItems^ GetItems();
		SqlTupleTypes^ GetTypes();

		// Methods to read header & body from stream
		void readHeader(IO::BinaryReader^ reader);
		void readBody(IO::BinaryReader^ reader, SqlInt32 column);

		// Methods to write header & body to stream
		void writeHeader(IO::BinaryWriter^ reader);
		void writeBody(IO::BinaryWriter^ reader, SqlInt32 column);

	private:
		// Literals
		literal String^ XML_ROOT_ELEMENT = "Tuple";
		literal String^ XML_TYPE_ATTRIBUTE = "type";
		literal String^ XML_ISNULL_ATTRIBUTE = "isNull";

		// Static value for a null
		static initonly Tuple^ NullTuple = create();

		// Typedefs
		typedef Collections::ArrayList SqlTupleValuesImpl;
		typedef System::Int64 SqlTupleNullsImpl;

		// Internal Implementation
		SqlTupleValues^ m_values;
		SqlTupleNulls m_nulls;
		SqlTupleItems^ m_items;
		SqlTupleTypes^ m_types;
		Byte m_isNull;

		// Factory method(s)
		static Tuple^ create();

		// Check functions
		void checkIndex(SqlInt32 index);
		void checkName(SqlString name);
		void checkType(int index, Object^ value);

		// Null handling
		bool isNull(int index);
		void setNull(int index, bool yes);

	public:
		/**
		 * Gets a value from the tuple based on a specified index
		 *
		 * @param index The index to lookup the value on
		 *
		 * @returns The value associated with the specified index
		 *
		 */
		[SqlMethod(IsDeterministic=true, OnNullCall=false, IsPrecise=true, DataAccess=DataAccessKind::None)]
		Object^ Get(SqlInt32 index);

		/**
		 * Gets a value from the tuple based on a specified name
		 *
		 * @param name The name to lookup the value on
		 *
		 * @returns The value associated with the specified name
		 *
		 */
		[SqlMethod(IsDeterministic=true, OnNullCall=false, IsPrecise=true, DataAccess=DataAccessKind::None)]
		Object^ GetByName(SqlString name);

		/**
		 * Sets a value in the tuple based on a specified index
		 *
		 * @param index The index of the value to update
		 * @param value The new value
		 *
		 */
		[SqlMethod(IsDeterministic=true, IsMutator=true, IsPrecise=true, DataAccess=DataAccessKind::None)]
		void SetByID(SqlInt32 index, Object^ value);

		/**
		 * Sets a value in the tuple based on a specified name
		 *
		 * @param name The name of the value to update
		 * @param value The new value
		 *
		 */
		[SqlMethod(IsDeterministic=true, IsMutator=true, IsPrecise=true, DataAccess=DataAccessKind::None)]
		void SetByName(SqlString name, Object^ value);


		/// The IsNull property; necessary for SqlClr types
		property bool IsNull {
			virtual bool get();
		}

		/// A static property for the null value; necessary for SqlClr types
		static property Tuple^ Null {
			Tuple^ get();
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
		static Tuple^ Parse(SqlString sqlValue);

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
#endif // _SMART_SQLCLR_TYPES__TUPLE_HPP_