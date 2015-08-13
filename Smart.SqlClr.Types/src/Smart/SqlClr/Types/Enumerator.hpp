
#ifndef _SMART_SQLCLR_TYPES__ENUMERATOR_HPP_
#define _SMART_SQLCLR_TYPES__ENUMERATOR_HPP_

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

	/**
	 * @class Enumerator
	 *
	 * @brief Allows for iteration over items in a collection
	 *
	 * This type is used within SQL Server to facilitate
	 * iteration over items in one of the collection types.
	 *
	 * @author Muhmud Ahmad
	 *
	 */
	[Serializable]
	[SqlUserDefinedType(Format::UserDefined, IsByteOrdered=false, MaxByteSize=-1, IsFixedLength=false)]
	public ref class Enumerator sealed : public Data::SqlTypes::INullable, 
												IBinarySerialize {
	internal:
		// Typedefs
		typedef Collections::IEnumerator SqlEnumerator;
		typedef Collections::IEnumerable SqlEnumerable;

		// Factory method
		static Enumerator^ New(SqlEnumerable^ sqlEnumerable, Internal::SqlObject^ itemType, bool isSorted, bool isCompressed);

		// Internals access methods
		Internal::SqlObject^ getItemType();
		SqlEnumerable^ GetSqlEnumerable();
		SqlEnumerator^ GetSqlEnumerator();

		// Clone function
		Enumerator^ Copy(bool includeItems);

	private:
		// Literals
		literal System::String^ XML_ROOT_ELEMENT = "Enumerator";
		literal System::String^ XML_VALUETYPE_ATTRIBUTE = "valueType";
		literal System::String^ XML_ISCOMPRESSED_ATTRIBUTE = "isCompressed";
		literal System::String^ XML_ISSORTED_ATTRIBUTE = "isSorted";
		literal System::String^ XML_ITEM_ELEMENT = "Item";

		// Typedef(s)
		typedef Collections::ArrayList SqlEnumerableImpl;

		// Static value for a null
		static initonly Enumerator^ NullEnumerator = create();

		// Members
		SqlEnumerator^ m_sqlEnumerator;
		SqlEnumerable^ m_sqlEnumerable;
		Internal::SqlObject^ m_itemType;
		Byte m_isNull;
		Byte m_isSorted;
		Byte m_isCompressed;
		Byte m_hasNext;
		int m_position;

		// Factory method(s)
		static Enumerator^ create();

		// Methods to read header & body from stream
		void readHeader(IO::BinaryReader^ reader);
		void readBody(IO::BinaryReader^ reader);

		
	public:
		/**
		 * Moves to the next item in the collection
		 *
		 */
		[SqlMethod(IsDeterministic=true, IsPrecise=true, DataAccess=DataAccessKind::None, IsMutator=true)]
		void MoveNext();

		/**
		 * Sets the enumerator back to it's original position
		 *
		 */
		[SqlMethod(IsDeterministic=true, IsPrecise=true, DataAccess=DataAccessKind::None, IsMutator=true)]
		virtual void Reset();

		/**
		 * Clones the enumerator to create a separate copy
		 *
		 * @returns A clone of the enumerator
		 *
		 */
		[SqlMethod(IsDeterministic=true, IsPrecise=true, DataAccess=DataAccessKind::None)]
		Enumerator^ Clone();

		/**
		 * Sets the state of the enumerator to be compressed or not
		 *
		 * @param compressed True if the enumerator should be compressed, otherwise false
		 *
		 */
		[SqlMethod(IsDeterministic=true, OnNullCall=false, IsPrecise=true, DataAccess=DataAccessKind::None, IsMutator=true)]
		void SetCompression(SqlBoolean compressed);

		/**
		 * Returns the current object
		 *
		 * @returns The current item in the enumeration
		 *
		 */
		property Object^ CurrentItem {
			virtual Object^ get();
		}

		/// The value type for the enumerated collection
		property SqlString ValueType {
			SqlString get();
		}

		/// The IsNull property; necessary for SqlClr types
		property bool IsNull {
			virtual bool get();
		}

		/// Returns whether the enumerator is sorted or not
		property SqlBoolean IsSorted {
			SqlBoolean get();
		}

		/// Returns whether the enumerator has another item
		property SqlBoolean HasNext {
			SqlBoolean get();
		}

		/// Returns the number of items in the enumerator
		property SqlInt32 Count {
			SqlInt32 get();
		}

		/// Returns whether the array is compressed or not
		property SqlBoolean IsCompressed {
			SqlBoolean get();
		}

		/// A static property for the null value; necessary for SqlClr types
		static property Enumerator^ Null {
			Enumerator^ get();
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
		 * @returns An Enumerator object
		 *
		 */
		[SqlMethod(IsDeterministic=true, OnNullCall = false, DataAccess=DataAccessKind::None)]
		static Enumerator^ Parse(SqlString sqlValue);

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

#endif // _SMART_SQLCLR_TYPES__ENUMERATOR_HPP_
