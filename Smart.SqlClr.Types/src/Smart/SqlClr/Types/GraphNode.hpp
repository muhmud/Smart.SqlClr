
#ifndef _SMART_SQLCLR_TYPES__GRAPHNODE_HPP_
#define _SMART_SQLCLR_TYPES__GRAPHNODE_HPP_

#using <System.dll>
#using <System.Data.dll>
#using <Microsoft.SqlServer.Types.dll>

namespace Smart { namespace SqlClr { 
	namespace Internal {
		ref class SqlObject;
	}

namespace Types {
	ref class Variant;

	using namespace System;
	using namespace System::Data::SqlTypes;
	using namespace Microsoft::SqlServer::Server;
	using namespace Microsoft::SqlServer::Types;
	
	/**
	 * @class GraphNode
	 *
	 * @brief Represents a node in a Graph
	 *
	 * This data type is used to convey a single node
	 * in the Graph data type.
	 *
	 * @see Smart::SqlClr::Types::GraphNode
	 *
	 * @author Muhmud Ahmad
	 *
	 */
	[Serializable]
	[SqlUserDefinedType(Format::UserDefined, IsByteOrdered=false, MaxByteSize=-1, IsFixedLength=false)]
	public ref class GraphNode sealed : public Data::SqlTypes::INullable, IBinarySerialize {
	internal:
		// Typedefs
		typedef Collections::Generic::KeyValuePair<SqlHierarchyId, Variant^> SqlGraphNode;

	private:
		// Literals
		literal System::String^ XML_ROOT_ELEMENT = "GraphNode";
		literal System::String^ XML_KEY_ELEMENT = "Key";
		literal System::String^ XML_VALUE_ELEMENT = "Value";

		// Static value for a null
		static initonly GraphNode^ NullGraphNode = create();

		// Internal Implementation
		SqlGraphNode m_graphNode;
		Byte m_isNull;

		// Factory method(s)
		static GraphNode^ create();

	public:
		/**
		 * Creates a new GraphNode
		 *
		 */
		[SqlMethod(IsDeterministic=true, IsPrecise=true, OnNullCall=false, DataAccess=DataAccessKind::None)]
		static GraphNode^ New(SqlHierarchyId nodeID, Variant^ value);

		/// Gets the ID of the node
		property SqlHierarchyId NodeID {
			SqlHierarchyId get();
		}

		/// Gets the values
		property Variant^ Value {
			Variant^ get();
		}

		/// The IsNull property; necessary for SqlClr types
		property bool IsNull {
			virtual bool get();
		}

		/// A static property for the null value; necessary for SqlClr types
		static property GraphNode^ Null {
			GraphNode^ get();
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
		 * @returns A Tree object
		 *
		 */
		[SqlMethod(IsDeterministic=true, OnNullCall = false, DataAccess=DataAccessKind::None)]
		static GraphNode^ Parse(SqlString sqlValue);

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


#endif // _SMART_SQLCLR_TYPES__GRAPHNODE_HPP_
