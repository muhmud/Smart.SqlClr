
#ifndef _SMART_SQLCLR_TYPES__GRAPH_HPP_
#define _SMART_SQLCLR_TYPES__GRAPH_HPP_

#using <System.dll>
#using <System.Data.dll>
#using <Microsoft.SqlServer.Types.dll>

namespace Smart { namespace SqlClr { 
	namespace Internal {
		ref class SqlObject;
	}

namespace Types {
	ref class Enumerator;
	ref class Variant;

	using namespace System;
	using namespace System::Data::SqlTypes;
	using namespace Microsoft::SqlServer::Server;
	using namespace Microsoft::SqlServer::Types;

	/**
	 * @class Graph
	 *
	 * @brief Graph data type for use with SQL Server
	 *
	 * This data type can be used to store a special type 
	 * of dictionary within SQL Server, that represents a 
	 * collection of hierarchyid nodes and data related
	 * to them.
	 *
	 * @author Muhmud Ahmad
	 *
	 */
	[Serializable]
	[SqlUserDefinedType(Format::UserDefined, IsByteOrdered=false, MaxByteSize=-1, IsFixedLength=false)]
	public ref class Graph sealed : public Data::SqlTypes::INullable, IBinarySerialize { 
	internal:
		// Typedefs
		typedef Collections::Generic::IDictionary<SqlHierarchyId, Object^> SqlGraph;
		typedef Collections::Generic::KeyValuePair<SqlHierarchyId, Object^> SqlGraphEntry;

	private:
		// Literals
		literal System::String^ XML_ROOT_ELEMENT = "Graph";
		literal System::String^ XML_VALUETYPE_ATTRIBUTE = "valueType";
		literal System::String^ XML_ISCOMPRESSED_ATTRIBUTE = "isCompressed";
		literal System::String^ XML_ENTRY_ELEMENT = "Entry";
		literal System::String^ XML_ENTRY_KEY_ATTRIBUTE = "key";

		// Typedefs
		typedef Collections::Generic::SortedDictionary<SqlHierarchyId, Object^> SqlGraphImpl;

		// Static value for a null
		static initonly Graph^ NullGraph = create();

		// Internal Implementation
		SqlGraph^ m_sqlGraph;
		Internal::SqlObject^ m_valueType;
		Byte m_isNull;
		Byte m_isCompressed;

		// Factory method(s)
		static Graph^ create();
		static Graph^ create(String^ valueType);

		// Checks whether supplied values match types
		void checkTypes(Object^ value);
		void checkTypes(Graph^ graph);
		bool checkTypes(Graph^ graph, bool noThrow);

		void checkNestedTypes(Object^ key);

		// Methods to read header & body from stream
		void readHeader(IO::BinaryReader^ reader);
		void readBody(IO::BinaryReader^ reader);

	internal:
		// Internal access functions
		SqlGraph^ getSqlTree();
		Internal::SqlObject^ getValueType();

		// Clone function
		Graph^ Copy(bool includeItems);

	public:
		/**
		 * Creates a new Graph of sql_variant
		 *
		 */
		[SqlMethod(IsDeterministic=true, IsPrecise=true, DataAccess=DataAccessKind::None)]
		static Graph^ New();

		/**
		 * Creates a new Graph of Variant
		 *
		 */
		[SqlMethod(IsDeterministic=true, IsPrecise=true, DataAccess=DataAccessKind::None)]
		static Graph^ NewVariant();

		/**
		 * Creates a new Graph using the specified value type
		 *
		 */
		[SqlMethod(IsDeterministic=true, IsPrecise=true, OnNullCall=false, DataAccess=DataAccessKind::None)]
		static Graph^ NewOf(SqlString valueType);

		/**
		 * Adds an entry to the tree
		 *
		 * @param key The key of the entry to be added
		 * @param value The value of the entry to be added
		 *
		 */
		[SqlMethod(IsDeterministic=true, IsPrecise=true, DataAccess=DataAccessKind::None, IsMutator=true)]
		void AddEntry(SqlHierarchyId key, Object^ value);

		/**
		 * Adds a Variant entry to the tree
		 *
		 * @param key The key of the entry to be added
		 * @param value The dictionary value of the entry to be added
		 *
		 */
		[SqlMethod(IsDeterministic=true, IsPrecise=true, DataAccess=DataAccessKind::None, IsMutator=true)]
		void AddVariantEntry(SqlHierarchyId key, Variant^ value);

		/**
		 * Adds an entry to the tree if it does not already exist
		 *
		 * @param key The key of the entry to be added
		 * @param value The value of the entry to be added
		 *
		 */
		[SqlMethod(IsDeterministic=true, IsPrecise=true, DataAccess=DataAccessKind::None, IsMutator=true)]
		void AddEntryIfNotExists(SqlHierarchyId key, Object^ value);

		/**
		 * Adds a variant entry to the tree if it does not already exist
		 *
		 * @param key The key of the entry to be added
		 * @param value The value of the entry to be added
		 *
		 */
		[SqlMethod(IsDeterministic=true, IsPrecise=true, DataAccess=DataAccessKind::None, IsMutator=true)]
		void AddVariantEntryIfNotExists(SqlHierarchyId key, Variant^ value);

		/**
		 * Updates an entry in the tree
		 *
		 * @param key The key of the entry to be updated
		 * @param newValue The new value for the entry
		 *
		 */
		[SqlMethod(IsDeterministic=true, IsPrecise=true, DataAccess=DataAccessKind::None, IsMutator=true)]
		void UpdateEntry(SqlHierarchyId key, Object^ newValue);

		/**
		 * Updates a Variant entry in the tree
		 *
		 * @param key The key of the entry to be updated
		 * @param newValue The new value for the entry
		 *
		 */
		[SqlMethod(IsDeterministic=true, IsPrecise=true, DataAccess=DataAccessKind::None, IsMutator=true)]
		void UpdateNestedEntry(SqlHierarchyId key, Variant^ newValue);

		/**
		 * Updates an entry in the tree if it exists
		 *
		 * @param key The key of the entry to be updated
		 * @param newValue The new value for the entry
		 *
		 */
		[SqlMethod(IsDeterministic=true, IsPrecise=true, DataAccess=DataAccessKind::None, IsMutator=true)]
		void UpdateEntryIfExists(SqlHierarchyId key, Object^ newValue);

		/**
		 * Updates a Variant entry in the tree if it exists
		 *
		 * @param key The key of the entry to be updated
		 * @param newValue The new value for the entry
		 *
		 */
		[SqlMethod(IsDeterministic=true, IsPrecise=true, DataAccess=DataAccessKind::None, IsMutator=true)]
		void UpdateVariantEntryIfExists(SqlHierarchyId key, Variant^ newValue);

		/**
		 * Merges an entry into the tree
		 *
		 * @param key The key of the entry to be inserted/updated
		 * @param value The value for the entry
		 *
		 */
		[SqlMethod(IsDeterministic=true, IsPrecise=true, DataAccess=DataAccessKind::None, IsMutator=true)]
		void MergeEntry(SqlHierarchyId key, Object^ value);

		/**
		 * Merges a Variant entry into the tree
		 *
		 * @param key The key of the entry to be inserted/updated
		 * @param value The value for the entry
		 *
		 */
		[SqlMethod(IsDeterministic=true, IsPrecise=true, DataAccess=DataAccessKind::None, IsMutator=true)]
		void MergeVariantEntry(SqlHierarchyId key, Variant^ value);

		/**
		 * Removes an entry from the tree
		 *
		 * @param key The key of the entry to be removed
		 *
		 */
		[SqlMethod(IsDeterministic=true, IsPrecise=true, DataAccess=DataAccessKind::None, IsMutator=true)]
		void RemoveEntry(SqlHierarchyId key);

		/**
		 * Removes an entry from the tree if it exists
		 *
		 * @param key The key of the entry to be removed
		 *
		 */
		[SqlMethod(IsDeterministic=true, IsPrecise=true, DataAccess=DataAccessKind::None, IsMutator=true)]
		void RemoveEntryIfExists(SqlHierarchyId key);
		
		/**
		 * Checks if the tree contains the specified key
		 *
		 * @param key The key to check for
		 *
		 * @returns True if an entry exists in the Dictionary for the key, otherwise false
		 *
		 */
		[SqlMethod(IsDeterministic=true, IsPrecise=true, DataAccess=DataAccessKind::None)]
		SqlBoolean ContainsKey(SqlHierarchyId key);

		/**
		 * Adds entries to the Graph dictionary from the one specified
		 *
		 * @param graph The Graph for which values should be added
		 *
		 */
		[SqlMethod(IsDeterministic=true, OnNullCall=false, IsPrecise=true, DataAccess=DataAccessKind::None, IsMutator=true)]
		void AddGraph(Graph^ tree);

		/**
		 * Updates entries in the Graph dictionary from the one specified if they exist
		 *
		 * @param graph The Graph for which values should be updated
		 *
		 */
		[SqlMethod(IsDeterministic=true, OnNullCall=false, IsPrecise=true, DataAccess=DataAccessKind::None, IsMutator=true)]
		void UpdateGraph(Graph^ tree);

		/**
		 * Removes entries in the current Graph based on the one specified if they exist
		 *
		 * @param graph The Graph to be used a the basis of entries to be removed
		 *
		 */
		[SqlMethod(IsDeterministic=true, OnNullCall=false, IsPrecise=true, DataAccess=DataAccessKind::None, IsMutator=true)]
		void RemoveGraph(Graph^ tree);

		/**
		 * Merges entries into the current Graph from the one specified
		 *
		 * @param graph The Graph to be merged
		 *
		 */
		[SqlMethod(IsDeterministic=true, OnNullCall=false, IsPrecise=true, DataAccess=DataAccessKind::None, IsMutator=true)]
		void MergeGraph(Graph^ tree);

		/**
		 * Clears all entries from the tree
		 *
		 */
		[SqlMethod(IsDeterministic=true, IsPrecise=true, DataAccess=DataAccessKind::None, IsMutator=true)]
		void Clear();

		/**
		 * Clones the Graph to create a separate copy
		 *
		 * @returns A clone of the Graph
		 *
		 */
		[SqlMethod(IsDeterministic=true, IsPrecise=true, DataAccess=DataAccessKind::None)]
		Graph^ Clone();

		/**
		 * Sets the state of the map to be compressed or not
		 *
		 * @param compressed True if the map should be compressed, otherwise false
		 *
		 */
		[SqlMethod(IsDeterministic=true, OnNullCall=false, IsPrecise=true, DataAccess=DataAccessKind::None, IsMutator=true)]
		void SetCompression(SqlBoolean compressed);

		/**
		 * Returns whether one Graph equals another
		 *
		 * @param tree The tree to compare the current instance to
		 *
		 * @returns True if the two trees are of the same type and with the same data items, otherwise false
		 *
		 */
		[SqlMethod(IsDeterministic=true, OnNullCall=false, IsPrecise=true, DataAccess=DataAccessKind::None, IsMutator=false)]
		SqlBoolean Equals(Graph^ tree);

		/**
		 * Gets a value from the Tree based on a specified key
		 *
		 * @param key The key to lookup the value on
		 *
		 * @returns The value associated with the specified key
		 *
		 */
		[SqlMethod(IsDeterministic=true, IsPrecise=true, DataAccess=DataAccessKind::None)]
		Object^ Get(SqlHierarchyId key);

		/**
		 * Gets a value from the Tree based on a specified key if it exists
		 *
		 * @param key The key to lookup the value on
		 *
		 * @returns The value associated with the specified key, or null
		 *
		 */
		[SqlMethod(IsDeterministic=true, IsPrecise=true, DataAccess=DataAccessKind::None)]
		Object^ GetIfExists(SqlHierarchyId key);

		/**
		 * Gets a variant value from the Tree based on a specified key
		 *
		 * @param key The key to lookup the value on
		 *
		 * @returns The value associated with the specified key
		 *
		 */
		[SqlMethod(IsDeterministic=true, IsPrecise=true, DataAccess=DataAccessKind::None)]
		Variant^ GetVariant(SqlHierarchyId key);

		/**
		 * Gets a variant value from the Variant based on a specified key if it exists
		 *
		 * @param The key to lookup the value on
		 *
		 * @returns The value associated with the specified key, or null
		 *
		 */
		[SqlMethod(IsDeterministic=true, IsPrecise=true, DataAccess=DataAccessKind::None)]
		Variant^ GetVariantIfExists(SqlHierarchyId key);

		/**
		 * Gets the siblings of a given node
		 *
		 * @param key The node to return the siblings for
		 *
		 * @returns A tree value
		 *
		 */
		Graph^ GetSiblings(SqlHierarchyId key);

		/**
		 * Gets the preceding siblings of a given node
		 *
		 * @param key The node to return the siblings for
		 *
		 * @returns A tree value
		 *
		 */
		Graph^ GetPrecedingSiblings(SqlHierarchyId key);

		/**
		 * Gets the following siblings of a given node
		 *
		 * @param key The node to return the siblings for
		 *
		 * @returns A tree value
		 *
		 */
		Graph^ GetFollowingSiblings(SqlHierarchyId key);

		/**
		 * Gets the ancestors of a given node
		 *
		 * @param key The node to return the siblings for
		 *
		 * @returns A tree value
		 *
		 */
		Graph^ GetAncestors(SqlHierarchyId key);

		/**
		 * Gets the descendants of a given node
		 *
		 * @param key The node to return the branch for
		 *
		 * @returns A tree value
		 *
		 */
		Graph^ GetDescendents(SqlHierarchyId key);

		/**
		 * Returns the count of siblings of a given node
		 *
		 * @param key The node to return the count for
		 *
		 * @returns A count of nodes
		 *
		 */
		SqlInt32 GetSiblingCount(SqlHierarchyId key);

		/**
		 * Returns The nodes at a given level
		 *
		 * @param key The node to returns nodes for
		 *
		 * @returns A count of nodes
		 *
		 */
		Graph^ GetNodes(SqlInt32 level);

		/**
		 * Returns the id of the next sibling in relation to a node
		 *
		 * @param key The node to return a value for
		 *
		 * @returns A SqlHierarchyId value
		 *
		 */
		SqlHierarchyId GetNextSibling(SqlHierarchyId key);

		/**
		 * Returns the id of the previous sibling in relation to a node
		 *
		 * @param key The node to return a value for
		 *
		 * @returns A SqlHierarchyId value
		 *
		 */
		SqlHierarchyId GetPreviousSibling(SqlHierarchyId key);

		/**
		 * Returns the depth of the tree, i.e. the number of levels
		 *
		 * @returns A integer value
		 *
		 */
		SqlInt32 GetDepth();


		/// Gets the Nodes of the Graph
		property Enumerator^ Nodes {
			Enumerator^ get();
		}

		/// The value type for the collection
		property SqlString ValueType {
			SqlString get();
		}

		/// The number of items in the Tree
		property SqlInt32 Count {
			SqlInt32 get();
		}

		/// Whether or not the Dictionary is compressed
		property SqlBoolean IsCompressed {
			SqlBoolean get();
		}

		/// The IsNull property; necessary for SqlClr types
		property bool IsNull {
			virtual bool get();
		}

		/// Returns whether the dictionary stores variants
		property SqlBoolean IsVariant {
			SqlBoolean get();
		}

		/// A static property for the null value; necessary for SqlClr types
		static property Graph^ Null {
			Graph^ get();
		}
		
		/// Converts the current instance to a string representation; necessary for SqlClr types
		virtual String^ ToString() override;

		/**
		 * Parses a string value to create an instance of the type
		 *
		 * Necessary for SqlClr types
		 *
		 * @param value The value to be parsed
		 *
		 * @returns A Graph object
		 *
		 */
		[SqlMethod(IsDeterministic=true, OnNullCall = false, DataAccess=DataAccessKind::None)]
		static Graph^ Parse(SqlString sqlValue);

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

#endif // _SMART_SQLCLR_TYPES__GRAPH_HPP_
