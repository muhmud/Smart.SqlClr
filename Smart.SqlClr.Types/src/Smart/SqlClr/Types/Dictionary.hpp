
#ifndef _SMART_SQLCLR_TYPES__DICTIONARY_HPP_
#define _SMART_SQLCLR_TYPES__DICTIONARY_HPP_

#using <System.dll>
#using <System.Data.dll>

namespace Smart { namespace SqlClr { 
	namespace Internal {
		ref class SqlObject;
	}

namespace Types {
	ref class Enumerator;

	using namespace System;
	using namespace System::Data::SqlTypes;
	using namespace Microsoft::SqlServer::Server;

	/**
	 * @class Dictionary
	 *
	 * @brief Dictionary data type for use with SQL Server
	 *
	 * This data type can be used to store a dictionary
	 * within SQL Server.
	 *
	 * @author Muhmud Ahmad
	 *
	 */
	[Serializable]
	[SqlUserDefinedType(Format::UserDefined, IsByteOrdered=false, MaxByteSize=-1, IsFixedLength=false)]
	public ref class Dictionary sealed : public Data::SqlTypes::INullable, IBinarySerialize { 
	internal:
		// Typedefs
		typedef Collections::IDictionary SqlDictionary;
		typedef Collections::DictionaryEntry SqlDictionaryEntry;

	private:
		// Literals
		literal System::String^ XML_ROOT_ELEMENT = "Dictionary";
		literal System::String^ XML_KEYTYPE_ATTRIBUTE = "keyType";
		literal System::String^ XML_VALUETYPE_ATTRIBUTE = "valueType";
		literal System::String^ XML_ISCOMPRESSED_ATTRIBUTE = "isCompressed";
		literal System::String^ XML_ENTRY_ELEMENT = "Entry";
		literal System::String^ XML_ENTRY_KEY_ATTRIBUTE = "key";

		// Typedefs
		typedef Collections::Hashtable SqlDictionaryImpl;
		typedef Collections::SortedList SqlSortedDictionaryImpl;

		// Static value for a null
		static initonly Dictionary^ NullDictionary = create();

		// Internal Implementation
		SqlDictionary^ m_sqlDictionary;
		Internal::SqlObject^ m_keyType;
		Internal::SqlObject^ m_valueType;
		Byte m_isNull;
		Byte m_isSorted;
		Byte m_isCompressed;

		// Factory method(s)
		static Dictionary^ create();
		static Dictionary^ create(String^ keyType, String^ valueType, bool sorted);
		static Dictionary^ create(Type^ keyType, Type^ valueType, bool sorted);

		// Checks whether supplied values match types
		void checkTypes(Object^ key, Object^ value);
		void checkTypes(Dictionary^ dictionary);
		bool checkTypes(Dictionary^ dictionary, bool noThrow);

		void checkNestedTypes(Object^ key);

		// Methods to read header & body from stream
		void readHeader(IO::BinaryReader^ reader);
		void readBody(IO::BinaryReader^ reader);

	internal:
		// Internal access functions
		SqlDictionary^ getSqlDictionary();
		Internal::SqlObject^ getKeyType();
		Internal::SqlObject^ getValueType();

		// Clone function
		Dictionary^ Copy(bool includeItems, bool useSortedImpl);

		// Create a new dictionary from Sql data type(s)
		static Dictionary^ NewOfType(Type^ keySqlDataType);
		static Dictionary^ NewOfType(Type^ keySqlDataType, Type^ valueSqlDataType);
		static Dictionary^ NewSortedOfType(Type^ keySqlDataType);
		static Dictionary^ NewSortedOfType(Type^ keySqlDataType, Type^ valueSqlDataType);

	public:
		/**
		 * Creates a new Dictionary of string to sql_variant
		 *
		 */
		[SqlMethod(IsDeterministic=true, IsPrecise=true, DataAccess=DataAccessKind::None)]
		static Dictionary^ New();

		/**
		 * Creates a new Dictionary of string to dictionary
		 *
		 */
		[SqlMethod(IsDeterministic=true, IsPrecise=true, DataAccess=DataAccessKind::None)]
		static Dictionary^ NewNested();

		/**
		 * Creates a new Dictionary using the specified types
		 *
		 */
		[SqlMethod(IsDeterministic=true, IsPrecise=true, OnNullCall=false, DataAccess=DataAccessKind::None)]
		static Dictionary^ NewOf(SqlString keyType, SqlString valueType);

		/**
		 * Creates a new Dictionary using the specified key and where the values are dictionaries
		 *
		 */
		[SqlMethod(IsDeterministic=true, IsPrecise=true, OnNullCall=false, DataAccess=DataAccessKind::None)]
		static Dictionary^ NewNestedOf(SqlString keyType);

		/**
		 * Creates a new sorted Dictionary using the specified types
		 *
		 */
		[SqlMethod(IsDeterministic=true, IsPrecise=true, OnNullCall=false, DataAccess=DataAccessKind::None)]
		static Dictionary^ NewSortedOf(SqlString keyType, SqlString valueType);

		/**
		 * Creates a new Dictionary using the specified key and where the values are dictionaries
		 *
		 */
		[SqlMethod(IsDeterministic=true, IsPrecise=true, OnNullCall=false, DataAccess=DataAccessKind::None)]
		static Dictionary^ NewSortedAndNestedOf(SqlString keyType);

		/**
		 * Adds an entry to the dictionary
		 *
		 * @param key The key of the entry to be added
		 * @param value The value of the entry to be added
		 *
		 */
		[SqlMethod(IsDeterministic=true, IsPrecise=true, DataAccess=DataAccessKind::None, IsMutator=true)]
		void AddEntry(Object^ key, Object^ value);

		/**
		 * Adds an dictionary entry to the dictionary
		 *
		 * @param key The key of the entry to be added
		 * @param value The dictionary value of the entry to be added
		 *
		 */
		[SqlMethod(IsDeterministic=true, IsPrecise=true, DataAccess=DataAccessKind::None, IsMutator=true)]
		void AddNestedEntry(Object^ key, Dictionary^ value);

		/**
		 * Adds an entry to the dictionary if it does not already exist
		 *
		 * @param key The key of the entry to be added
		 * @param value The value of the entry to be added
		 *
		 */
		[SqlMethod(IsDeterministic=true, IsPrecise=true, DataAccess=DataAccessKind::None, IsMutator=true)]
		void AddEntryIfNotExists(Object^ key, Object^ value);

		/**
		 * Adds an dictionary entry to the dictionary if it does not already exist
		 *
		 * @param key The key of the entry to be added
		 * @param value The value of the entry to be added
		 *
		 */
		[SqlMethod(IsDeterministic=true, IsPrecise=true, DataAccess=DataAccessKind::None, IsMutator=true)]
		void AddNestedEntryIfNotExists(Object^ key, Dictionary^ value);

		/**
		 * Updates an entry in the dictionary
		 *
		 * @param key The key of the entry to be updated
		 * @param newValue The new value for the entry
		 *
		 */
		[SqlMethod(IsDeterministic=true, IsPrecise=true, DataAccess=DataAccessKind::None, IsMutator=true)]
		void UpdateEntry(Object^ key, Object^ newValue);

		/**
		 * Updates a dictionary entry in the dictionary
		 *
		 * @param key The key of the entry to be updated
		 * @param newValue The new value for the entry
		 *
		 */
		[SqlMethod(IsDeterministic=true, IsPrecise=true, DataAccess=DataAccessKind::None, IsMutator=true)]
		void UpdateNestedEntry(Object^ key, Dictionary^ newValue);

		/**
		 * Updates an entry in the dictionary if it exists
		 *
		 * @param key The key of the entry to be updated
		 * @param newValue The new value for the entry
		 *
		 */
		[SqlMethod(IsDeterministic=true, IsPrecise=true, DataAccess=DataAccessKind::None, IsMutator=true)]
		void UpdateEntryIfExists(Object^ key, Object^ newValue);

		/**
		 * Updates a dictionary entry in the dictionary if it exists
		 *
		 * @param key The key of the entry to be updated
		 * @param newValue The new value for the entry
		 *
		 */
		[SqlMethod(IsDeterministic=true, IsPrecise=true, DataAccess=DataAccessKind::None, IsMutator=true)]
		void UpdateNestedEntryIfExists(Object^ key, Dictionary^ newValue);

		/**
		 * Merges an entry into the dictionary
		 *
		 * @param key The key of the entry to be inserted/updated
		 * @param value The value for the entry
		 *
		 */
		[SqlMethod(IsDeterministic=true, IsPrecise=true, DataAccess=DataAccessKind::None, IsMutator=true)]
		void MergeEntry(Object^ key, Object^ value);

		/**
		 * Merges a dictionary entry into the dictionary
		 *
		 * @param key The key of the entry to be inserted/updated
		 * @param value The value for the entry
		 *
		 */
		[SqlMethod(IsDeterministic=true, IsPrecise=true, DataAccess=DataAccessKind::None, IsMutator=true)]
		void MergeNestedEntry(Object^ key, Dictionary^ value);

		/**
		 * Removes an entry from the dictionary
		 *
		 * @param key The key of the entry to be removed
		 *
		 */
		[SqlMethod(IsDeterministic=true, IsPrecise=true, DataAccess=DataAccessKind::None, IsMutator=true)]
		void RemoveEntry(Object^ key);

		/**
		 * Removes an entry from the dictionary if it exists
		 *
		 * @param key The key of the entry to be removed
		 *
		 */
		[SqlMethod(IsDeterministic=true, IsPrecise=true, DataAccess=DataAccessKind::None, IsMutator=true)]
		void RemoveEntryIfExists(Object^ key);
		
		/**
		 * Checks if the dictionary contains the specified key
		 *
		 * @param key The key to check for
		 *
		 * @returns True if an entry exists in the Dictionary for the key, otherwise false
		 *
		 */
		[SqlMethod(IsDeterministic=true, IsPrecise=true, DataAccess=DataAccessKind::None)]
		SqlBoolean ContainsKey(Object^ key);

		/**
		 * Adds entries to the current dictionary from the one specified if they don't already exist
		 *
		 * @param dictionary The dictionary for which values should be added
		 *
		 */
		[SqlMethod(IsDeterministic=true, OnNullCall=false, IsPrecise=true, DataAccess=DataAccessKind::None, IsMutator=true)]
		void AddDictionary(Dictionary^ dictionary);

		/**
		 * Adds items from another dictionary object. Duplicates will cause a failure
		 *
		 * @param dictionary The dictionary to be combined with the current instance
		 *
		 */
		[SqlMethod(IsDeterministic=true, OnNullCall=false, IsPrecise=true, DataAccess=DataAccessKind::None, IsMutator=true)]
		void AddFromDictionary(Dictionary^ dictionary);

		/**
		 * Updates entries in the current dictionary from the one specified if they exist
		 *
		 * @param dictionary The dictionary for which values should be updated
		 *
		 */
		[SqlMethod(IsDeterministic=true, OnNullCall=false, IsPrecise=true, DataAccess=DataAccessKind::None, IsMutator=true)]
		void UpdateDictionary(Dictionary^ dictionary);

		/**
		 * Removes entries in the current dictionary based on the one specified if they exist
		 *
		 * @param dictionary The dictionary to be used a the basis of entries to be removed
		 *
		 */
		[SqlMethod(IsDeterministic=true, OnNullCall=false, IsPrecise=true, DataAccess=DataAccessKind::None, IsMutator=true)]
		void RemoveDictionary(Dictionary^ dictionary);

		/**
		 * Merges entries into the current dictionary from the one specified
		 *
		 * @param dictionary The dictionary to be merged
		 *
		 */
		[SqlMethod(IsDeterministic=true, OnNullCall=false, IsPrecise=true, DataAccess=DataAccessKind::None, IsMutator=true)]
		void MergeDictionary(Dictionary^ dictionary);

		/**
		 * Clears all entries from the dictionary
		 *
		 */
		[SqlMethod(IsDeterministic=true, IsPrecise=true, DataAccess=DataAccessKind::None, IsMutator=true)]
		void Clear();

		/**
		 * Clones the dictionary to create a separate copy
		 *
		 * @returns A clone of the dictionary
		 *
		 */
		[SqlMethod(IsDeterministic=true, IsPrecise=true, DataAccess=DataAccessKind::None)]
		Dictionary^ Clone();

		/**
		 * Sets the state of the map to be compressed or not
		 *
		 * @param compressed True if the map should be compressed, otherwise false
		 *
		 */
		[SqlMethod(IsDeterministic=true, OnNullCall=false, IsPrecise=true, DataAccess=DataAccessKind::None, IsMutator=true)]
		void SetCompression(SqlBoolean compressed);

		/**
		 * Returns whether one Dictionary equals another
		 *
		 * @param dictionary The dictionary to compare the current instance to
		 *
		 * @returns True if the two dictionaries are of the same type and with the same data items, otherwise false
		 *
		 */
		[SqlMethod(IsDeterministic=true, OnNullCall=false, IsPrecise=true, DataAccess=DataAccessKind::None, IsMutator=false)]
		SqlBoolean Equals(Dictionary^ dictionary);

		/**
		 * Gets a value from the Dictionary based on a specified key
		 *
		 * @param key The key to lookup the value on
		 *
		 * @returns The value associated with the specified key
		 *
		 */
		[SqlMethod(IsDeterministic=true, IsPrecise=true, DataAccess=DataAccessKind::None)]
		Object^ Get(Object^ key);

		/**
		 * Gets a value from the Dictionary based on a specified key if it exists
		 *
		 * @param key The key to lookup the value on
		 *
		 * @returns The value associated with the specified key, or null
		 *
		 */
		[SqlMethod(IsDeterministic=true, IsPrecise=true, DataAccess=DataAccessKind::None)]
		Object^ GetIfExists(Object^ key);

		/**
		 * Gets a dictionary value from the Dictionary based on a specified key
		 *
		 * @param key The key to lookup the value on
		 *
		 * @returns The value associated with the specified key
		 *
		 */
		[SqlMethod(IsDeterministic=true, IsPrecise=true, DataAccess=DataAccessKind::None)]
		Dictionary^ GetDictionary(Object^ key);

		/**
		 * Gets a dictionary value from the Dictionary based on a specified key if it exists
		 *
		 * @param The key to lookup the value on
		 *
		 * @returns The value associated with the specified key, or null
		 *
		 */
		[SqlMethod(IsDeterministic=true, IsPrecise=true, DataAccess=DataAccessKind::None)]
		Dictionary^ GetDictionaryIfExists(Object^ key);

		/// Gets the keys
		property Enumerator^ Keys {
			Enumerator^ get();
		}

		/// Gets the values
		property Enumerator^ Values {
			Enumerator^ get();
		}

		/// The key type for the collection
		property SqlString KeyType {
			SqlString get();
		}

		/// The value type for the collection
		property SqlString ValueType {
			SqlString get();
		}

		/// The number of items in the Dictionary
		property SqlInt32 Count {
			SqlInt32 get();
		}

		/// Whether or not the Dictionary is compressed
		property SqlBoolean IsCompressed {
			SqlBoolean get();
		}

		/// Whether or not the Dictionary is sorted
		property SqlBoolean IsSorted {
			SqlBoolean get();
		}

		/// The IsNull property; necessary for SqlClr types
		property bool IsNull {
			virtual bool get();
		}

		/// Returns whether the dictionary stores nested dictionaries
		property SqlBoolean IsNested {
			SqlBoolean get();
		}

		/// A static property for the null value; necessary for SqlClr types
		static property Dictionary^ Null {
			Dictionary^ get();
		}

		/// Converts the current instance to a string representation; necessary for SqlClr types
		virtual String^ ToString() override;

		/**
		* Outputs the contents of the list as a simple string
		*
		* @returns An easy to read string
		*
		*/
		[SqlMethod(IsDeterministic = true, DataAccess = DataAccessKind::None)]
		String^ ToSimpleString();

		/**
		 * Parses a string value to create an instance of the type
		 *
		 * Necessary for SqlClr types
		 *
		 * @param value The value to be parsed
		 *
		 * @returns A Dictionary object
		 *
		 */
		[SqlMethod(IsDeterministic=true, OnNullCall = false, DataAccess=DataAccessKind::None)]
		static Dictionary^ Parse(SqlString sqlValue);

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

#endif // _SMART_SQLCLR_TYPES__DICTIONARY_HPP_
