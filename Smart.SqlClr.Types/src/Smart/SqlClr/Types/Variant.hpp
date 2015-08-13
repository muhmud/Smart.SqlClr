
#ifndef _SMART_SQLCLR_TYPES__VARIANT_HPP_
#define _SMART_SQLCLR_TYPES__VARIANT_HPP_

#using <System.dll>
#using <System.Data.dll>
#using <Microsoft.SqlServer.Types.dll>

namespace Smart { namespace SqlClr { namespace Types {
	ref class Dictionary;
	ref class Enumerator;
	ref class List;

	using namespace System;
	using namespace System::Data::SqlTypes;
	using namespace Microsoft::SqlServer::Server;
	using namespace Microsoft::SqlServer::Types;

	/**
	 * @class Variant
	 *
	 * @brief Allows for storage of data of any type
	 *
	 * The variant type can be used to store data of
	 * any type in Sql, and provides more options for
	 * types than sql_variant.
	 *
	 * @author Muhmud Ahmad
	 *
	 */
	[Serializable]
	[SqlUserDefinedType(Format::UserDefined, IsByteOrdered=false, MaxByteSize=-1, IsFixedLength=false)]
	public ref class Variant sealed : public Data::SqlTypes::INullable, 
												IBinarySerialize {
	private:
		literal System::String^ XML_ROOT_ELEMENT = "Variant";
		literal System::String^ XML_TYPE_ATTRIBUTE = "type";
		literal System::String^ XML_VALUE_ELEMENT = "Value";

		// Static value for a null
		static initonly Variant^ NullVariant = create();
		
		// Members
		Object^ m_value;

		// Factory Method(s)
		static Variant^ create();

	public:
		/**
		 * Creates a Variant from a bigint
		 *
		 * @param value The value to create the variant from
		 *
		 * @returns A Variant object
		 *
		 */
		static Variant^ CreateFromBigInt(SqlInt64 value);

		/**
		 * Creates a Variant from a binary/varbinary value
		 *
		 * @param value The value to create the variant from
		 *
		 * @returns A Variant object
		 *
		 */
		static Variant^ CreateFromBinary(SqlBinary value);

		/**
		 * Creates a Variant from a bit value
		 *
		 * @param value The value to create the variant from
		 *
		 * @returns A Variant object
		 *
		 */
		static Variant^ CreateFromBit(SqlBoolean value);

		/**
		 * Creates a Variant from a varchar value
		 *
		 * @param value The value to create the variant from
		 *
		 * @returns A Variant object
		 *
		 */
		static Variant^ CreateFromVarchar(SqlString value);

		/**
		 * Creates a Variant from a nvarchar value
		 *
		 * @param value The value to create the variant from
		 *
		 * @returns A Variant object
		 *
		 */
		static Variant^ CreateFromNVarchar(SqlString value);

		/**
		 * Creates a Variant from a date value
		 *
		 * @param value The value to create the variant from
		 *
		 * @returns A Variant object
		 *
		 */
		static Variant^ CreateFromDate(DateTime value);

		/**
		 * Creates a Variant from a time value
		 *
		 * @param value The value to create the variant from
		 *
		 * @returns A Variant object
		 *
		 */
		static Variant^ CreateFromTime(DateTime value);

		/**
		 * Creates a Variant from a datetime2 value
		 *
		 * @param value The value to create the variant from
		 *
		 * @returns A Variant object
		 *
		 */
		static Variant^ CreateFromDateTime2(DateTime value);

		/**
		 * Creates a Variant from a datetime value
		 *
		 * @param value The value to create the variant from
		 *
		 * @returns A Variant object
		 *
		 */
		static Variant^ CreateFromDateTime(SqlDateTime value);

		/**
		 * Creates a Variant from a datetimeoffset value
		 *
		 * @param value The value to create the variant from
		 *
		 * @returns A Variant object
		 *
		 */
		static Variant^ CreateFromDateTimeOffset(DateTimeOffset value);

		/**
		 * Creates a Variant from a decimal/numeric value
		 *
		 * @param value The value to create the variant from
		 *
		 * @returns A Variant object
		 *
		 */
		static Variant^ CreateFromDecimal(SqlDecimal value);

		/**
		 * Creates a Variant from a double value
		 *
		 * @param value The value to create the variant from
		 *
		 * @returns A Variant object
		 *
		 */
		static Variant^ CreateFromFloat(SqlDouble value);
		
		/**
		 * Creates a Variant from an int value
		 *
		 * @param value The value to create the variant from
		 *
		 * @returns A Variant object
		 *
		 */
		static Variant^ CreateFromInt(SqlInt32 value);

		/**
		 * Creates a Variant from a money value
		 *
		 * @param value The value to create the variant from
		 *
		 * @returns A Variant object
		 *
		 */
		static Variant^ CreateFromMoney(SqlMoney value);

		/**
		 * Creates a Variant from a real value
		 *
		 * @param value The value to create the variant from
		 *
		 * @returns A Variant object
		 *
		 */
		static Variant^ CreateFromReal(SqlSingle value);

		/**
		 * Creates a Variant from a smallint value
		 *
		 * @param value The value to create the variant from
		 *
		 * @returns A Variant object
		 *
		 */
		static Variant^ CreateFromSmallInt(SqlInt16 value);

		/**
		 * Creates a Variant from a byte value
		 *
		 * @param value The value to create the variant from
		 *
		 * @returns A Variant object
		 *
		 */
		static Variant^ CreateFromTinyInt(SqlByte value);

		/**
		 * Creates a Variant from a uniqueidentifier value
		 *
		 * @param value The value to create the variant from
		 *
		 * @returns A Variant object
		 *
		 */
		static Variant^ CreateFromUniqueIdentifier(SqlGuid value);
		
		/**
		 * Creates a Variant from a xml value
		 *
		 * @param value The value to create the variant from
		 *
		 * @returns A Variant object
		 *
		 */
		static Variant^ CreateFromXml(SqlXml^ value);

		/**
		 * Creates a Variant from a hierarchyid value
		 *
		 * @param value The value to create the variant from
		 *
		 * @returns A Variant object
		 *
		 */
		static Variant^ CreateFromHierarchyID(SqlHierarchyId value);
		
		/**
		 * Creates a Variant from a geography value
		 *
		 * @param value The value to create the variant from
		 *
		 * @returns A Variant object
		 *
		 */
		static Variant^ CreateFromGeography(SqlGeography^ value);

		/**
		 * Creates a Variant from a geometry value
		 *
		 * @param value The value to create the variant from
		 *
		 * @returns A Variant object
		 *
		 */
		static Variant^ CreateFromGeometry(SqlGeometry^ value);

		/**
		 * Creates a Variant from a Dictionary value
		 *
		 * @param value The value to create the variant from
		 *
		 * @returns A Variant object
		 *
		 */
		static Variant^ CreateFromDictionary(Dictionary^ value);

		/**
		 * Creates a Variant from a Enumerator value
		 *
		 * @param value The value to create the variant from
		 *
		 * @returns A Variant object
		 *
		 */
		static Variant^ CreateFromEnumerator(Enumerator^ value);

		/**
		 * Creates a Variant from a List value
		 *
		 * @param value The value to create the variant from
		 *
		 * @returns A Variant object
		 *
		 */
		static Variant^ CreateFromList(List^ value);

		/**
		 * Creates a Variant from a sql_variant value
		 *
		 * @param value The value to create the variant from
		 *
		 * @returns A Variant object
		 *
		 */
		static Variant^ CreateFrom(Object^ value);


		/// Gets the type of the Variant as a string
		property SqlString Type {
			SqlString get();
		}

		/// Get the variant as a bigint
		property SqlInt64 AsBigInt {
			SqlInt64 get();
		}

		/// Get the variant as a binary
		property SqlBinary AsBinary {
			SqlBinary get();
		}

		/// Get the variant as a bit
		property SqlBoolean AsBoolean {
			SqlBoolean get();
		}

		/// Get the variant as a nvarchar
		property SqlString AsNVarchar {
			SqlString get();
		}
	
		/// Get the variant as a datetime2
		property DateTime AsDateTime2 {
			DateTime get();
		}

		/// Get the variant as a datetime
		property SqlDateTime AsDateTime {
			SqlDateTime get();
		}

		/// Get the variant as a datetimeoffset
		property DateTimeOffset AsDateTimeOffset {
			DateTimeOffset get();
		}

		/// Get the variant as a decimal
		property SqlDecimal AsDecimal {
			SqlDecimal get();
		}

		/// Get the variant as a float
		property SqlDouble AsFloat {
			SqlDouble get();
		}

		/// Get the variant as a int
		property SqlInt32 AsInt {
			SqlInt32 get();
		}
		
		/// Get the variant as a money
		property SqlMoney AsMoney {
			SqlMoney get();
		}

		/// Get the variant as a real
		property SqlSingle AsReal {
			SqlSingle get();
		}

		/// Get the variant as a smallint
		property SqlInt16 AsSmallInt {
			SqlInt16 get();
		}

		/// Get the variant as a tinyint
		property SqlByte AsTinyInt {
			SqlByte get();
		}

		/// Get the variant as a uniqueidentifier
		property SqlGuid AsUniqueIdentifier {
			SqlGuid get();
		}

		/// Get the variant as xml
		property SqlXml^ AsXml {
			SqlXml^ get();
		}

		/// Get the variant as a hierarchyid
		property SqlHierarchyId AsHierarchyID {
			SqlHierarchyId get();
		}

		/// Get the variant as a geography
		property SqlGeography^ AsGeography {
			SqlGeography^ get();
		}

		/// Get the variant as geometry
		property SqlGeometry^ AsGeometry {
			SqlGeometry^ get();
		}

		/// Get the variant as a Dictionary
		property Dictionary^ AsDictionary {
			Dictionary^ get();
		}

		/// Get the variant as a Enumerator
		property Enumerator^ AsEnumerator {
			Enumerator^ get();
		}

		/// Get the variant as a List
		property List^ AsList {
			List^ get();
		}

		/// Get the variant as a sql_variant
		property Object^ AsSqlVariant {
			Object^ get();
		}

		/// The IsNull property; necessary for SqlClr types
		property bool IsNull {
			virtual bool get();
		}

		/// A static property for the null value; necessary for SqlClr types
		static property Variant^ Null {
			Variant^ get();
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
		 * @returns A Dictionary object
		 *
		 */
		[SqlMethod(IsDeterministic=true, OnNullCall = false, DataAccess=DataAccessKind::None)]
		static Variant^ Parse(SqlString sqlValue);

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

#endif // _SMART_SQLCLR_TYPES__VARIANT_HPP_
