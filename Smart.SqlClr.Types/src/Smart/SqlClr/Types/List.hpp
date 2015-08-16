
#ifndef _SMART_SQLCLR_TYPES__LIST_HPP_
#define _SMART_SQLCLR_TYPES__LIST_HPP_

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
	ref class Variant;

	/**
	 * @class List
	 *
	 * @brief List data type for use with SQL Server
	 *
	 * This data type can be used to store an array
	 * within SQL Server.
	 *
	 * @author Muhmud Ahmad
	 *
	 */
	[Serializable]
	[SqlUserDefinedType(Format::UserDefined, IsByteOrdered=false, MaxByteSize=-1, IsFixedLength=false)]
	public ref class List sealed : public Data::SqlTypes::INullable, IBinarySerialize {
	internal:
		// Typedefs
		typedef Collections::IList SqlList;

	private:
		// Literals
		literal System::String^ XML_ROOT_ELEMENT = "List";
		literal System::String^ XML_VALUETYPE_ATTRIBUTE = "valueType";
		literal System::String^ XML_ISCOMPRESSED_ATTRIBUTE = "isCompressed";
		literal System::String^ XML_ISSORTED_ATTRIBUTE = "isSorted";
		literal System::String^ XML_ITEM_ELEMENT = "Item";

		// Static value for a null
		static initonly List^ NullList = create();

		// Typedefs
		typedef Collections::ArrayList SqlListImpl;

		// Internal Implementation
		SqlList^ m_sqlList;
		Internal::SqlObject^ m_valueType;
		Byte m_isNull;
		Byte m_isCompressed;
		Byte m_isSorted;
		Byte m_isReadOnly;

		// Factory method(s)
		static List^ create();
		static List^ create(String^ valueType);
		static List^ create(Type^ sqlDataType);

		// Checks whether supplied values match types
		void checkTypes(Object^ value);
		void checkTypes(List^ list);
		bool checkTypes(List^ list, bool noThrow);
		void checkVariantTypes();
		void checkNonVariantTypes();

		// Methods to read header & body from stream
		void readHeader(IO::BinaryReader^ reader);
		void readBody(IO::BinaryReader^ reader);

	internal:
		// Internal access functions
		SqlList^ getSqlList();
		Internal::SqlObject^ getValueType();

		// Clone function
		List^ Copy(bool includeItems);

		// Create a new list from a Sql data type
		static List^ NewOfType(Type^ sqlDataType);

	public:
		/**
		 * Creates a new List of sql_variant
		 *
		 */
		[SqlMethod(IsDeterministic=true, IsPrecise=true, DataAccess=DataAccessKind::None)]
		static List^ New();

		/**
		 * Creates a new List of Variant
		 *
		 */
		[SqlMethod(IsDeterministic=true, IsPrecise=true, DataAccess=DataAccessKind::None)]
		static List^ NewOfVariant();

		/**
		 * Creates a new List of the specified type
		 *
		 */
		[SqlMethod(IsDeterministic=true, IsPrecise=true, OnNullCall=false, DataAccess=DataAccessKind::None)]
		static List^ NewOf(SqlString valueType);

		/**
		 * Adds an entry to the List
		 *
		 * @param value The value of the entry to be added
		 *
		 */
		[SqlMethod(IsDeterministic=true, IsPrecise=true, DataAccess=DataAccessKind::None, IsMutator=true)]
		void AddItem(Object^ value);

		/**
		 * Adds a Variant entry to the List
		 *
		 * @param value The list value to be added
		 *
		 */
		[SqlMethod(IsDeterministic=true, IsPrecise=true, DataAccess=DataAccessKind::None, IsMutator=true)]
		void AddVariant(Variant^ value);

		/**
		 * Adds an entry to the list if it does not already exist
		 *
		 * @param value The value of the entry to be added
		 *
		 */
		[SqlMethod(IsDeterministic=true, IsPrecise=true, DataAccess=DataAccessKind::None, IsMutator=true)]
		void AddIfNotExists(Object^ value);

		/**
		 * Inserts an item into the list at a specified index
		 *
		 * @param index The index at which to insert the item
		 * @param value The value to be inserted
		 *
		 */
		[SqlMethod(IsDeterministic=true, IsPrecise=true, DataAccess=DataAccessKind::None, IsMutator=true)]
		void InsertAt(int index, Object^ value);

		/**
		 * Inserts a Variant item into the list at a specified index
		 *
		 * @param index The index at which to insert the item
		 * @param value The value to be inserted
		 *
		 */
		[SqlMethod(IsDeterministic=true, IsPrecise=true, DataAccess=DataAccessKind::None, IsMutator=true)]
		void InsertVariantAt(int index, Variant^ value);

		/**
		 * Removes an entry from the list
		 *
		 * @param value The value of the entry to be removed
		 *
		 */
		[SqlMethod(IsDeterministic=true, IsPrecise=true, DataAccess=DataAccessKind::None, IsMutator=true)]
		void Remove(Object^ value);

		/**
		 * Removes an entry from the list if it exists
		 *
		 * @param value The value of the entry to be removed
		 *
		 */
		[SqlMethod(IsDeterministic=true, IsPrecise=true, DataAccess=DataAccessKind::None, IsMutator=true)]
		void RemoveIfExists(Object^ value);

		/**
		 * Removes all items from the list relating to a value
		 *
		 * @param value The value of the entry to be removed
		 *
		 */
		[SqlMethod(IsDeterministic=true, IsPrecise=true, DataAccess=DataAccessKind::None, IsMutator=true)]
		void RemoveAll(Object^ value);

		/**
		 * Removes an entry from the list relating to an index
		 *
		 * @param index The index of the entry to be removed
		 *
		 */
		[SqlMethod(IsDeterministic=true, IsPrecise=true, DataAccess=DataAccessKind::None, IsMutator=true)]
		void RemoveAt(int index);

		/**
		 * Returns the index of an item in the list
		 *
		 * @param value The value for which the index should be returned
		 *
		 * @returns An index value
		 *
		 */
		[SqlMethod(IsDeterministic=true, IsPrecise=true, DataAccess=DataAccessKind::None)]
		SqlInt32 IndexOf(Object^ value);

		/**
		 * Checks if the list contains the specified value
		 *
		 * @param value The value to check for
		 *
		 * @returns True if an entry exists in the Array for the value, otherwise false
		 *
		 */
		[SqlMethod(IsDeterministic=true, IsPrecise=true, DataAccess=DataAccessKind::None)]
		SqlBoolean ContainsValue(Object^ value);

		/**
		 * Adds entries to the current list from the one specified
		 *
		 * @param a The list for which values should be added
		 *
		 */
		[SqlMethod(IsDeterministic=true, OnNullCall=false, IsPrecise=true, DataAccess=DataAccessKind::None, IsMutator=true)]
		void AddFromList(List^ list);

		/**
		 * Clears all entries from the list
		 *
		 */
		[SqlMethod(IsDeterministic=true, IsPrecise=true, DataAccess=DataAccessKind::None, IsMutator=true)]
		void Clear();

		/**
		 * Sorts the items in the list using quicksort
		 *
		 */
		[SqlMethod(IsDeterministic=false, IsPrecise=true, DataAccess=DataAccessKind::None, IsMutator=true)]
		void Sort();

		/**
		 * Clones the list to create a separate copy
		 *
		 * @returns A clone of the list
		 *
		 */
		[SqlMethod(IsDeterministic=true, IsPrecise=true, DataAccess=DataAccessKind::None)]
		List^ Clone();

		/**
		 * Sets the state of the list to be compressed or not
		 *
		 * @param compressed True if the list should be compressed, otherwise false
		 *
		 */
		[SqlMethod(IsDeterministic=true, OnNullCall=false, IsPrecise=true, DataAccess=DataAccessKind::None, IsMutator=true)]
		void SetCompression(SqlBoolean compressed);

		/**
		 * Returns whether one list equals another
		 *
		 * @param a The list to compare the current instance to
		 *
		 * @returns True if the two lists are of the same type and with the same data items, otherwise false
		 *
		 */
		[SqlMethod(IsDeterministic=true, IsPrecise=true, DataAccess=DataAccessKind::None)]
		SqlBoolean Equals(List^ list);

		/**
		 * Gets a value from the list based on a specified index
		 *
		 * @param index The index to lookup the value on
		 *
		 * @returns The value associated with the specified index
		 *
		 */
		[SqlMethod(IsDeterministic=true, IsPrecise=true, DataAccess=DataAccessKind::None)]
		Object^ Get(int index);

		/**
		 * Gets a Variant value from the array based on a specified index
		 *
		 * @param index The index to lookup the value on
		 *
		 * @returns The value associated with the specified index
		 *
		 */
		[SqlMethod(IsDeterministic=true, IsPrecise=true, DataAccess=DataAccessKind::None)]
		Variant^ GetVariant(int index);


		/// Gets the values
		property Enumerator^ Items {
			Enumerator^ get();
		}

		/// The value type for the collection
		property SqlString ValueType {
			SqlString get();
		}

		/// The number of items in the Array
		property SqlInt32 Count {
			SqlInt32 get();
		}

		/// Whether or not the Array is compressed
		property SqlBoolean IsCompressed {
			SqlBoolean get();
		}

		/// The IsNull property; necessary for SqlClr types
		property bool IsNull {
			virtual bool get();
		}

		/// Returns whether the dictionary stores variant objects
		property SqlBoolean IsVariant {
			SqlBoolean get();
		}

		/// Returns whether the array is sorted or not
		property SqlBoolean IsSorted {
			SqlBoolean get();
		}
		
		/// A static property for the null value; necessary for SqlClr types
		static property List^ Null {
			List^ get();
		}

		/// Converts the current instance to a string representation; necessary for SqlClr types
		virtual String^ ToString() override;

		/**
		 * Outputs the contents of the list as a simple string
		 *
		 * @returns An easy to read string
		 *
		 */
		[SqlMethod(IsDeterministic=true, DataAccess=DataAccessKind::None)]
		String^ ToSimpleString();

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
		static List^ Parse(SqlString sqlValue);

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

#endif // _SMART_SQLCLR_TYPES__LIST_HPP_
