
#include <Smart/SqlClr/Internal/Test/SqlObject.hpp>

#using <System.dll>
#using <System.Data.dll>
#using <System.Xml.dll>
#using <Microsoft.SqlServer.Types.dll>

namespace Smart { namespace SqlClr { namespace Internal { namespace Test { 
	using namespace System;
	using namespace System::Data;
	using namespace NUnit::Framework;
	
	void SqlObject::assertSqlTypes(String^ expectedSqlType, Type^ expectedSqlDataType, bool expectedSqlVariantCompatible, int expectedPrecision, int expectedScale, int expectedLength, Internal::SqlObject^ object) {
		Assert::AreEqual(expectedSqlType, object->SqlType, "The SqlObject does not have the right SqlType");
		Assert::AreEqual(expectedSqlDataType, object->SqlDataType, "The SqlObject does not have the right SqlDataType");
		Assert::AreEqual(expectedSqlVariantCompatible, object->IsSqlVariantCompatible, "The SqlObject does not have the right Variant compatibility");
		Assert::AreEqual(expectedPrecision, object->Precision, "The SqlObject does not have the right precision");
		Assert::AreEqual(expectedScale, object->Scale, "The SqlObject does not have the right scale");
		Assert::AreEqual(expectedLength, object->Length, "The SqlObject does not have the right length");

	}

	void SqlObject::TestConstruction() {
		// Create an object
		Internal::SqlObject^ object = Internal::SqlObject::Create();
		assertSqlTypes("varchar", SqlTypes::SqlString::typeid, true, -1, -1, 8000, object);

		// Test each type
		object = Internal::SqlObject::Create("bigint");
		assertSqlTypes("bigint", SqlTypes::SqlInt64::typeid, true, -1, -1, -1, object);

		object = Internal::SqlObject::Create("binary");
		assertSqlTypes("binary", SqlTypes::SqlBinary::typeid, true, -1, -1, 8000, object);

		object = Internal::SqlObject::Create("bit");
		assertSqlTypes("bit", SqlTypes::SqlBoolean::typeid, true, -1, -1, -1, object);

		object = Internal::SqlObject::Create("char");
		assertSqlTypes("char", SqlTypes::SqlString::typeid, true, -1, -1, 8000, object);

		object = Internal::SqlObject::Create("date");
		assertSqlTypes("date", DateTime::typeid, true, -1, -1, -1, object);

		object = Internal::SqlObject::Create("datetime");
		assertSqlTypes("datetime", SqlTypes::SqlDateTime::typeid, true, -1, -1, -1, object);

		object = Internal::SqlObject::Create("datetime2");
		assertSqlTypes("datetime2", DateTime::typeid, true, -1, -1, -1, object);

		object = Internal::SqlObject::Create("time");
		assertSqlTypes("time", DateTime::typeid, true, -1, -1, -1, object);

		object = Internal::SqlObject::Create("datetimeoffset");
		assertSqlTypes("datetimeoffset", DateTimeOffset::typeid, true, -1, -1, -1, object);

		object = Internal::SqlObject::Create("decimal");
		assertSqlTypes("decimal", SqlTypes::SqlDecimal::typeid, true, -1, -1, -1, object);

		object = Internal::SqlObject::Create("float");
		assertSqlTypes("float", SqlTypes::SqlDouble::typeid, true, -1, -1, -1, object);

		object = Internal::SqlObject::Create("int");
		assertSqlTypes("int", SqlTypes::SqlInt32::typeid, true, -1, -1, -1, object);

		object = Internal::SqlObject::Create("money");
		assertSqlTypes("money", SqlTypes::SqlMoney::typeid, true, -1, -1, -1, object);

		object = Internal::SqlObject::Create("nchar");
		assertSqlTypes("nchar", SqlTypes::SqlString::typeid, true, -1, -1, 4000, object);

		object = Internal::SqlObject::Create("numeric");
		assertSqlTypes("decimal", SqlTypes::SqlDecimal::typeid, true, -1, -1, -1, object);

		object = Internal::SqlObject::Create("nvarchar");
		assertSqlTypes("nvarchar", SqlTypes::SqlString::typeid, true, -1, -1, 4000, object);

		object = Internal::SqlObject::Create("real");
		assertSqlTypes("real", SqlTypes::SqlSingle::typeid, true, -1, -1, -1, object);

		object = Internal::SqlObject::Create("smalldatetime");
		assertSqlTypes("smalldatetime", SqlTypes::SqlDateTime::typeid, true, -1, -1, -1, object);

		object = Internal::SqlObject::Create("smallint");
		assertSqlTypes("smallint", SqlTypes::SqlInt16::typeid, true, -1, -1, -1, object);

		object = Internal::SqlObject::Create("smallmoney");
		assertSqlTypes("smallmoney", SqlTypes::SqlMoney::typeid, true, -1, -1, -1, object);

		object = Internal::SqlObject::Create("tinyint");
		assertSqlTypes("tinyint", SqlTypes::SqlByte::typeid, true, -1, -1, -1, object);

		object = Internal::SqlObject::Create("uniqueidentifier");
		assertSqlTypes("uniqueidentifier", SqlTypes::SqlGuid::typeid, true, -1, -1, -1, object);

		object = Internal::SqlObject::Create("varbinary");
		assertSqlTypes("varbinary", SqlTypes::SqlBinary::typeid, true, -1, -1, 8000, object);

		object = Internal::SqlObject::Create("varchar");
		assertSqlTypes("varchar", SqlTypes::SqlString::typeid, true, -1, -1, 8000, object);

		// Test an invalid type
		try {
			object = Internal::SqlObject::Create("invalid");

			Assert::True(false, "Creating a SqlObject for an invalid type should throw an exception");
		} catch (Exception^) { }

		// Test a null parameter
		try {
			object = Internal::SqlObject::Create((String^) nullptr);

			Assert::True(false, "Creating a SqlObject for a null parameter should throw an exception");
		} catch (Exception^) { }

		// Test a empoty parameter
		try {
			object = Internal::SqlObject::Create(String::Empty);

			Assert::True(false, "Creating a SqlObject for a empty parameter should throw an exception");
		} catch (Exception^) { }

		// Test a space parameter
		try {
			object = Internal::SqlObject::Create("		");

			Assert::True(false, "Creating a SqlObject for a space parameter should throw an exception");
		} catch (Exception^) { }

		object = Internal::SqlObject::Create(" VarChar	");
		assertSqlTypes("varchar", SqlTypes::SqlString::typeid, true, -1, -1, 8000, object);

		// Valid type with a space in the middle
		try {
			object = Internal::SqlObject::Create("Var Char");

			Assert::True(false, "Creating a SqlObject for a valid type with a space should throw an exception");
		} catch (Exception^) { }

		object = Internal::SqlObject::Create("VarChar (128)");
		assertSqlTypes("varchar(128)", SqlTypes::SqlString::typeid, true, -1, -1, 128, object);

		// Valid type with invalid size
		try {
			object = Internal::SqlObject::Create("varchar(-1)");

			Assert::True(false, "Creating a SqlObject for a valid type with invalid size should throw an exception");
		} catch (Exception^) { }

		// Valid type with invalid size
		try {
			object = Internal::SqlObject::Create("char(0)");

			Assert::True(false, "Creating a SqlObject for a valid type with invalid size should throw an exception");
		} catch (Exception^) { }

		// Valid type with invalid size
		try {
			object = Internal::SqlObject::Create("nchar(A)");

			Assert::True(false, "Creating a SqlObject for a valid type with invalid size should throw an exception");
		} catch (Exception^) { }

		// Valid type with invalid size
		try {
			object = Internal::SqlObject::Create("nvarchar(0x10)");

			Assert::True(false, "Creating a SqlObject for a valid type with invalid size should throw an exception");
		} catch (Exception^) { }

		// Valid type with invalid size
		try {
			object = Internal::SqlObject::Create("nvarchar(8000)");

			Assert::True(false, "Creating a SqlObject for a valid type with invalid size should throw an exception");
		} catch (Exception^) { }

		// Valid type with invalid size
		try {
			object = Internal::SqlObject::Create("nchar(8000)");

			Assert::True(false, "Creating a SqlObject for a valid type with invalid size should throw an exception");
		} catch (Exception^) { }

		object = Internal::SqlObject::Create("   CHar (   8000   ) ");
		assertSqlTypes("char(8000)", SqlTypes::SqlString::typeid, true, -1, -1, 8000, object);

		// Valid type with invalid size
		try {
			object = Internal::SqlObject::Create("varchar( 8 000)");

			Assert::True(false, "Creating a SqlObject for a valid type with invalid size should throw an exception");
		} catch (Exception^) { }

		object = Internal::SqlObject::Create("   NChar (   0004000   ) ");
		assertSqlTypes("nchar(4000)", SqlTypes::SqlString::typeid, true, -1, -1, 4000, object);

		object = Internal::SqlObject::Create("   binary (   8000 ) ");
		assertSqlTypes("binary(8000)", SqlTypes::SqlBinary::typeid, true, -1, -1, 8000, object);

		object = Internal::SqlObject::Create("varbinary(8000)");
		assertSqlTypes("varbinary(8000)", SqlTypes::SqlBinary::typeid, true, -1, -1, 8000, object);

		// Valid type with invalid size
		try {
			object = Internal::SqlObject::Create("varbinary(00)");

			Assert::True(false, "Creating a SqlObject for a valid type with invalid size should throw an exception");
		} catch (Exception^) { }

		// Valid type with invalid size
		try {
			object = Internal::SqlObject::Create("varbinary(8001)");

			Assert::True(false, "Creating a SqlObject for a valid type with invalid size should throw an exception");
		} catch (Exception^) { }

		// Valid type with invalid size
		try {
			object = Internal::SqlObject::Create("binary(8001)");

			Assert::True(false, "Creating a SqlObject for a valid type with invalid size should throw an exception");
		} catch (Exception^) { }

		object = Internal::SqlObject::Create("float(23)");
		assertSqlTypes("real", SqlTypes::SqlSingle::typeid, true, 23, -1, -1, object);

		object = Internal::SqlObject::Create("float(24)");
		assertSqlTypes("real", SqlTypes::SqlSingle::typeid, true, 24, -1, -1, object);

		object = Internal::SqlObject::Create("float(25)");
		assertSqlTypes("float", SqlTypes::SqlDouble::typeid, true, 25, -1, -1, object);

		object = Internal::SqlObject::Create("float(53)");
		assertSqlTypes("float", SqlTypes::SqlDouble::typeid, true, 53, -1, -1, object);

		// Valid type with invalid size
		try {
			object = Internal::SqlObject::Create("float(0)");

			Assert::True(false, "Creating a SqlObject for a valid type with invalid size should throw an exception");
		} catch (Exception^) { }

		try {
			object = Internal::SqlObject::Create("float(54)");

			Assert::True(false, "Creating a SqlObject for a valid type with invalid size should throw an exception");
		} catch (Exception^) { }

		try {
			object = Internal::SqlObject::Create("float(540)");

			Assert::True(false, "Creating a SqlObject for a valid type with invalid size should throw an exception");
		} catch (Exception^) { }

		object = Internal::SqlObject::Create("datetime2(0)");
		assertSqlTypes("datetime2(0)", DateTime::typeid, true, 0, -1, -1, object);

		object = Internal::SqlObject::Create("datetime2(3)");
		assertSqlTypes("datetime2(3)", DateTime::typeid, true, 3, -1, -1, object);

		object = Internal::SqlObject::Create("datetime2(7)");
		assertSqlTypes("datetime2(7)", DateTime::typeid, true, 7, -1, -1, object);

		try {
			object = Internal::SqlObject::Create("datetime2(-1)");

			Assert::True(false, "Creating a SqlObject for a valid type with invalid size should throw an exception");
		} catch (Exception^) { }

		try {
			object = Internal::SqlObject::Create("datetime2(8)");

			Assert::True(false, "Creating a SqlObject for a valid type with invalid size should throw an exception");
		} catch (Exception^) { }

		object = Internal::SqlObject::Create("time(4 )");
		assertSqlTypes("time(4)", DateTime::typeid, true, 4, -1, -1, object);

		object = Internal::SqlObject::Create("datetimeoffset(2 )");
		assertSqlTypes("datetimeoffset(2)", DateTimeOffset::typeid, true, 2, -1, -1, object);

		object = Internal::SqlObject::Create("decimal(38)");
		assertSqlTypes("decimal(38)", SqlTypes::SqlDecimal::typeid, true, 38, 0, -1, object);

		object = Internal::SqlObject::Create("decimal(1)");
		assertSqlTypes("decimal(1)", SqlTypes::SqlDecimal::typeid, true, 1, 0, -1, object);

		object = Internal::SqlObject::Create("numeric(12,12)");
		assertSqlTypes("decimal(12, 12)", SqlTypes::SqlDecimal::typeid, true, 12, 12, -1, object);

		object = Internal::SqlObject::Create("numeric      (13,     12)");
		assertSqlTypes("decimal(13, 12)", SqlTypes::SqlDecimal::typeid, true, 13, 12, -1, object);

		try {
			object = Internal::SqlObject::Create("numeric(0)");

			Assert::True(false, "Creating a SqlObject for a valid type with invalid size should throw an exception");
		} catch (Exception^) { }

		try {
			object = Internal::SqlObject::Create("numeric(-1)");

			Assert::True(false, "Creating a SqlObject for a valid type with invalid size should throw an exception");
		} catch (Exception^) { }

		try {
			object = Internal::SqlObject::Create("numeric(39)");

			Assert::True(false, "Creating a SqlObject for a valid type with invalid size should throw an exception");
		} catch (Exception^) { }

		try {
			object = Internal::SqlObject::Create("numeric(12, 13)");

			Assert::True(false, "Creating a SqlObject for a valid type with invalid size should throw an exception");
		} catch (Exception^) { }

		try {
			object = Internal::SqlObject::Create("numeric(12, A)");

			Assert::True(false, "Creating a SqlObject for a valid type with invalid size should throw an exception");
		} catch (Exception^) { }

		try {
			object = Internal::SqlObject::Create("numeric(12, -1)");

			Assert::True(false, "Creating a SqlObject for a valid type with invalid size should throw an exception");
		} catch (Exception^) { }

		try {
			object = Internal::SqlObject::Create("numeric(09, 39)");

			Assert::True(false, "Creating a SqlObject for a valid type with invalid size should throw an exception");
		} catch (Exception^) { }

		object = Internal::SqlObject::Create("decimal(21, 0)");
		assertSqlTypes("decimal(21, 0)", SqlTypes::SqlDecimal::typeid, true, 21, 0, -1, object);

		object = Internal::SqlObject::Create("decimal ( 21 , 020 )");
		assertSqlTypes("decimal(21, 20)", SqlTypes::SqlDecimal::typeid, true, 21, 20, -1, object);

		object = Internal::SqlObject::Create("nvarchar( max )");
		assertSqlTypes("nvarchar(max)", SqlTypes::SqlString::typeid, false, -1, -1, -1, object);

		object = Internal::SqlObject::Create("varchar(max )");
		assertSqlTypes("varchar(max)", SqlTypes::SqlString::typeid, false, -1, -1, -1, object);

		object = Internal::SqlObject::Create("varbinary ( max)");
		assertSqlTypes("varbinary(max)", SqlTypes::SqlBinary::typeid, false, -1, -1, -1, object);

		object = Internal::SqlObject::Create("xml");
		assertSqlTypes("xml", SqlTypes::SqlXml::typeid, false, -1, -1, -1, object);

		object = Internal::SqlObject::Create("hierarchyid");
		assertSqlTypes("hierarchyid", SqlTypes::SqlBinary::typeid, false, -1, -1, -1, object);

		object = Internal::SqlObject::Create("geometry");
		assertSqlTypes("geometry", SqlTypes::SqlBinary::typeid, false, -1, -1, -1, object);

		object = Internal::SqlObject::Create("geography");
		assertSqlTypes("geography", SqlTypes::SqlBinary::typeid, false, -1, -1, -1, object);

		object = Internal::SqlObject::Create("UDT");
		assertSqlTypes("udt", SqlTypes::SqlBinary::typeid, false, -1, -1, -1, object);

		object = Internal::SqlObject::Create("char(256)");
		assertSqlTypes("char(256)", SqlTypes::SqlString::typeid, true, -1, -1, 256, object);

		object = Internal::SqlObject::Create("nchar  (97)");
		assertSqlTypes("nchar(97)", SqlTypes::SqlString::typeid, true, -1, -1, 97, object);

		object = Internal::SqlObject::Create("nvarchar  (3999)");
		assertSqlTypes("nvarchar(3999)", SqlTypes::SqlString::typeid, true, -1, -1, 3999, object);

		object = Internal::SqlObject::Create("sql_variant");
		assertSqlTypes("sql_variant", Object::typeid, false, -1, -1, -1, object);

		object = Internal::SqlObject::Create("binary       varying");
		assertSqlTypes("varbinary", SqlTypes::SqlBinary::typeid, true, -1, -1, 8000, object);

		object = Internal::SqlObject::Create("binary     varying   (7999)");
		assertSqlTypes("varbinary(7999)", SqlTypes::SqlBinary::typeid, true, -1, -1, 7999, object);

		object = Internal::SqlObject::Create("binary                 varying   (   8000 )");
		assertSqlTypes("varbinary(8000)", SqlTypes::SqlBinary::typeid, true, -1, -1, 8000, object);

		try {
			object = Internal::SqlObject::Create("binary varying   (8001)");

			Assert::True(false, "Creating a SqlObject for a valid type with invalid size should throw an exception");
		} catch (Exception^) { }

		object = Internal::SqlObject::Create("dec");
		assertSqlTypes("decimal", SqlTypes::SqlDecimal::typeid, true, -1, -1, -1, object);

		try {
			object = Internal::SqlObject::Create("dec(09, 39)");

			Assert::True(false, "Creating a SqlObject for a valid type with invalid size should throw an exception");
		} catch (Exception^) { }

		object = Internal::SqlObject::Create("DeC    (15,     12)");
		assertSqlTypes("decimal(15, 12)", SqlTypes::SqlDecimal::typeid, true, 15, 12, -1, object);

		object = Internal::SqlObject::Create("national character varying");
		assertSqlTypes("nvarchar", SqlTypes::SqlString::typeid, true, -1, -1, 4000, object);

		object = Internal::SqlObject::Create("national char varying  (2000)");
		assertSqlTypes("nvarchar(2000)", SqlTypes::SqlString::typeid, true, -1, -1, 2000, object);

		try {
			object = Internal::SqlObject::Create("national char varying(4001)");

			Assert::True(false, "Creating a SqlObject for a valid type with invalid size should throw an exception");
		} catch (Exception^) { }

		object = Internal::SqlObject::Create("national char ");
		assertSqlTypes("nchar", SqlTypes::SqlString::typeid, true, -1, -1, 4000, object);

		object = Internal::SqlObject::Create("national char (100)");
		assertSqlTypes("nchar(100)", SqlTypes::SqlString::typeid, true, -1, -1, 100, object);

		object = Internal::SqlObject::Create("national character");
		assertSqlTypes("nchar", SqlTypes::SqlString::typeid, true, -1, -1, 4000, object);

		object = Internal::SqlObject::Create("national character (1203)");
		assertSqlTypes("nchar(1203)", SqlTypes::SqlString::typeid, true, -1, -1, 1203, object);

		object = Internal::SqlObject::Create("character (1203)");
		assertSqlTypes("char(1203)", SqlTypes::SqlString::typeid, true, -1, -1, 1203, object);

		object = Internal::SqlObject::Create("character");
		assertSqlTypes("char", SqlTypes::SqlString::typeid, true, -1, -1, 8000, object);

		object = Internal::SqlObject::Create("character varying");
		assertSqlTypes("varchar", SqlTypes::SqlString::typeid, true, -1, -1, 8000, object);

		object = Internal::SqlObject::Create("  char    varying(333)    ");
		assertSqlTypes("varchar(333)", SqlTypes::SqlString::typeid, true, -1, -1, 333, object);

		object = Internal::SqlObject::Create("char varying");
		assertSqlTypes("varchar", SqlTypes::SqlString::typeid, true, -1, -1, 8000, object);

		object = Internal::SqlObject::Create("char varying(23)");
		assertSqlTypes("varchar(23)", SqlTypes::SqlString::typeid, true, -1, -1, 23, object);

		try {
			object = Internal::SqlObject::Create("char varying(8001)");

			Assert::True(false, "Creating a SqlObject for a valid type with invalid size should throw an exception");
		} catch (Exception^) { }

		object = Internal::SqlObject::Create("double precision");
		assertSqlTypes("float", SqlTypes::SqlDouble::typeid, true, -1, -1, -1, object);

		object = Internal::SqlObject::Create("integer");
		assertSqlTypes("int", SqlTypes::SqlInt32::typeid, true, -1, -1, -1, object);

		object = Internal::SqlObject::Create("string");
		assertSqlTypes("string", SqlTypes::SqlString::typeid, true, -1, -1, -1, object);
	}

	void SqlObject::TestValidity() {
		// Create an object
		Internal::SqlObject^ object = Internal::SqlObject::Create();
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlString("abc")));
		Assert::AreEqual(false, object->IsValueValid(SqlTypes::SqlString(String::Empty->PadLeft(8001, ' '))));
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlString::Null));
		Assert::AreEqual(true, object->IsValueValid(DBNull::Value));
		Assert::AreEqual(true, object->IsValueValid(nullptr));

		object = Internal::SqlObject::Create("varchar(34)");
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlString("sdkfjbnk")));
		Assert::AreEqual(false, object->IsValueValid(SqlTypes::SqlString(String::Empty->PadLeft(35, ' '))));
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlString::Null));
		Assert::AreEqual(true, object->IsValueValid(DBNull::Value));
		Assert::AreEqual(true, object->IsValueValid(nullptr));

		object = Internal::SqlObject::Create("char(254)");
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlString("wefokjln324olkn")));
		Assert::AreEqual(false, object->IsValueValid(SqlTypes::SqlString(String::Empty->PadLeft(255, ' '))));
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlString::Null));
		Assert::AreEqual(true, object->IsValueValid(DBNull::Value));
		Assert::AreEqual(true, object->IsValueValid(nullptr));

		object = Internal::SqlObject::Create("nchar(254)");
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlString("fwer908hj")));
		Assert::AreEqual(false, object->IsValueValid(SqlTypes::SqlString(String::Empty->PadLeft(255, ' '))));
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlString::Null));
		Assert::AreEqual(true, object->IsValueValid(DBNull::Value));
		Assert::AreEqual(true, object->IsValueValid(nullptr));

		object = Internal::SqlObject::Create("nchar");
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlString("234oikjun")));
		Assert::AreEqual(false, object->IsValueValid(SqlTypes::SqlString(String::Empty->PadLeft(4001, ' '))));
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlString::Null));
		Assert::AreEqual(true, object->IsValueValid(DBNull::Value));
		Assert::AreEqual(true, object->IsValueValid(nullptr));

		object = Internal::SqlObject::Create("nvarchar(38)");
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlString("32i4kjubn23")));
		Assert::AreEqual(false, object->IsValueValid(SqlTypes::SqlString(String::Empty->PadLeft(39, ' '))));
		Assert::AreEqual(false, object->IsValueValid(SqlTypes::SqlBinary::Null));
		Assert::AreEqual(true, object->IsValueValid(DBNull::Value));
		Assert::AreEqual(true, object->IsValueValid(nullptr));

		object = Internal::SqlObject::Create("varbinary(128)");
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlBinary(System::Text::Encoding::ASCII->GetBytes(String::Empty->PadLeft(128, ' ')))));
		Assert::AreEqual(false, object->IsValueValid(SqlTypes::SqlBinary(System::Text::Encoding::ASCII->GetBytes(String::Empty->PadLeft(129, ' ')))));
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlBinary::Null));
		Assert::AreEqual(true, object->IsValueValid(DBNull::Value));
		Assert::AreEqual(true, object->IsValueValid(nullptr));

		object = Internal::SqlObject::Create("binary(777)");
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlBinary(System::Text::Encoding::ASCII->GetBytes(String::Empty->PadLeft(777, ' ')))));
		Assert::AreEqual(false, object->IsValueValid(SqlTypes::SqlBinary(System::Text::Encoding::ASCII->GetBytes(String::Empty->PadLeft(778, ' ')))));
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlBinary::Null));
		Assert::AreEqual(true, object->IsValueValid(DBNull::Value));
		Assert::AreEqual(true, object->IsValueValid(nullptr));

		object = Internal::SqlObject::Create("varbinary");
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlBinary(System::Text::Encoding::ASCII->GetBytes(String::Empty->PadLeft(8000, ' ')))));
		Assert::AreEqual(false, object->IsValueValid(SqlTypes::SqlBinary(System::Text::Encoding::ASCII->GetBytes(String::Empty->PadLeft(8001, ' ')))));
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlBinary::Null));
		Assert::AreEqual(true, object->IsValueValid(DBNull::Value));
		Assert::AreEqual(true, object->IsValueValid(nullptr));

		object = Internal::SqlObject::Create("binary");
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlBinary(System::Text::Encoding::ASCII->GetBytes(String::Empty->PadLeft(8000, ' ')))));
		Assert::AreEqual(false, object->IsValueValid(SqlTypes::SqlBinary(System::Text::Encoding::ASCII->GetBytes(String::Empty->PadLeft(8001, ' ')))));
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlBinary::Null));
		Assert::AreEqual(true, object->IsValueValid(DBNull::Value));
		Assert::AreEqual(true, object->IsValueValid(nullptr));

		object = Internal::SqlObject::Create("decimal");
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlDecimal(1.0)));
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlDecimal(1)));
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlDecimal(1234.234234)));
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlDecimal::Parse("12345678901234567890123456789012345678")));
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlDecimal(safe_cast<Decimal>(1234567890123456789.0123456789012345678))));
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlDecimal::Null));
		Assert::AreEqual(true, object->IsValueValid(DBNull::Value));
		Assert::AreEqual(true, object->IsValueValid(nullptr));

		object = Internal::SqlObject::Create("decimal(18)");
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlDecimal(123456789012345678)));
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlDecimal(123456789012345678.123213)));
		Assert::AreEqual(false, object->IsValueValid(SqlTypes::SqlDecimal(1234567890123456789)));
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlDecimal::Null));
		Assert::AreEqual(true, object->IsValueValid(DBNull::Value));
		Assert::AreEqual(true, object->IsValueValid(nullptr));

		object = Internal::SqlObject::Create("decimal(20, 2)");
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlDecimal(123456789012345678)));
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlDecimal(123456789012345678.123213)));
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlDecimal::Parse("12345678901234567890")));
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlDecimal::Parse("12345678901234567890.213")));
		Assert::AreEqual(false, object->IsValueValid(SqlTypes::SqlDecimal::Parse("123456789012345678901.213")));
		Assert::AreEqual(false, object->IsValueValid(SqlTypes::SqlDecimal::Parse("123456789012345678901")));
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlDecimal::Null));
		Assert::AreEqual(true, object->IsValueValid(DBNull::Value));
		Assert::AreEqual(true, object->IsValueValid(nullptr));

		object = Internal::SqlObject::Create("numeric");
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlDecimal(1.0)));
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlDecimal(1)));
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlDecimal(1234.234234)));
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlDecimal::Parse("12345678901234567890123456789012345678")));
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlDecimal(safe_cast<Decimal>(1234567890123456789.0123456789012345678))));
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlDecimal::Null));
		Assert::AreEqual(true, object->IsValueValid(DBNull::Value));
		Assert::AreEqual(true, object->IsValueValid(nullptr));

		object = Internal::SqlObject::Create("numeric(18)");
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlDecimal(123456789012345678)));
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlDecimal(123456789012345678.123213)));
		Assert::AreEqual(false, object->IsValueValid(SqlTypes::SqlDecimal(1234567890123456789)));
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlDecimal::Null));
		Assert::AreEqual(true, object->IsValueValid(DBNull::Value));
		Assert::AreEqual(true, object->IsValueValid(nullptr));

		object = Internal::SqlObject::Create("numeric(20, 2)");
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlDecimal(123456789012345678)));
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlDecimal(123456789012345678.123213)));
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlDecimal::Parse("12345678901234567890")));
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlDecimal::Parse("12345678901234567890.213")));
		Assert::AreEqual(false, object->IsValueValid(SqlTypes::SqlDecimal::Parse("123456789012345678901.213")));
		Assert::AreEqual(false, object->IsValueValid(SqlTypes::SqlDecimal::Parse("123456789012345678901")));
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlDecimal::Null));
		Assert::AreEqual(true, object->IsValueValid(DBNull::Value));
		Assert::AreEqual(true, object->IsValueValid(nullptr));

		object = Internal::SqlObject::Create("bigint");
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlInt64(123456789012345678)));
		Assert::AreEqual(false, object->IsValueValid(SqlTypes::SqlInt32(1234567890)));
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlInt64::Null));
		Assert::AreEqual(true, object->IsValueValid(DBNull::Value));
		Assert::AreEqual(true, object->IsValueValid(nullptr));

		object = Internal::SqlObject::Create("bit");
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlBoolean(true)));
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlBoolean(false)));
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlBoolean::Null));
		Assert::AreEqual(true, object->IsValueValid(DBNull::Value));
		Assert::AreEqual(true, object->IsValueValid(nullptr));

		object = Internal::SqlObject::Create("date");
		Assert::AreEqual(true, object->IsValueValid(DateTime::Now));
		Assert::AreEqual(true, object->IsValueValid(DateTime::Now.Date));
		Assert::AreEqual(true, object->IsValueValid(DBNull::Value));
		Assert::AreEqual(true, object->IsValueValid(nullptr));

		object = Internal::SqlObject::Create("datetime");
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlDateTime(DateTime::Now)));
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlDateTime(DateTime::Now.Date)));
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlDateTime::Null));
		Assert::AreEqual(true, object->IsValueValid(DBNull::Value));
		Assert::AreEqual(true, object->IsValueValid(nullptr));

		object = Internal::SqlObject::Create("datetime2");
		Assert::AreEqual(true, object->IsValueValid(DateTime::Now));
		Assert::AreEqual(true, object->IsValueValid(DateTime::Now.Date));
		Assert::AreEqual(false, object->IsValueValid(DateTimeOffset::Now));
		Assert::AreEqual(true, object->IsValueValid(DBNull::Value));
		Assert::AreEqual(true, object->IsValueValid(nullptr));

		object = Internal::SqlObject::Create("datetime2(5)");
		Assert::AreEqual(true, object->IsValueValid(DateTime::Now));
		Assert::AreEqual(true, object->IsValueValid(DateTime::Now.Date));
		Assert::AreEqual(false, object->IsValueValid(DateTimeOffset::Now));
		Assert::AreEqual(true, object->IsValueValid(DBNull::Value));
		Assert::AreEqual(true, object->IsValueValid(nullptr));

		object = Internal::SqlObject::Create("datetimeoffset");
		Assert::AreEqual(false, object->IsValueValid(DateTime::Now));
		Assert::AreEqual(false, object->IsValueValid(DateTime::Now.Date));
		Assert::AreEqual(true, object->IsValueValid(DateTimeOffset::Now));
		Assert::AreEqual(true, object->IsValueValid(DBNull::Value));
		Assert::AreEqual(true, object->IsValueValid(nullptr));

		object = Internal::SqlObject::Create("datetimeoffset(7)");
		Assert::AreEqual(false, object->IsValueValid(DateTime::Now));
		Assert::AreEqual(false, object->IsValueValid(DateTime::Now.Date));
		Assert::AreEqual(true, object->IsValueValid(DateTimeOffset::Now));
		Assert::AreEqual(true, object->IsValueValid(DBNull::Value));
		Assert::AreEqual(true, object->IsValueValid(nullptr));

		object = Internal::SqlObject::Create("time(3)");
		Assert::AreEqual(true, object->IsValueValid(DateTime::Now));
		Assert::AreEqual(true, object->IsValueValid(DateTime::Now.Date));
		Assert::AreEqual(false, object->IsValueValid(DateTimeOffset::Now));
		Assert::AreEqual(true, object->IsValueValid(DBNull::Value));
		Assert::AreEqual(true, object->IsValueValid(nullptr));

		object = Internal::SqlObject::Create("float");
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlDouble(1.2345678)));
		Assert::AreEqual(false, object->IsValueValid(SqlTypes::SqlSingle(1.2345678)));
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlDouble::Null));
		Assert::AreEqual(true, object->IsValueValid(DBNull::Value));
		Assert::AreEqual(true, object->IsValueValid(nullptr));

		object = Internal::SqlObject::Create("int");
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlInt32(1232345)));
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlInt32::Null));
		Assert::AreEqual(true, object->IsValueValid(DBNull::Value));
		Assert::AreEqual(true, object->IsValueValid(nullptr));

		object = Internal::SqlObject::Create("money");
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlMoney(324.3454)));
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlMoney::Null));
		Assert::AreEqual(true, object->IsValueValid(DBNull::Value));
		Assert::AreEqual(true, object->IsValueValid(nullptr));

		object = Internal::SqlObject::Create("real");
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlSingle(324.3454)));
		Assert::AreEqual(false, object->IsValueValid(SqlTypes::SqlDouble(324.3454)));
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlSingle::Null));
		Assert::AreEqual(true, object->IsValueValid(DBNull::Value));
		Assert::AreEqual(true, object->IsValueValid(nullptr));

		object = Internal::SqlObject::Create("smalldatetime");
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlDateTime(DateTime::Now)));
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlDateTime::Null));
		Assert::AreEqual(true, object->IsValueValid(DBNull::Value));
		Assert::AreEqual(true, object->IsValueValid(nullptr));

		object = Internal::SqlObject::Create("smallint");
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlInt16(12345)));
		Assert::AreEqual(false, object->IsValueValid(SqlTypes::SqlInt32(1232345)));
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlInt16::Null));
		Assert::AreEqual(true, object->IsValueValid(DBNull::Value));
		Assert::AreEqual(true, object->IsValueValid(nullptr));

		object = Internal::SqlObject::Create("smallmoney");
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlMoney(324.3454)));
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlMoney::Null));
		Assert::AreEqual(true, object->IsValueValid(DBNull::Value));
		Assert::AreEqual(true, object->IsValueValid(nullptr));

		object = Internal::SqlObject::Create("tinyint");
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlByte(125)));
		Assert::AreEqual(false, object->IsValueValid(SqlTypes::SqlInt32(1232345)));
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlByte::Null));
		Assert::AreEqual(true, object->IsValueValid(DBNull::Value));
		Assert::AreEqual(true, object->IsValueValid(nullptr));

		object = Internal::SqlObject::Create("uniqueidentifier");
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlGuid(Guid::NewGuid())));
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlGuid::Null));
		Assert::AreEqual(true, object->IsValueValid(DBNull::Value));
		Assert::AreEqual(true, object->IsValueValid(nullptr));

		object = Internal::SqlObject::Create("varchar(max)");
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlString(String::Empty->PadLeft(8001, ' '))));
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlString::Null));
		Assert::AreEqual(true, object->IsValueValid(DBNull::Value));
		Assert::AreEqual(true, object->IsValueValid(nullptr));

		object = Internal::SqlObject::Create("nvarchar(max)");
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlString(String::Empty->PadLeft(8001, ' '))));
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlString::Null));
		Assert::AreEqual(true, object->IsValueValid(DBNull::Value));
		Assert::AreEqual(true, object->IsValueValid(nullptr));

		object = Internal::SqlObject::Create("varbinary(max)");
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlBinary(System::Text::Encoding::ASCII->GetBytes(String::Empty->PadLeft(8001, ' ')))));
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlBinary::Null));
		Assert::AreEqual(true, object->IsValueValid(DBNull::Value));
		Assert::AreEqual(true, object->IsValueValid(nullptr));

		object = Internal::SqlObject::Create("xml");
		Assert::AreEqual(true, object->IsValueValid(gcnew SqlTypes::SqlXml(Xml::XmlReader::Create(gcnew IO::StringReader("<xml><a>1</a></xml>")))));
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlXml::Null));
		Assert::AreEqual(true, object->IsValueValid(DBNull::Value));
		Assert::AreEqual(true, object->IsValueValid(nullptr));

		object = Internal::SqlObject::Create("sql_variant");
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlString("a")));
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlString("ljrehswfbeljurhytbp8i3uy74fgb")));
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlString(String::Empty->PadRight(8000))));
		Assert::AreEqual(false, object->IsValueValid(SqlTypes::SqlString(String::Empty->PadRight(8001))));
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlBinary(gcnew cli::array<Byte> { 12, 13, 15, 18, 100, 200 })));
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlBinary(gcnew cli::array<Byte> { 249 })));
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlBinary(gcnew cli::array<Byte> { 249 })));
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlBinary(System::Text::Encoding::ASCII->GetBytes(String::Empty->PadLeft(8000, ' ')))));
		Assert::AreEqual(false, object->IsValueValid(SqlTypes::SqlBinary(System::Text::Encoding::ASCII->GetBytes(String::Empty->PadLeft(8001, ' ')))));
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlInt64(327645238746345)));
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlBoolean(true)));
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlBoolean(false)));
		Assert::AreEqual(true, object->IsValueValid(DateTime(2012, 11, 30, 11, 12, 13, 101)));
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlDateTime(DateTime(2012, 11, 30, 11, 12, 13, 101))));
		Assert::AreEqual(true, object->IsValueValid(DateTimeOffset(DateTime(2012, 11, 30, 11, 12, 13, 101), TimeSpan(1, 20, 0))));
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlDecimal(23.34532)));
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlDouble(324.345435345)));
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlInt32(213213)));
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlMoney(23234.3452)));
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlSingle(32432443.0)));
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlInt16(12312)));
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlByte(255)));
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlGuid(Guid::NewGuid())));
		Assert::AreEqual(false, object->IsValueValid(gcnew SqlTypes::SqlXml(Xml::XmlReader::Create(gcnew IO::StringReader("<xml><a>1</a></xml>")))));
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlString::Null));
		Assert::AreEqual(false, object->IsValueValid(Microsoft::SqlServer::Types::SqlHierarchyId::Parse("/1/2/1/")));
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlString::Null));
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlBinary::Null));
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlInt64::Null));
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlBoolean::Null));
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlDateTime::Null));
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlDecimal::Null));
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlDouble::Null));
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlInt32::Null));
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlMoney::Null));
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlSingle::Null));
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlInt16::Null));
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlByte::Null));
		Assert::AreEqual(true, object->IsValueValid(SqlTypes::SqlGuid::Null));
		Assert::AreEqual(false, object->IsValueValid(SqlTypes::SqlXml::Null));
		Assert::AreEqual(true, object->IsValueValid(DBNull::Value));
		Assert::AreEqual(true, object->IsValueValid(nullptr));
	}

	void SqlObject::TestGettingValue() {
		// Create an object
		Internal::SqlObject^ object = Internal::SqlObject::Create();
		SqlTypes::SqlString testStringValue = SqlTypes::SqlString("teststring");
		Assert::AreEqual(testStringValue.Value, object->GetValue(testStringValue), "Actual value did not match expected value");
		Assert::AreNotEqual("dswfjknsdkfjn", object->GetValue(testStringValue), "Actual value matched non-expected value");
		Assert::AreEqual("teststring", object->GetValue(testStringValue), "Actual value did not match same value");
		Assert::AreEqual(nullptr, object->GetValue(SqlTypes::SqlString::Null), "Actual value did not match same value for nullptrs");
		Assert::AreEqual(nullptr, object->GetValue(DBNull::Value), "Actual value did not match same value for nulls");
		Assert::AreEqual(nullptr, object->GetValue(nullptr), "Actual value did not match same value for nullptrs");
		
		object = Internal::SqlObject::Create("bigint");
		SqlTypes::SqlInt64 testBigintValue = SqlTypes::SqlInt64(40000000000);
		Assert::AreEqual(testBigintValue.Value, object->GetValue(testBigintValue), "Actual value did not match expected value");
		Assert::AreNotEqual(234324, object->GetValue(testBigintValue), "Actual value matched non-expected value");
		Assert::AreEqual(40000000000, object->GetValue(testBigintValue), "Actual value did not match same value");
		Assert::AreEqual(nullptr, object->GetValue(SqlTypes::SqlInt64::Null), "Actual value did not match same value for nullptrs");
		Assert::AreEqual(nullptr, object->GetValue(DBNull::Value), "Actual value did not match same value for nulls");
		Assert::AreEqual(nullptr, object->GetValue(nullptr), "Actual value did not match same value for nullptrs");

		object = Internal::SqlObject::Create("binary");
		SqlTypes::SqlBinary testBinaryValue = SqlTypes::SqlBinary(gcnew cli::array<Byte> { 1, 2, 3, 4, 5, 6, 7, 8 });
		Assert::AreEqual(testBinaryValue.Value, object->GetValue(testBinaryValue), "Actual value did not match expected value");
		Assert::AreNotEqual(gcnew cli::array<Byte> { 34, 54, 56, 78, 87 }, object->GetValue(testBinaryValue), "Actual value matched non-expected value");
		Assert::AreEqual(gcnew cli::array<Byte> { 1, 2, 3, 4, 5, 6, 7, 8 }, object->GetValue(testBinaryValue), "Actual value did not match same value");
		Assert::AreEqual(nullptr, object->GetValue(SqlTypes::SqlBinary::Null), "Actual value did not match same value for nullptrs");
		Assert::AreEqual(nullptr, object->GetValue(DBNull::Value), "Actual value did not match same value for nulls");
		Assert::AreEqual(nullptr, object->GetValue(nullptr), "Actual value did not match same value for nullptrs");

		object = Internal::SqlObject::Create("bit");
		SqlTypes::SqlBoolean testBooleanValue = SqlTypes::SqlBoolean(true);
		Assert::AreEqual(testBooleanValue.Value, object->GetValue(testBooleanValue), "Actual value did not match expected value");
		Assert::AreNotEqual(false, object->GetValue(testBooleanValue), "Actual value matched non-expected value");
		Assert::AreEqual(true, object->GetValue(testBooleanValue), "Actual value did not match same value");
		Assert::AreEqual(nullptr, object->GetValue(SqlTypes::SqlBoolean::Null), "Actual value did not match same value for nullptrs");
		Assert::AreEqual(nullptr, object->GetValue(DBNull::Value), "Actual value did not match same value for nulls");
		Assert::AreEqual(nullptr, object->GetValue(nullptr), "Actual value did not match same value for nullptrs");

		object = Internal::SqlObject::Create("char");
		SqlTypes::SqlString testCharValue = SqlTypes::SqlString("d");
		Assert::AreEqual(testCharValue.Value, object->GetValue(testCharValue), "Actual value did not match expected value");
		Assert::AreNotEqual("e", object->GetValue(testCharValue), "Actual value matched non-expected value");
		Assert::AreEqual("d", object->GetValue(testCharValue), "Actual value did not match same value");
		Assert::AreEqual(nullptr, object->GetValue(SqlTypes::SqlString::Null), "Actual value did not match same value for nullptrs");
		Assert::AreEqual(nullptr, object->GetValue(DBNull::Value), "Actual value did not match same value for nulls");
		Assert::AreEqual(nullptr, object->GetValue(nullptr), "Actual value did not match same value for nullptrs");

		object = Internal::SqlObject::Create("date");
		DateTime date = DateTime::Now.Date;
		DateTime testDateValue = date;
		Assert::AreEqual(testDateValue, object->GetValue(testDateValue), "Actual value did not match expected value");
		Assert::AreNotEqual(DateTime::Now.AddSeconds(1), object->GetValue(testDateValue), "Actual value matched non-expected value");
		Assert::AreEqual(DateTime(date.Ticks), object->GetValue(testDateValue), "Actual value did not match same value");
		Assert::AreEqual(nullptr, object->GetValue(DBNull::Value), "Actual value did not match same value for nulls");
		Assert::AreEqual(nullptr, object->GetValue(nullptr), "Actual value did not match same value for nullptrs");


		object = Internal::SqlObject::Create("datetime");
		DateTime datetime = DateTime::Now;
		SqlTypes::SqlDateTime testDateTimeValue = SqlTypes::SqlDateTime(datetime);
		Assert::AreEqual(testDateTimeValue.Value, object->GetValue(testDateTimeValue), "Actual value did not match expected value");
		Assert::AreNotEqual(DateTime::Now.AddSeconds(2), object->GetValue(testDateTimeValue), "Actual value matched non-expected value");
		Assert::AreEqual(SqlTypes::SqlDateTime(DateTime(datetime.Ticks)).Value, object->GetValue(testDateTimeValue), "Actual value did not match same value");
		Assert::AreEqual(nullptr, object->GetValue(SqlTypes::SqlDateTime::Null), "Actual value did not match same value for nullptrs");
		Assert::AreEqual(nullptr, object->GetValue(DBNull::Value), "Actual value did not match same value for nulls");
		Assert::AreEqual(nullptr, object->GetValue(nullptr), "Actual value did not match same value for nullptrs");

		object = Internal::SqlObject::Create("datetime2");
		DateTime datetime2 = DateTime::Now;
		DateTime testDateTime2Value = datetime2;
		Assert::AreEqual(testDateTime2Value, object->GetValue(testDateTime2Value), "Actual value did not match expected value");
		Assert::AreNotEqual(DateTime::Now.AddSeconds(1), object->GetValue(testDateTime2Value), "Actual value matched non-expected value");
		Assert::AreEqual(DateTime(datetime2.Ticks), object->GetValue(testDateTime2Value), "Actual value did not match same value");
		Assert::AreEqual(nullptr, object->GetValue(DBNull::Value), "Actual value did not match same value for nulls");
		Assert::AreEqual(nullptr, object->GetValue(nullptr), "Actual value did not match same value for nullptrs");

		object = Internal::SqlObject::Create("time");
		DateTime time = DateTime::Now;
		DateTime testTimeValue = time;
		Assert::AreEqual(testTimeValue, object->GetValue(testTimeValue), "Actual value did not match expected value");
		Assert::AreNotEqual(DateTime::Now.AddSeconds(1), object->GetValue(testTimeValue), "Actual value matched non-expected value");
		Assert::AreEqual(DateTime(time.Ticks), object->GetValue(testTimeValue), "Actual value did not match same value");
		Assert::AreEqual(nullptr, object->GetValue(DBNull::Value), "Actual value did not match same value for nulls");
		Assert::AreEqual(nullptr, object->GetValue(nullptr), "Actual value did not match same value for nullptrs");

		object = Internal::SqlObject::Create("datetimeoffset");
		DateTimeOffset datetimeoffset = DateTimeOffset::Now;
		DateTimeOffset testDateTimeOffsetValue = datetimeoffset;
		Assert::AreEqual(testDateTimeOffsetValue, object->GetValue(testDateTimeOffsetValue), "Actual value did not match expected value");
		Assert::AreNotEqual(DateTimeOffset::Now.AddSeconds(1), object->GetValue(testDateTimeOffsetValue), "Actual value matched non-expected value");
		Assert::AreEqual(DateTimeOffset(DateTime(datetimeoffset.Ticks)), object->GetValue(testDateTimeOffsetValue), "Actual value did not match same value");
		Assert::AreEqual(nullptr, object->GetValue(DBNull::Value), "Actual value did not match same value for nulls");
		Assert::AreEqual(nullptr, object->GetValue(nullptr), "Actual value did not match same value for nullptrs");

		object = Internal::SqlObject::Create("decimal");
		SqlTypes::SqlDecimal testDecimalValue = SqlTypes::SqlDecimal(1.23456);
		Assert::AreEqual(testDecimalValue.Value, object->GetValue(testDecimalValue), "Actual value did not match expected value");
		Assert::AreNotEqual(6.9, object->GetValue(testDecimalValue), "Actual value matched non-expected value");
		Assert::AreEqual(1.23456, object->GetValue(testDecimalValue), "Actual value did not match same value");
		Assert::AreEqual(nullptr, object->GetValue(SqlTypes::SqlDecimal::Null), "Actual value did not match same value for nullptrs");
		Assert::AreEqual(nullptr, object->GetValue(DBNull::Value), "Actual value did not match same value for nulls");
		Assert::AreEqual(nullptr, object->GetValue(nullptr), "Actual value did not match same value for nullptrs");

		object = Internal::SqlObject::Create("float");
		SqlTypes::SqlDouble testDoubleValue = SqlTypes::SqlDouble(2.34567);
		Assert::AreEqual(testDoubleValue.Value, object->GetValue(testDoubleValue), "Actual value did not match expected value");
		Assert::AreNotEqual(2.9, object->GetValue(testDoubleValue), "Actual value matched non-expected value");
		Assert::AreEqual(System::Convert::ToDouble(2.34567), object->GetValue(testDoubleValue), "Actual value did not match same value");
		Assert::AreEqual(nullptr, object->GetValue(SqlTypes::SqlDouble::Null), "Actual value did not match same value for nullptrs");
		Assert::AreEqual(nullptr, object->GetValue(DBNull::Value), "Actual value did not match same value for nulls");
		Assert::AreEqual(nullptr, object->GetValue(nullptr), "Actual value did not match same value for nullptrs");

		object = Internal::SqlObject::Create("int");
		SqlTypes::SqlInt32 testInt32Value = SqlTypes::SqlInt32(1000000000);
		Assert::AreEqual(testInt32Value.Value, object->GetValue(testInt32Value), "Actual value did not match expected value");
		Assert::AreNotEqual(878687687, object->GetValue(testInt32Value), "Actual value matched non-expected value");
		Assert::AreEqual(1000000000, object->GetValue(testInt32Value), "Actual value did not match same value");
		Assert::AreEqual(nullptr, object->GetValue(SqlTypes::SqlInt32::Null), "Actual value did not match same value for nullptrs");
		Assert::AreEqual(nullptr, object->GetValue(DBNull::Value), "Actual value did not match same value for nulls");
		Assert::AreEqual(nullptr, object->GetValue(nullptr), "Actual value did not match same value for nullptrs");

		object = Internal::SqlObject::Create("money");
		SqlTypes::SqlMoney testMoneyValue = SqlTypes::SqlMoney(34567.5467);
		Assert::AreEqual(testMoneyValue.Value, object->GetValue(testMoneyValue), "Actual value did not match expected value");
		Assert::AreNotEqual(99.99, object->GetValue(testMoneyValue), "Actual value matched non-expected value");
		Assert::AreEqual(34567.5467, object->GetValue(testMoneyValue), "Actual value did not match same value");
		Assert::AreEqual(nullptr, object->GetValue(SqlTypes::SqlMoney::Null), "Actual value did not match same value for nullptrs");
		Assert::AreEqual(nullptr, object->GetValue(DBNull::Value), "Actual value did not match same value for nulls");
		Assert::AreEqual(nullptr, object->GetValue(nullptr), "Actual value did not match same value for nullptrs");

		object = Internal::SqlObject::Create("nchar");
		SqlTypes::SqlString testNCharValue = SqlTypes::SqlString("jhkdsbfsdfkljn");
		Assert::AreEqual(testNCharValue.Value, object->GetValue(testNCharValue), "Actual value did not match expected value");
		Assert::AreNotEqual("dsfjknsdf", object->GetValue(testNCharValue), "Actual value matched non-expected value");
		Assert::AreEqual("jhkdsbfsdfkljn", object->GetValue(testNCharValue), "Actual value did not match same value");
		Assert::AreEqual(nullptr, object->GetValue(SqlTypes::SqlString::Null), "Actual value did not match same value for nullptrs");
		Assert::AreEqual(nullptr, object->GetValue(DBNull::Value), "Actual value did not match same value for nulls");
		Assert::AreEqual(nullptr, object->GetValue(nullptr), "Actual value did not match same value for nullptrs");

		object = Internal::SqlObject::Create("numeric");
		SqlTypes::SqlDecimal testNumericValue = SqlTypes::SqlDecimal(892734.987324);
		Assert::AreEqual(testNumericValue.Value, object->GetValue(testNumericValue), "Actual value did not match expected value");
		Assert::AreNotEqual(9898.9898, object->GetValue(testNumericValue), "Actual value matched non-expected value");
		Assert::AreEqual(892734.987324, object->GetValue(testNumericValue), "Actual value did not match same value");
		Assert::AreEqual(nullptr, object->GetValue(SqlTypes::SqlDecimal::Null), "Actual value did not match same value for nullptrs");
		Assert::AreEqual(nullptr, object->GetValue(DBNull::Value), "Actual value did not match same value for nulls");
		Assert::AreEqual(nullptr, object->GetValue(nullptr), "Actual value did not match same value for nullptrs");

		object = Internal::SqlObject::Create("nvarchar");
		SqlTypes::SqlString testNVarcharValue = SqlTypes::SqlString("sdfuihbesfriub");
		Assert::AreEqual(testNVarcharValue.Value, object->GetValue(testNVarcharValue), "Actual value did not match expected value");
		Assert::AreNotEqual("edkjlfnss", object->GetValue(testNVarcharValue), "Actual value matched non-expected value");
		Assert::AreEqual("sdfuihbesfriub", object->GetValue(testNVarcharValue), "Actual value did not match same value");
		Assert::AreEqual(nullptr, object->GetValue(SqlTypes::SqlString::Null), "Actual value did not match same value for nullptrs");
		Assert::AreEqual(nullptr, object->GetValue(DBNull::Value), "Actual value did not match same value for nulls");
		Assert::AreEqual(nullptr, object->GetValue(nullptr), "Actual value did not match same value for nullptrs");

		object = Internal::SqlObject::Create("real");
		SqlTypes::SqlSingle testRealValue = SqlTypes::SqlSingle(213.3425);
		Assert::AreEqual(testRealValue.Value, object->GetValue(testRealValue), "Actual value did not match expected value");
		Assert::AreNotEqual(3434.34344, object->GetValue(testRealValue), "Actual value matched non-expected value");
		Assert::AreEqual(System::Convert::ToSingle(213.3425), object->GetValue(testRealValue), "Actual value did not match same value");
		Assert::AreEqual(nullptr, object->GetValue(SqlTypes::SqlSingle::Null), "Actual value did not match same value for nullptrs");
		Assert::AreEqual(nullptr, object->GetValue(DBNull::Value), "Actual value did not match same value for nulls");
		Assert::AreEqual(nullptr, object->GetValue(nullptr), "Actual value did not match same value for nullptrs");

		object = Internal::SqlObject::Create("smalldatetime");
		DateTime smalldatetime = DateTime::Now;
		SqlTypes::SqlDateTime testSmallDateTimeValue = SqlTypes::SqlDateTime(smalldatetime);
		Assert::AreEqual(testSmallDateTimeValue.Value, object->GetValue(testSmallDateTimeValue), "Actual value did not match expected value");
		Assert::AreNotEqual(DateTime::Now.AddSeconds(3), object->GetValue(testSmallDateTimeValue), "Actual value matched non-expected value");
		Assert::AreEqual(SqlTypes::SqlDateTime(DateTime(smalldatetime.Ticks)).Value, object->GetValue(testSmallDateTimeValue), "Actual value did not match same value");
		Assert::AreEqual(nullptr, object->GetValue(SqlTypes::SqlDateTime::Null), "Actual value did not match same value for nullptrs");
		Assert::AreEqual(nullptr, object->GetValue(DBNull::Value), "Actual value did not match same value for nulls");
		Assert::AreEqual(nullptr, object->GetValue(nullptr), "Actual value did not match same value for nullptrs");

		object = Internal::SqlObject::Create("smallint");
		SqlTypes::SqlInt16 testSmallIntValue = SqlTypes::SqlInt16(324);
		Assert::AreEqual(testSmallIntValue.Value, object->GetValue(testSmallIntValue), "Actual value did not match expected value");
		Assert::AreNotEqual(9, object->GetValue(testSmallIntValue), "Actual value matched non-expected value");
		Assert::AreEqual(324, object->GetValue(testSmallIntValue), "Actual value did not match same value");
		Assert::AreEqual(nullptr, object->GetValue(SqlTypes::SqlInt16::Null), "Actual value did not match same value for nullptrs");
		Assert::AreEqual(nullptr, object->GetValue(DBNull::Value), "Actual value did not match same value for nulls");
		Assert::AreEqual(nullptr, object->GetValue(nullptr), "Actual value did not match same value for nullptrs");

		object = Internal::SqlObject::Create("smallmoney");
		SqlTypes::SqlMoney testSmallMoneyValue = SqlTypes::SqlMoney(234.2342);
		Assert::AreEqual(testSmallMoneyValue.Value, object->GetValue(testSmallMoneyValue), "Actual value did not match expected value");
		Assert::AreNotEqual(2.2345, object->GetValue(testSmallMoneyValue), "Actual value matched non-expected value");
		Assert::AreEqual(234.2342, object->GetValue(testSmallMoneyValue), "Actual value did not match same value");
		Assert::AreEqual(nullptr, object->GetValue(SqlTypes::SqlMoney::Null), "Actual value did not match same value for nullptrs");
		Assert::AreEqual(nullptr, object->GetValue(DBNull::Value), "Actual value did not match same value for nulls");
		Assert::AreEqual(nullptr, object->GetValue(nullptr), "Actual value did not match same value for nullptrs");

		object = Internal::SqlObject::Create("tinyint");
		SqlTypes::SqlByte testTinyintValue = SqlTypes::SqlByte(255);
		Assert::AreEqual(testTinyintValue.Value, object->GetValue(testTinyintValue), "Actual value did not match expected value");
		Assert::AreNotEqual(7, object->GetValue(testTinyintValue), "Actual value matched non-expected value");
		Assert::AreEqual(255, object->GetValue(testTinyintValue), "Actual value did not match same value");
		Assert::AreEqual(nullptr, object->GetValue(SqlTypes::SqlByte::Null), "Actual value did not match same value for nullptrs");
		Assert::AreEqual(nullptr, object->GetValue(DBNull::Value), "Actual value did not match same value for nulls");
		Assert::AreEqual(nullptr, object->GetValue(nullptr), "Actual value did not match same value for nullptrs");

		object = Internal::SqlObject::Create("uniqueidentifier");
		System::Guid guid = System::Guid::NewGuid();
		SqlTypes::SqlGuid testUniqueIdentifierValue = SqlTypes::SqlGuid(guid);
		Assert::AreEqual(testUniqueIdentifierValue.Value, object->GetValue(testUniqueIdentifierValue), "Actual value did not match expected value");
		Assert::AreNotEqual(System::Guid::NewGuid(), object->GetValue(testUniqueIdentifierValue), "Actual value matched non-expected value");
		Assert::AreEqual(System::Guid::Guid(guid.ToString()), object->GetValue(testUniqueIdentifierValue), "Actual value did not match same value");
		Assert::AreEqual(nullptr, object->GetValue(SqlTypes::SqlGuid::Null), "Actual value did not match same value for nullptrs");
		Assert::AreEqual(nullptr, object->GetValue(DBNull::Value), "Actual value did not match same value for nulls");
		Assert::AreEqual(nullptr, object->GetValue(nullptr), "Actual value did not match same value for nullptrs");

		object = Internal::SqlObject::Create("varbinary");
		SqlTypes::SqlBinary testVarbinaryValue = SqlTypes::SqlBinary(gcnew cli::array<Byte> { 34, 23, 54, 32, 56, 213, 128, 32, 45, 65, 67, 78, 32, 45 });
		Assert::AreEqual(testVarbinaryValue.Value, object->GetValue(testVarbinaryValue), "Actual value did not match expected value");
		Assert::AreNotEqual(gcnew cli::array<Byte> { 1, 2 }, object->GetValue(testVarbinaryValue), "Actual value matched non-expected value");
		Assert::AreEqual(gcnew cli::array<Byte> { 34, 23, 54, 32, 56, 213, 128, 32, 45, 65, 67, 78, 32, 45 }, object->GetValue(testVarbinaryValue), "Actual value did not match same value");
		Assert::AreEqual(nullptr, object->GetValue(SqlTypes::SqlBinary::Null), "Actual value did not match same value for nullptrs");
		Assert::AreEqual(nullptr, object->GetValue(DBNull::Value), "Actual value did not match same value for nulls");
		Assert::AreEqual(nullptr, object->GetValue(nullptr), "Actual value did not match same value for nullptrs");

		object = Internal::SqlObject::Create("varchar");
		SqlTypes::SqlString testVarcharValue = SqlTypes::SqlString("l34k2n5moik234jn590fsd-");
		Assert::AreEqual(testVarcharValue.Value, object->GetValue(testVarcharValue), "Actual value did not match expected value");
		Assert::AreNotEqual("erkjfnbderskjfn", object->GetValue(testVarcharValue), "Actual value matched non-expected value");
		Assert::AreEqual("l34k2n5moik234jn590fsd-", object->GetValue(testVarcharValue), "Actual value did not match same value");
		Assert::AreEqual(nullptr, object->GetValue(SqlTypes::SqlString::Null), "Actual value did not match same value for nullptrs");
		Assert::AreEqual(nullptr, object->GetValue(DBNull::Value), "Actual value did not match same value for nulls");
		Assert::AreEqual(nullptr, object->GetValue(nullptr), "Actual value did not match same value for nullptrs");

		object = Internal::SqlObject::Create("xml");
		SqlTypes::SqlXml^ testXmlValue = gcnew SqlTypes::SqlXml(Xml::XmlReader::Create(gcnew IO::StringReader("<xml><a>1</a></xml>")));
		Assert::AreEqual(testXmlValue->Value, object->GetValue(testXmlValue), "Actual value did not match expected value");
		Assert::AreNotEqual("<xml><a>2</a></xml>", object->GetValue(testXmlValue), "Actual value matched non-expected value");
		Assert::AreEqual("<xml><a>1</a></xml>", object->GetValue(testXmlValue), "Actual value did not match same value");
		Assert::AreEqual(nullptr, object->GetValue(SqlTypes::SqlXml::Null), "Actual value did not match same value for nullptrs");
		Assert::AreEqual(nullptr, object->GetValue(DBNull::Value), "Actual value did not match same value for nulls");
		Assert::AreEqual(nullptr, object->GetValue(nullptr), "Actual value did not match same value for nullptrs");

		object = Internal::SqlObject::Create("sql_variant");
		Assert::AreEqual("erkjfnbderskjfn", object->GetValue(SqlTypes::SqlString("erkjfnbderskjfn")), "Actual value matched non-expected value");
		Assert::AreEqual(234324234, object->GetValue(SqlTypes::SqlInt64(234324234)), "Actual value matched non-expected value");
		Assert::AreEqual(gcnew cli::array<Byte> { 12, 13, 14, 15, 165 }, object->GetValue(SqlTypes::SqlBinary(gcnew cli::array<Byte> { 12, 13, 14, 15, 165 })), "Actual value matched non-expected value");
		Assert::AreEqual(true, object->GetValue(SqlTypes::SqlBoolean(true)), "Actual value matched non-expected value");
		Assert::AreEqual(false, object->GetValue(SqlTypes::SqlBoolean(false)), "Actual value matched non-expected value");
		Assert::AreEqual(DateTime(2010, 10, 12, 11, 10, 9, 201), object->GetValue(DateTime(2010, 10, 12, 11, 10, 9, 201)), "Actual value matched non-expected value");
		Assert::AreEqual(DateTime(2010, 10, 12, 11, 10, 9, 200), object->GetValue(SqlTypes::SqlDateTime(DateTime(2010, 10, 12, 11, 10, 9, 200))), "Actual value matched non-expected value");
		Assert::AreEqual(DateTimeOffset(DateTime(2010, 10, 12, 11, 10, 9, 201), TimeSpan(1, 0, 0)), object->GetValue(DateTimeOffset(DateTime(2010, 10, 12, 11, 10, 9, 201), TimeSpan(1, 0, 0))), "Actual value matched non-expected value");
		Assert::AreEqual(123.1235, object->GetValue(SqlTypes::SqlDecimal(123.1235)), "Actual value matched non-expected value");
		Assert::AreEqual(234324.3453245435, object->GetValue(SqlTypes::SqlDouble(234324.3453245435)), "Actual value matched non-expected value");
		Assert::AreEqual(23434654, object->GetValue(SqlTypes::SqlInt32(23434654)), "Actual value matched non-expected value");
		Assert::AreEqual(123234.3456, object->GetValue(SqlTypes::SqlMoney(123234.3456)), "Actual value matched non-expected value");
		Assert::AreEqual(234.345f, object->GetValue(SqlTypes::SqlSingle(234.345)), "Actual value matched non-expected value");
		Assert::AreEqual(12332, object->GetValue(SqlTypes::SqlInt16(12332)), "Actual value matched non-expected value");
		Assert::AreEqual(123, object->GetValue(SqlTypes::SqlByte(123)), "Actual value matched non-expected value");
		Assert::AreEqual(testUniqueIdentifierValue.Value, object->GetValue(testUniqueIdentifierValue), "Actual value matched non-expected value");
		Assert::AreNotEqual(Guid::NewGuid(), object->GetValue(testUniqueIdentifierValue), "Actual value matched non-expected value");
		Assert::AreEqual(nullptr, object->GetValue(SqlTypes::SqlString::Null), "Actual value did not match same value for nullptrs");
		Assert::AreEqual(nullptr, object->GetValue(SqlTypes::SqlBinary::Null), "Actual value did not match same value for nullptrs");
		Assert::AreEqual(nullptr, object->GetValue(SqlTypes::SqlInt64::Null), "Actual value did not match same value for nullptrs");
		Assert::AreEqual(nullptr, object->GetValue(SqlTypes::SqlBoolean::Null), "Actual value did not match same value for nullptrs");
		Assert::AreEqual(nullptr, object->GetValue(SqlTypes::SqlDecimal::Null), "Actual value did not match same value for nullptrs");
		Assert::AreEqual(nullptr, object->GetValue(SqlTypes::SqlDouble::Null), "Actual value did not match same value for nullptrs");
		Assert::AreEqual(nullptr, object->GetValue(SqlTypes::SqlInt32::Null), "Actual value did not match same value for nullptrs");
		Assert::AreEqual(nullptr, object->GetValue(SqlTypes::SqlMoney::Null), "Actual value did not match same value for nullptrs");
		Assert::AreEqual(nullptr, object->GetValue(SqlTypes::SqlSingle::Null), "Actual value did not match same value for nullptrs");
		Assert::AreEqual(nullptr, object->GetValue(SqlTypes::SqlInt16::Null), "Actual value did not match same value for nullptrs");
		Assert::AreEqual(nullptr, object->GetValue(SqlTypes::SqlByte::Null), "Actual value did not match same value for nullptrs");
		Assert::AreEqual(nullptr, object->GetValue(SqlTypes::SqlGuid::Null), "Actual value did not match same value for nullptrs");
		Assert::AreEqual(nullptr, object->GetValue(SqlTypes::SqlXml::Null), "Actual value did not match same value for nullptrs");
		Assert::AreEqual(nullptr, object->GetValue(DBNull::Value), "Actual value did not match same value for nulls");
		Assert::AreEqual(nullptr, object->GetValue(nullptr), "Actual value did not match same value for nullptrs");
	}

	void SqlObject::TestParsing() {
		// Create an object
		Internal::SqlObject^ object = Internal::SqlObject::Create();
		SqlTypes::SqlString testStringValue = SqlTypes::SqlString("teststring");
		Assert::AreEqual(SqlTypes::SqlString("teststring"), object->Parse(testStringValue.Value), "Actual value did not match expected value");
		Assert::AreNotEqual(SqlTypes::SqlString("teststring2"), object->Parse(testStringValue.Value), "Actual value matched non-expected value");
		Assert::AreEqual(SqlTypes::SqlString::Null, object->Parse(nullptr), "Actual value did not match expected null value");
		Assert::AreEqual(SqlTypes::SqlString(String::Empty), object->Parse(String::Empty), "Actual value did not match expected null value");

		object = Internal::SqlObject::Create("bigint");
		SqlTypes::SqlInt64 testBigintValue = SqlTypes::SqlInt64(40000000000);
		Assert::AreEqual(testBigintValue, object->Parse(testBigintValue.ToString()), "Actual value did not match expected value");
		Assert::AreNotEqual(SqlTypes::SqlInt64(1234213213), object->Parse(testBigintValue.ToString()), "Actual value matched non-expected value");
		Assert::AreEqual(SqlTypes::SqlInt64::Null, object->Parse(nullptr), "Actual value did not match expected null value");

		object = Internal::SqlObject::Create("binary");
		SqlTypes::SqlBinary testBinaryValue = SqlTypes::SqlBinary(gcnew cli::array<Byte> { 1, 2, 3, 4, 5, 6, 7, 8 });
		Assert::AreEqual(testBinaryValue, object->Parse("0x0102030405060708"), "Actual value did not match expected value");
		Assert::AreNotEqual(SqlTypes::SqlBinary(gcnew cli::array<Byte> { 1, 2, 3, 4, 5, 6, 7, 7 }), object->Parse("0x0102030405060708"), "Actual value matched non-expected value");
		Assert::AreEqual(SqlTypes::SqlBinary::Null, object->Parse(nullptr), "Actual value did not match expected null value");
		Assert::AreEqual(SqlTypes::SqlBinary(gcnew cli::array<Byte> { 0 }), object->Parse(String::Empty), "Actual value did not match expected null value");

		object = Internal::SqlObject::Create("bit");
		SqlTypes::SqlBoolean testBooleanValue = SqlTypes::SqlBoolean(true);
		Assert::AreEqual(testBooleanValue, object->Parse("True"), "Actual value did not match expected value");
		Assert::AreEqual(testBooleanValue, object->Parse("true"), "Actual value did not match expected value");
		Assert::AreEqual(testBooleanValue, object->Parse("tRuE"), "Actual value did not match expected value");
		Assert::AreEqual(testBooleanValue, object->Parse("1"), "Actual value did not match expected value");
		testBooleanValue = SqlTypes::SqlBoolean(false);
		Assert::AreEqual(testBooleanValue, object->Parse("False"), "Actual value did not match expected value");
		Assert::AreEqual(testBooleanValue, object->Parse("false"), "Actual value did not match expected value");
		Assert::AreEqual(testBooleanValue, object->Parse("fAlSE"), "Actual value did not match expected value");
		Assert::AreEqual(testBooleanValue, object->Parse("0"), "Actual value did not match expected value");
		Assert::AreNotEqual(testBooleanValue, object->Parse("True"), "Actual value matched non-expected value");
		Assert::AreEqual(SqlTypes::SqlBoolean::Null, object->Parse(nullptr), "Actual value did not match expected null value");

		object = Internal::SqlObject::Create("char");
		SqlTypes::SqlString testCharValue = SqlTypes::SqlString("d");
		Assert::AreEqual(testCharValue, object->Parse("d"), "Actual value did not match expected value");
		Assert::AreNotEqual(testCharValue, object->Parse("e"), "Actual value matched non-expected value");
		Assert::AreEqual(SqlTypes::SqlString::Null, object->Parse(nullptr), "Actual value did not match expected null value");

		object = Internal::SqlObject::Create("date");
		DateTime date = DateTime::Now.Date;
		DateTime testDateValue = date;
		Assert::AreEqual(testDateValue, object->Parse(date.ToString()), "Actual value did not match expected value");
		Assert::AreEqual(testDateValue, object->Parse(date.ToString("dd MMM yyyy")), "Actual value did not match expected value");
		Assert::AreEqual(testDateValue, object->Parse(date.ToString("ddd dd MMMM yyyy")), "Actual value did not match expected value");
		Assert::AreEqual(testDateValue, object->Parse(date.ToString("dddd dd MMMM yyyy")), "Actual value did not match expected value");
		Assert::AreNotEqual(testDateValue, object->Parse(date.AddDays(1).ToString()), "Actual value matched non-expected value");
		Assert::AreEqual(DBNull::Value, object->Parse(nullptr), "Actual value did not match expected null value");

		object = Internal::SqlObject::Create("datetime");
		DateTime datetime = DateTime::Now;
		SqlTypes::SqlDateTime testDateTimeValue = SqlTypes::SqlDateTime(datetime);
		Assert::AreEqual(testDateTimeValue, object->Parse(datetime.ToString("yyyy-MM-dd HH:mm:ss.fffffff")), "Actual value did not match expected value");
		Assert::AreNotEqual(testDateTimeValue, object->Parse(datetime.AddDays(1).ToString("yyyy-MM-dd HH:mm:ss.fffffff")), "Actual value matched non-expected value");
		Assert::AreEqual(SqlTypes::SqlDateTime::Null, object->Parse(nullptr), "Actual value did not match expected null value");

		object = Internal::SqlObject::Create("datetime2");
		DateTime datetime2 = DateTime::Now;
		DateTime testDateTime2Value = datetime2;
		Assert::AreEqual(testDateTime2Value, object->Parse(datetime2.ToString("yyyy-MM-dd HH:mm:ss.fffffff")), "Actual value did not match expected value");
		Assert::AreNotEqual(testDateTime2Value, object->Parse(datetime2.AddDays(1).ToString("yyyy-MM-dd HH:mm:ss.fffffff")), "Actual value matched non-expected value");
		Assert::AreEqual(DBNull::Value, object->Parse(nullptr), "Actual value did not match expected null value");

		object = Internal::SqlObject::Create("time");
		DateTime time = DateTime::Parse(DateTime::Now.ToString("HH:mm:ss.fffffff"));
		DateTime time2 = DateTime::Parse(DateTime::Now.ToString("HH:mm:ss"));
		DateTime testTimeValue = time;
		Assert::AreEqual(testTimeValue, object->Parse(time.ToString("HH:mm:ss.fffffff")), "Actual value did not match expected value");
		Assert::AreEqual(time2, object->Parse(time2.ToString("HH:mm:ss")), "Actual value did not match expected value");
		Assert::AreNotEqual(testTimeValue, object->Parse(time.AddHours(1).ToString("HH:mm:ss.fffffff")), "Actual value matched non-expected value");
		Assert::AreEqual(DBNull::Value, object->Parse(nullptr), "Actual value did not match expected null value");

		object = Internal::SqlObject::Create("datetimeoffset");
		DateTimeOffset datetimeoffset = DateTimeOffset::Now;
		DateTimeOffset testDateTimeOffsetValue = datetimeoffset;
		Assert::AreEqual(testDateTimeOffsetValue, object->Parse(datetimeoffset.ToString("yyyy-MM-dd HH:mm:ss.fffffff zzzz")), "Actual value did not match expected value");
		Assert::AreNotEqual(testDateTimeOffsetValue, object->Parse(datetimeoffset.AddHours(1).ToString("yyyy-MM-dd HH:mm:ss.fffffff zzzz")), "Actual value matched non-expected value");
		Assert::AreEqual(DBNull::Value, object->Parse(nullptr), "Actual value did not match expected null value");

		object = Internal::SqlObject::Create("decimal");
		SqlTypes::SqlDecimal testDecimalValue = SqlTypes::SqlDecimal::Parse("1.23456");
		Assert::AreEqual(testDecimalValue, object->Parse("1.23456"), "Actual value did not match expected value");
		Assert::AreNotEqual(testDecimalValue, object->Parse("1.23466"), "Actual value matched non-expected value");
		Assert::AreEqual(SqlTypes::SqlDecimal::Null, object->Parse(nullptr), "Actual value did not match expected null value");

		object = Internal::SqlObject::Create("float");
		SqlTypes::SqlDouble testDoubleValue = SqlTypes::SqlDouble(2.34567);
		Assert::AreEqual(testDoubleValue, object->Parse("2.34567"), "Actual value did not match expected value");
		Assert::AreNotEqual(testDoubleValue, object->Parse("2.34568"), "Actual value matched non-expected value");
		Assert::AreEqual(SqlTypes::SqlDouble::Null, object->Parse(nullptr), "Actual value did not match expected null value");

		object = Internal::SqlObject::Create("int");
		SqlTypes::SqlInt32 testInt32Value = SqlTypes::SqlInt32(1000000000);
		Assert::AreEqual(testInt32Value, object->Parse("1000000000"), "Actual value did not match expected value");
		Assert::AreNotEqual(testInt32Value, object->Parse("1000010000"), "Actual value matched non-expected value");
		Assert::AreEqual(SqlTypes::SqlInt32::Null, object->Parse(nullptr), "Actual value did not match expected null value");

		object = Internal::SqlObject::Create("money");
		SqlTypes::SqlMoney testMoneyValue = SqlTypes::SqlMoney(34567.5467);
		Assert::AreEqual(testMoneyValue, object->Parse("34567.5467"), "Actual value did not match expected value");
		Assert::AreNotEqual(testMoneyValue, object->Parse("33567.5467"), "Actual value matched non-expected value");
		Assert::AreEqual(SqlTypes::SqlMoney::Null, object->Parse(nullptr), "Actual value did not match expected null value");

		object = Internal::SqlObject::Create("nchar");
		SqlTypes::SqlString testNCharValue = SqlTypes::SqlString("jhkdsbfsdfkljn");
		Assert::AreEqual(testNCharValue, object->Parse("jhkdsbfsdfkljn"), "Actual value did not match expected value");
		Assert::AreNotEqual(testNCharValue, object->Parse("jhkdsbfsdfklfn"), "Actual value matched non-expected value");
		Assert::AreEqual(SqlTypes::SqlString::Null, object->Parse(nullptr), "Actual value did not match expected null value");

		object = Internal::SqlObject::Create("numeric");
		SqlTypes::SqlDecimal testNumericValue = SqlTypes::SqlDecimal::Parse("892734.987324");
		Assert::AreEqual(testNumericValue, object->Parse("892734.987324"), "Actual value did not match expected value");
		Assert::AreNotEqual(testNumericValue, object->Parse("996723.997324"), "Actual value matched non-expected value");
		Assert::AreEqual(SqlTypes::SqlDecimal::Null, object->Parse(nullptr), "Actual value did not match expected null value");

		object = Internal::SqlObject::Create("nvarchar");
		SqlTypes::SqlString testNVarcharValue = SqlTypes::SqlString("sdfuihbesfriub");
		Assert::AreEqual(testNVarcharValue, object->Parse("sdfuihbesfriub"), "Actual value did not match expected value");
		Assert::AreNotEqual(testNVarcharValue, object->Parse("zedkfjnsedifuhieujrnw"), "Actual value matched non-expected value");
		Assert::AreEqual(SqlTypes::SqlString::Null, object->Parse(nullptr), "Actual value did not match expected null value");

		object = Internal::SqlObject::Create("real");
		SqlTypes::SqlSingle testRealValue = SqlTypes::SqlSingle(213.3425);
		Assert::AreEqual(testRealValue, object->Parse("213.3425"), "Actual value did not match expected value");
		Assert::AreNotEqual(testRealValue, object->Parse("23.34543"), "Actual value matched non-expected value");
		Assert::AreEqual(SqlTypes::SqlSingle::Null, object->Parse(nullptr), "Actual value did not match expected null value");

		object = Internal::SqlObject::Create("smalldatetime");
		DateTime smalldatetime = DateTime::Now;
		SqlTypes::SqlDateTime testSmallDateTimeValue = SqlTypes::SqlDateTime(smalldatetime);
		Assert::AreEqual(testSmallDateTimeValue, object->Parse(smalldatetime.ToString("yyyy-MM-dd HH:mm:ss.fffffff")), "Actual value did not match expected value");
		Assert::AreNotEqual(testSmallDateTimeValue, object->Parse(smalldatetime.AddDays(1).ToString("yyyy-MM-dd HH:mm:ss.fffffff")), "Actual value matched non-expected value");
		Assert::AreEqual(SqlTypes::SqlDateTime::Null, object->Parse(nullptr), "Actual value did not match expected null value");

		object = Internal::SqlObject::Create("smallint");
		SqlTypes::SqlInt16 testSmallIntValue = SqlTypes::SqlInt16(324);
		Assert::AreEqual(testSmallIntValue, object->Parse("324"), "Actual value did not match expected value");
		Assert::AreNotEqual(testSmallIntValue, object->Parse("281"), "Actual value matched non-expected value");
		Assert::AreEqual(SqlTypes::SqlInt16::Null, object->Parse(nullptr), "Actual value did not match expected null value");

		object = Internal::SqlObject::Create("smallmoney");
		SqlTypes::SqlMoney testSmallMoneyValue = SqlTypes::SqlMoney(234.2342);
		Assert::AreEqual(testSmallMoneyValue, object->Parse("234.2342"), "Actual value did not match expected value");
		Assert::AreNotEqual(testSmallMoneyValue, object->Parse("1.023"), "Actual value matched non-expected value");
		Assert::AreEqual(SqlTypes::SqlMoney::Null, object->Parse(nullptr), "Actual value did not match expected null value");

		object = Internal::SqlObject::Create("tinyint");
		SqlTypes::SqlByte testTinyintValue = SqlTypes::SqlByte(255);
		Assert::AreEqual(testTinyintValue, object->Parse("255"), "Actual value did not match expected value");
		Assert::AreNotEqual(testTinyintValue, object->Parse("128"), "Actual value matched non-expected value");
		Assert::AreEqual(SqlTypes::SqlByte::Null, object->Parse(nullptr), "Actual value did not match expected null value");

		object = Internal::SqlObject::Create("uniqueidentifier");
		System::Guid guid = System::Guid::NewGuid();
		SqlTypes::SqlGuid testUniqueIdentifierValue = SqlTypes::SqlGuid(guid);
		Assert::AreEqual(testUniqueIdentifierValue, object->Parse(guid.ToString()), "Actual value did not match expected value");
		Assert::AreNotEqual(testUniqueIdentifierValue, object->Parse(System::Guid::NewGuid().ToString()), "Actual value matched non-expected value");
		Assert::AreEqual(SqlTypes::SqlGuid::Null, object->Parse(nullptr), "Actual value did not match expected null value");

		object = Internal::SqlObject::Create("varbinary");
		SqlTypes::SqlBinary testVarbinaryValue = SqlTypes::SqlBinary(gcnew cli::array<Byte> { 34, 23, 54, 32, 56, 213, 128, 32, 45, 65, 67, 78, 32, 45 });
		Assert::AreEqual(testVarbinaryValue, object->Parse("0x2217362038D580202D41434E202D"), "Actual value did not match expected value");
		Assert::AreNotEqual(testVarbinaryValue, object->Parse("0x2217362038D5F0202D41434E202D"), "Actual value matched non-expected value");
		Assert::AreEqual(SqlTypes::SqlBinary::Null, object->Parse(nullptr), "Actual value did not match expected null value");

		object = Internal::SqlObject::Create("varchar");
		SqlTypes::SqlString testVarcharValue = SqlTypes::SqlString("l34k2n5moik234jn590fsd-");
		Assert::AreEqual(testVarcharValue, object->Parse("l34k2n5moik234jn590fsd-"), "Actual value did not match expected value");
		Assert::AreNotEqual(testVarcharValue, object->Parse("kjernfkjnr8945iojnf"), "Actual value matched non-expected value");
		Assert::AreEqual(SqlTypes::SqlString::Null, object->Parse(nullptr), "Actual value did not match expected null value");

		object = Internal::SqlObject::Create("xml");
		SqlTypes::SqlXml^ testXmlValue = gcnew SqlTypes::SqlXml(Xml::XmlReader::Create(gcnew IO::StringReader("<xml><a>1</a></xml>")));
		Assert::AreEqual(testXmlValue->Value, safe_cast<SqlTypes::SqlXml^>(object->Parse("<xml><a>1</a></xml>"))->Value, "Actual value did not match expected value");
		Assert::AreNotEqual(testXmlValue->Value, safe_cast<SqlTypes::SqlXml^>(object->Parse("<xml><a>2</a></xml>"))->Value, "Actual value matched non-expected value");
		// Assert::AreEqual(SqlTypes::SqlXml::Null, object->Parse(nullptr), "Actual value did not match expected null value");

		object = Internal::SqlObject::Create("sql_variant");
		try {
			Assert::AreEqual("l34k2n5moik234jn590fsd-", object->Parse("l34k2n5moik234jn590fsd-"), "Actual value did not match expected value");

			Assert::True(false, "Parse operation did not throw exception");
		} catch (Exception^) { }

		Assert::AreEqual(SqlTypes::SqlString("l34k2n5moik234jn590fsd-"), object->Parse("[varchar(30)]l34k2n5moik234jn590fsd-"), "Actual value did not match expected value");
		Assert::AreEqual(SqlTypes::SqlString("12345678901234567890abcdefghjk"), object->Parse("[varchar(30)]12345678901234567890abcdefghjk"), "Actual value did not match expected value");

		try {
			object->Parse("[varchar(30)]12345678901234567890abcdefghjkl");

			Assert::True(false, "Parse operation did not throw exception");
		} catch (Exception^) { }

		Assert::AreEqual(SqlTypes::SqlString("l34k2n5moik234jn590fsd-"), object->Parse("[varchar(30)]l34k2n5moik234jn590fsd-"), "Actual value did not match expected value");

		try {
			object->Parse(String::Format("[varchar]{0}", String::Empty->PadLeft(8001)));

			Assert::True(false, "Parse operation did not throw exception");
		} catch (Exception^) { }

		Assert::AreEqual(SqlTypes::SqlInt64(23423423456324324), object->Parse("[bigint]23423423456324324"), "Actual value did not match expected value");
		Assert::AreEqual(SqlTypes::SqlBinary(gcnew cli::array<Byte> { 5, 6, 7, 15, 255, 0xEF, 0xAD, 0x23, 0x43, 0x24, 0x50, 0x98 }), object->Parse("[varbinary]0x0506070FFFEFAD2343245098"), "Actual value did not match expected value");

		try {
			object->Parse(String::Format("[varbinary]0x{0}", String::Empty->PadLeft(8001, '1')));

			Assert::True(false, "Parse operation did not throw exception");
		} catch (Exception^) { }

		Assert::AreEqual(SqlTypes::SqlBoolean(true), object->Parse("[bit]1"), "Actual value did not match expected value");
		Assert::AreEqual(SqlTypes::SqlBoolean(false), object->Parse("[bit]0"), "Actual value did not match expected value");
		Assert::AreEqual(SqlTypes::SqlBoolean(true), object->Parse("[bit]true"), "Actual value did not match expected value");
		Assert::AreEqual(SqlTypes::SqlBoolean(false), object->Parse("[bit]false"), "Actual value did not match expected value");

		Assert::AreEqual(DateTime(2015, 12, 31, 1, 2, 3, 45), object->Parse("[datetime2]2015-12-31 01:02:03.0450000"), "Actual value did not match expected value");
		Assert::AreEqual(DateTime(2015, 12, 31, 1, 2, 3, 45), object->Parse("[datetime2(6)]2015-12-31 01:02:03.0450000"), "Actual value did not match expected value");
		Assert::AreEqual(DateTime(2015, 12, 31), object->Parse("[date]2015-12-31"), "Actual value did not match expected value");
		Assert::AreEqual(DateTime::Now.Date.AddHours(12).AddMinutes(13).AddSeconds(14).AddMilliseconds(123), object->Parse("[time]12:13:14.1230000"), "Actual value did not match expected value");
		Assert::AreEqual(DateTime::Now.Date.AddHours(12).AddMinutes(13).AddSeconds(14).AddMilliseconds(123), object->Parse("[time(2)]12:13:14.1230000"), "Actual value did not match expected value");
		Assert::AreEqual(SqlTypes::SqlDateTime(DateTime(2015, 12, 31, 1, 2, 3, 45)), object->Parse("[datetime]2015-12-31 01:02:03.045"), "Actual value did not match expected value");
		Assert::AreEqual(DateTimeOffset(DateTime(2015, 12, 31, 1, 2, 3, 45), TimeSpan(5, 20, 0)), object->Parse("[datetimeoffset]2015-12-31 01:02:03.045 +05:20"), "Actual value did not match expected value");
		
		Assert::AreEqual(SqlTypes::SqlDecimal::Parse("23.435"), object->Parse("[decimal]23.435"), "Actual value did not match expected value");
		Assert::AreEqual(SqlTypes::SqlDecimal::Parse("23.435"), object->Parse("[numeric]23.435"), "Actual value did not match expected value");
		Assert::AreEqual(SqlTypes::SqlDecimal::Parse("23.435"), object->Parse("[numeric(6, 3)]23.435"), "Actual value did not match expected value");
		Assert::AreEqual(SqlTypes::SqlDecimal::Parse("23.435"), object->Parse("[decimal(6, 4)]23.435"), "Actual value did not match expected value");
		
		try {
			object->Parse("[decimal(6, 4)]234.435");

			Assert::True(false, "Parse operation did not throw exception");
		} catch (Exception^) { }

		try {
			object->Parse("[decimal(39, 4)]234.435");

			Assert::True(false, "Parse operation did not throw exception");
		} catch (Exception^) { }

		Assert::AreEqual(SqlTypes::SqlDecimal::Parse("123456789123456789.12345678912345678912"), object->Parse("[decimal]123456789123456789.12345678912345678912"), "Actual value did not match expected value");
		Assert::AreEqual(SqlTypes::SqlDecimal::Parse("123456789123456789.12345678912345678912"), object->Parse("[numeric]123456789123456789.12345678912345678912"), "Actual value did not match expected value");

		Assert::AreEqual(SqlTypes::SqlDouble(12345.58425), object->Parse("[float]12345.58425"), "Actual value did not match expected value");
		Assert::AreEqual(SqlTypes::SqlDouble(12345.58425), object->Parse("[float(53)]12345.58425"), "Actual value did not match expected value");
		Assert::AreEqual(SqlTypes::SqlInt32(123452345), object->Parse("[int]123452345"), "Actual value did not match expected value");
		Assert::AreEqual(SqlTypes::SqlMoney(123452345.1234), object->Parse("[money]123452345.1234"), "Actual value did not match expected value");
		Assert::AreEqual(SqlTypes::SqlSingle(123452345.1234), object->Parse("[real]123452345.1234"), "Actual value did not match expected value");
		Assert::AreEqual(SqlTypes::SqlSingle(123452345.1234), object->Parse("[float(23)]123452345.1234"), "Actual value did not match expected value");
		Assert::AreEqual(SqlTypes::SqlInt16(12345), object->Parse("[smallint]12345"), "Actual value did not match expected value");
		Assert::AreEqual(SqlTypes::SqlByte(255), object->Parse("[tinyint]255"), "Actual value did not match expected value");
		Assert::AreEqual(SqlTypes::SqlGuid::Parse("35FC0644-16CE-4A88-84EE-959CF53A8965"), object->Parse("[uniqueidentifier]35FC0644-16CE-4A88-84EE-959CF53A8965"), "Actual value did not match expected value");

		Assert::AreEqual(nullptr, object->Parse(nullptr), "Actual value did not match expected value");
	}

	void SqlObject::TestReadingAndWriting() {
		IO::MemoryStream^ ms = gcnew IO::MemoryStream;

		Internal::SqlObject^ object = Internal::SqlObject::Create("bigint");
		object->Write(SqlTypes::SqlInt64(100030000100000000), gcnew IO::BinaryWriter(ms));
		ms->Seek(0, IO::SeekOrigin::Begin);
		Assert::AreEqual(SqlTypes::SqlInt64(100030000100000000), object->Read(gcnew IO::BinaryReader(ms)));
		ms->Seek(0, IO::SeekOrigin::Begin);

		object = Internal::SqlObject::Create("binary");
		object->Write(SqlTypes::SqlBinary(gcnew cli::array<Byte> { 12, 13, 14, 15, 16, 32, 31, 30, 124, 216, 24 }), gcnew IO::BinaryWriter(ms));
		ms->Seek(0, IO::SeekOrigin::Begin);
		Assert::AreEqual(SqlTypes::SqlBinary(gcnew cli::array<Byte> { 12, 13, 14, 15, 16, 32, 31, 30, 124, 216, 24 }), object->Read(gcnew IO::BinaryReader(ms)));
		ms->Seek(0, IO::SeekOrigin::Begin);
		
		object = Internal::SqlObject::Create("bit");
		object->Write(SqlTypes::SqlBoolean(true), gcnew IO::BinaryWriter(ms));
		ms->Seek(0, IO::SeekOrigin::Begin);
		Assert::AreEqual(SqlTypes::SqlBoolean(true), object->Read(gcnew IO::BinaryReader(ms)));
		ms->Seek(0, IO::SeekOrigin::Begin);

		object->Write(SqlTypes::SqlBoolean(false), gcnew IO::BinaryWriter(ms));
		ms->Seek(0, IO::SeekOrigin::Begin);
		Assert::AreEqual(SqlTypes::SqlBoolean(false), object->Read(gcnew IO::BinaryReader(ms)));
		ms->Seek(0, IO::SeekOrigin::Begin);

		object = Internal::SqlObject::Create("varchar");
		object->Write(SqlTypes::SqlString("this is my test string!123450-9i324dfsvjikbn"), gcnew IO::BinaryWriter(ms));
		ms->Seek(0, IO::SeekOrigin::Begin);
		Assert::AreEqual(SqlTypes::SqlString("this is my test string!123450-9i324dfsvjikbn"), object->Read(gcnew IO::BinaryReader(ms)));
		ms->Seek(0, IO::SeekOrigin::Begin);

		object = Internal::SqlObject::Create("datetime");
		object->Write(SqlTypes::SqlDateTime(DateTime::Parse("2013-12-01 13:12:11.123")), gcnew IO::BinaryWriter(ms));
		ms->Seek(0, IO::SeekOrigin::Begin);
		Assert::AreEqual(SqlTypes::SqlDateTime(DateTime::Parse("2013-12-01 13:12:11.123")), object->Read(gcnew IO::BinaryReader(ms)));
		ms->Seek(0, IO::SeekOrigin::Begin);

		object = Internal::SqlObject::Create("datetime2");
		object->Write(DateTime::Parse("2012-11-02 12:13:12.4567891"), gcnew IO::BinaryWriter(ms));
		ms->Seek(0, IO::SeekOrigin::Begin);
		Assert::AreEqual(DateTime::Parse("2012-11-02 12:13:12.4567891"), object->Read(gcnew IO::BinaryReader(ms)));
		ms->Seek(0, IO::SeekOrigin::Begin);

		object = Internal::SqlObject::Create("datetimeoffset");
		object->Write(DateTimeOffset::Parse("2012-11-02 12:13:12.4567891 +01:00"), gcnew IO::BinaryWriter(ms));
		ms->Seek(0, IO::SeekOrigin::Begin);
		Assert::AreEqual(DateTimeOffset::Parse("2012-11-02 12:13:12.4567891 +01:00"), object->Read(gcnew IO::BinaryReader(ms)));
		ms->Seek(0, IO::SeekOrigin::Begin);

		object = Internal::SqlObject::Create("decimal");
		object->Write(SqlTypes::SqlDecimal(safe_cast<Decimal>(1.53425342)), gcnew IO::BinaryWriter(ms));
		ms->Seek(0, IO::SeekOrigin::Begin);
		Assert::AreEqual(SqlTypes::SqlDecimal(safe_cast<Decimal>(1.53425342)), object->Read(gcnew IO::BinaryReader(ms)));
		ms->Seek(0, IO::SeekOrigin::Begin);

		object = Internal::SqlObject::Create("float");
		object->Write(SqlTypes::SqlDouble(safe_cast<double>(1.5342534223)), gcnew IO::BinaryWriter(ms));
		ms->Seek(0, IO::SeekOrigin::Begin);
		Assert::AreEqual(SqlTypes::SqlDouble(safe_cast<double>(1.5342534223)), object->Read(gcnew IO::BinaryReader(ms)));
		ms->Seek(0, IO::SeekOrigin::Begin);

		object = Internal::SqlObject::Create("int");
		object->Write(SqlTypes::SqlInt32(1000300001), gcnew IO::BinaryWriter(ms));
		ms->Seek(0, IO::SeekOrigin::Begin);
		Assert::AreEqual(SqlTypes::SqlInt32(1000300001), object->Read(gcnew IO::BinaryReader(ms)));
		ms->Seek(0, IO::SeekOrigin::Begin);

		object = Internal::SqlObject::Create("money");
		object->Write(SqlTypes::SqlMoney(safe_cast<Decimal>(1234.5342)), gcnew IO::BinaryWriter(ms));
		ms->Seek(0, IO::SeekOrigin::Begin);
		Assert::AreEqual(SqlTypes::SqlMoney(safe_cast<Decimal>(1234.5342)), object->Read(gcnew IO::BinaryReader(ms)));
		ms->Seek(0, IO::SeekOrigin::Begin);

		object = Internal::SqlObject::Create("real");
		object->Write(SqlTypes::SqlSingle(safe_cast<float>(1234.5342)), gcnew IO::BinaryWriter(ms));
		ms->Seek(0, IO::SeekOrigin::Begin);
		Assert::AreEqual(SqlTypes::SqlSingle(safe_cast<float>(1234.5342)), object->Read(gcnew IO::BinaryReader(ms)));
		ms->Seek(0, IO::SeekOrigin::Begin);

		object = Internal::SqlObject::Create("smallint");
		object->Write(SqlTypes::SqlInt16(safe_cast<short>(1234)), gcnew IO::BinaryWriter(ms));
		ms->Seek(0, IO::SeekOrigin::Begin);
		Assert::AreEqual(SqlTypes::SqlInt16(safe_cast<short>(1234)), object->Read(gcnew IO::BinaryReader(ms)));
		ms->Seek(0, IO::SeekOrigin::Begin);

		object = Internal::SqlObject::Create("tinyint");
		object->Write(SqlTypes::SqlByte(safe_cast<Byte>(128)), gcnew IO::BinaryWriter(ms));
		ms->Seek(0, IO::SeekOrigin::Begin);
		Assert::AreEqual(SqlTypes::SqlByte(safe_cast<Byte>(128)), object->Read(gcnew IO::BinaryReader(ms)));
		ms->Seek(0, IO::SeekOrigin::Begin);

		object = Internal::SqlObject::Create("uniqueidentifier");
		Guid guid = Guid::NewGuid();
		object->Write(SqlTypes::SqlGuid(guid), gcnew IO::BinaryWriter(ms));
		ms->Seek(0, IO::SeekOrigin::Begin);
		Assert::AreEqual(SqlTypes::SqlGuid(guid), object->Read(gcnew IO::BinaryReader(ms)));
		ms->Seek(0, IO::SeekOrigin::Begin);

		object = Internal::SqlObject::Create("xml");
		object->Write(gcnew SqlTypes::SqlXml(Xml::XmlReader::Create(gcnew IO::StringReader("<xml><b>2</b></xml>"))), gcnew IO::BinaryWriter(ms));
		ms->Seek(0, IO::SeekOrigin::Begin);
		Assert::AreEqual("<xml><b>2</b></xml>", safe_cast<SqlTypes::SqlXml^>(object->Read(gcnew IO::BinaryReader(ms)))->Value);
		ms->Seek(0, IO::SeekOrigin::Begin);

		object = Internal::SqlObject::Create("sql_variant");
		object->Write(SqlTypes::SqlString("abdefeg"), gcnew IO::BinaryWriter(ms));
		ms->Seek(0, IO::SeekOrigin::Begin);
		Assert::AreEqual(SqlTypes::SqlString("abdefeg"), object->Read(gcnew IO::BinaryReader(ms)));
		ms->Seek(0, IO::SeekOrigin::Begin);

		object->Write(SqlTypes::SqlInt64(32114876234), gcnew IO::BinaryWriter(ms));
		ms->Seek(0, IO::SeekOrigin::Begin);
		Assert::AreEqual(SqlTypes::SqlInt64(32114876234), object->Read(gcnew IO::BinaryReader(ms)));
		ms->Seek(0, IO::SeekOrigin::Begin);

		object->Write(SqlTypes::SqlBinary(gcnew cli::array<Byte> { 23, 24, 25, 25, 255 }), gcnew IO::BinaryWriter(ms));
		ms->Seek(0, IO::SeekOrigin::Begin);
		Assert::AreEqual(SqlTypes::SqlBinary(gcnew cli::array<Byte> { 23, 24, 25, 25, 255 }), object->Read(gcnew IO::BinaryReader(ms)));
		ms->Seek(0, IO::SeekOrigin::Begin);

		object->Write(SqlTypes::SqlBoolean(true), gcnew IO::BinaryWriter(ms));
		ms->Seek(0, IO::SeekOrigin::Begin);
		Assert::AreEqual(SqlTypes::SqlBoolean(true), object->Read(gcnew IO::BinaryReader(ms)));
		ms->Seek(0, IO::SeekOrigin::Begin);

		object->Write(SqlTypes::SqlBoolean(false), gcnew IO::BinaryWriter(ms));
		ms->Seek(0, IO::SeekOrigin::Begin);
		Assert::AreEqual(SqlTypes::SqlBoolean(false), object->Read(gcnew IO::BinaryReader(ms)));
		ms->Seek(0, IO::SeekOrigin::Begin);

		object->Write(DateTime(2000, 10, 9, 8, 7, 6, 5), gcnew IO::BinaryWriter(ms));
		ms->Seek(0, IO::SeekOrigin::Begin);
		Assert::AreEqual(DateTime(2000, 10, 9, 8, 7, 6, 5), object->Read(gcnew IO::BinaryReader(ms)));
		ms->Seek(0, IO::SeekOrigin::Begin);

		object->Write(SqlTypes::SqlDateTime(DateTime(2000, 10, 9, 8, 7, 6, 5)), gcnew IO::BinaryWriter(ms));
		ms->Seek(0, IO::SeekOrigin::Begin);
		Assert::AreEqual(SqlTypes::SqlDateTime(DateTime(2000, 10, 9, 8, 7, 6, 5)), object->Read(gcnew IO::BinaryReader(ms)));
		ms->Seek(0, IO::SeekOrigin::Begin);

		object->Write(DateTimeOffset(DateTime(2000, 10, 9, 8, 7, 6, 5), TimeSpan(1, 5, 0)), gcnew IO::BinaryWriter(ms));
		ms->Seek(0, IO::SeekOrigin::Begin);
		Assert::AreEqual(DateTimeOffset(DateTime(2000, 10, 9, 8, 7, 6, 5), TimeSpan(1, 5, 0)), object->Read(gcnew IO::BinaryReader(ms)));
		ms->Seek(0, IO::SeekOrigin::Begin);

		object->Write(SqlTypes::SqlDecimal(12.34), gcnew IO::BinaryWriter(ms));
		ms->Seek(0, IO::SeekOrigin::Begin);
		Assert::AreEqual(SqlTypes::SqlDecimal(12.34), object->Read(gcnew IO::BinaryReader(ms)));
		ms->Seek(0, IO::SeekOrigin::Begin);

		object->Write(SqlTypes::SqlDouble(213.123f), gcnew IO::BinaryWriter(ms));
		ms->Seek(0, IO::SeekOrigin::Begin);
		Assert::AreEqual(SqlTypes::SqlDouble(213.123f), object->Read(gcnew IO::BinaryReader(ms)));
		ms->Seek(0, IO::SeekOrigin::Begin);

		object->Write(SqlTypes::SqlInt32(234234), gcnew IO::BinaryWriter(ms));
		ms->Seek(0, IO::SeekOrigin::Begin);
		Assert::AreEqual(SqlTypes::SqlInt32(234234), object->Read(gcnew IO::BinaryReader(ms)));
		ms->Seek(0, IO::SeekOrigin::Begin);

		object->Write(SqlTypes::SqlMoney(234234.2345), gcnew IO::BinaryWriter(ms));
		ms->Seek(0, IO::SeekOrigin::Begin);
		Assert::AreEqual(SqlTypes::SqlMoney(234234.2345), object->Read(gcnew IO::BinaryReader(ms)));
		ms->Seek(0, IO::SeekOrigin::Begin);

		object->Write(SqlTypes::SqlSingle(324.3245), gcnew IO::BinaryWriter(ms));
		ms->Seek(0, IO::SeekOrigin::Begin);
		Assert::AreEqual(SqlTypes::SqlSingle(324.3245), object->Read(gcnew IO::BinaryReader(ms)));
		ms->Seek(0, IO::SeekOrigin::Begin);

		object->Write(SqlTypes::SqlInt16(23556), gcnew IO::BinaryWriter(ms));
		ms->Seek(0, IO::SeekOrigin::Begin);
		Assert::AreEqual(SqlTypes::SqlInt16(23556), object->Read(gcnew IO::BinaryReader(ms)));
		ms->Seek(0, IO::SeekOrigin::Begin);

		object->Write(SqlTypes::SqlByte(255), gcnew IO::BinaryWriter(ms));
		ms->Seek(0, IO::SeekOrigin::Begin);
		Assert::AreEqual(SqlTypes::SqlByte(255), object->Read(gcnew IO::BinaryReader(ms)));
		ms->Seek(0, IO::SeekOrigin::Begin);

		object->Write(SqlTypes::SqlGuid(guid), gcnew IO::BinaryWriter(ms));
		ms->Seek(0, IO::SeekOrigin::Begin);
		Assert::AreEqual(SqlTypes::SqlGuid(guid), object->Read(gcnew IO::BinaryReader(ms)));
		ms->Seek(0, IO::SeekOrigin::Begin);
	}

	void SqlObject::TestStringConversion() {
		Internal::SqlObject^ object = Internal::SqlObject::Create();
		Assert::AreEqual("ewqunqweiujnb", object->ConvertToString(SqlTypes::SqlString("ewqunqweiujnb")), "String conversion did not produce expected result");

		object = Internal::SqlObject::Create("bigint");
		Assert::AreEqual("34532452352352345", object->ConvertToString(SqlTypes::SqlInt64(34532452352352345)), "String conversion did not produce expected result");

		object = Internal::SqlObject::Create("binary");
		Assert::AreEqual("0x010203040506070F", object->ConvertToString(SqlTypes::SqlBinary(gcnew cli::array<Byte> { 1, 2, 3, 4, 5, 6, 7, 15 })), "String conversion did not produce expected result");

		object = Internal::SqlObject::Create("bit");
		Assert::AreEqual("True", object->ConvertToString(SqlTypes::SqlBoolean(true)), "String conversion did not produce expected result");
		Assert::AreEqual("False", object->ConvertToString(SqlTypes::SqlBoolean(false)), "String conversion did not produce expected result");

		object = Internal::SqlObject::Create("char");
		Assert::AreEqual("c", object->ConvertToString(SqlTypes::SqlString("c")), "String conversion did not produce expected result");

		object = Internal::SqlObject::Create("date");
		Assert::AreEqual("2013-12-01", object->ConvertToString(DateTime(2013, 12, 01)), "String conversion did not produce expected result");

		object = Internal::SqlObject::Create("datetime");
		Assert::AreEqual("2013-12-01 11:12:13.323", object->ConvertToString(SqlTypes::SqlDateTime(DateTime(2013, 12, 1, 11, 12, 13, 323))), "String conversion did not produce expected result");

		object = Internal::SqlObject::Create("datetime2");
		Assert::AreEqual("2015-11-02 10:09:26.3231234", object->ConvertToString(DateTime(635820557663231234)), "String conversion did not produce expected result");

		object = Internal::SqlObject::Create("time");
		Assert::AreEqual("10:09:26.3231234", object->ConvertToString(DateTime(635820557663231234)), "String conversion did not produce expected result");

		object = Internal::SqlObject::Create("datetimeoffset");
		Assert::AreEqual("2015-11-02 10:09:26.3231234 +02:20", object->ConvertToString(DateTimeOffset(DateTime(635820557663231234), TimeSpan(2, 20, 0))), "String conversion did not produce expected result");

		object = Internal::SqlObject::Create("decimal");
		Assert::AreEqual("4.45678", object->ConvertToString(SqlTypes::SqlDecimal(Decimal::Parse("4.45678"))), "String conversion did not produce expected result");

		object = Internal::SqlObject::Create("float");
		Assert::AreEqual("4.5678", object->ConvertToString(SqlTypes::SqlDouble(4.5678)), "String conversion did not produce expected result");

		object = Internal::SqlObject::Create("int");
		Assert::AreEqual("213123", object->ConvertToString(SqlTypes::SqlInt32(213123)), "String conversion did not produce expected result");

		object = Internal::SqlObject::Create("money");
		Assert::AreEqual("12.1223", object->ConvertToString(SqlTypes::SqlMoney(12.1223)), "String conversion did not produce expected result");

		object = Internal::SqlObject::Create("nchar");
		Assert::AreEqual("asdikjbn324iub", object->ConvertToString(SqlTypes::SqlString("asdikjbn324iub")), "String conversion did not produce expected result");

		object = Internal::SqlObject::Create("numeric");
		Assert::AreEqual("456.234678", object->ConvertToString(SqlTypes::SqlDecimal(Decimal::Parse("456.234678"))), "String conversion did not produce expected result");

		object = Internal::SqlObject::Create("nvarchar");
		Assert::AreEqual("2345rwefsdfg436dwf", object->ConvertToString(SqlTypes::SqlString("2345rwefsdfg436dwf")), "String conversion did not produce expected result");

		object = Internal::SqlObject::Create("real");
		Assert::AreEqual("54.65847", object->ConvertToString(SqlTypes::SqlSingle(54.65847)), "String conversion did not produce expected result");

		object = Internal::SqlObject::Create("smalldatetime");
		Assert::AreEqual("2010-11-04 21:10", object->ConvertToString(SqlTypes::SqlDateTime(DateTime(2010, 11, 4, 21, 10, 12, 501))), "String conversion did not produce expected result");

		object = Internal::SqlObject::Create("smalldatetime");
		Assert::AreEqual("2010-11-04 21:10", object->ConvertToString(SqlTypes::SqlDateTime(DateTime(2010, 11, 4, 21, 10, 12, 501))), "String conversion did not produce expected result");

		object = Internal::SqlObject::Create("smallint");
		Assert::AreEqual("2345", object->ConvertToString(SqlTypes::SqlInt16(2345)), "String conversion did not produce expected result");

		object = Internal::SqlObject::Create("smallmoney");
		Assert::AreEqual("1234.2345", object->ConvertToString(SqlTypes::SqlMoney(1234.2345)), "String conversion did not produce expected result");

		object = Internal::SqlObject::Create("tinyint");
		Assert::AreEqual("243", object->ConvertToString(SqlTypes::SqlByte(243)), "String conversion did not produce expected result");

		object = Internal::SqlObject::Create("uniqueidentifier");
		Assert::AreEqual("ce4e83d0-fdcf-4d49-82ad-b3c52eadd076", object->ConvertToString(SqlTypes::SqlGuid(gcnew cli::array<Byte> { 208, 131, 78, 206, 207, 253, 73, 77, 130, 173, 179, 197, 46, 173, 208, 118 })), "String conversion did not produce expected result");

		object = Internal::SqlObject::Create("varbinary");
		Assert::AreEqual("0xFFFE1012", object->ConvertToString(SqlTypes::SqlBinary(gcnew cli::array<Byte> { 255, 254, 16, 18 })), "String conversion did not produce expected result");

		object = Internal::SqlObject::Create("varchar");
		Assert::AreEqual("we	fhb	wef23485r-=+", object->ConvertToString(SqlTypes::SqlString("we	fhb	wef23485r-=+")), "String conversion did not produce expected result");

		object = Internal::SqlObject::Create("varchar(max)");
		Assert::AreEqual(";kjadsnf;kjhanbsdf;lkin34890thn[3489fhjnipo	[wendf;iojnwercjerwnc ", object->ConvertToString(SqlTypes::SqlString(";kjadsnf;kjhanbsdf;lkin34890thn[3489fhjnipo	[wendf;iojnwercjerwnc ")), "String conversion did not produce expected result");

		object = Internal::SqlObject::Create("nvarchar(max)");
		Assert::AreEqual("iopuj;3421;n;598-hdfagijun345t98", object->ConvertToString(SqlTypes::SqlString("iopuj;3421;n;598-hdfagijun345t98")), "String conversion did not produce expected result");

		object = Internal::SqlObject::Create("varbinary(max)");
		Assert::AreEqual("0x02030405", object->ConvertToString(SqlTypes::SqlBinary(gcnew cli::array<Byte> { 2, 3, 4, 5 })), "String conversion did not produce expected result");

		object = Internal::SqlObject::Create("xml");
		Assert::AreEqual("<xml><a>1</a></xml>", object->ConvertToString(gcnew SqlTypes::SqlXml(Xml::XmlReader::Create(gcnew IO::StringReader("<xml><a>1</a></xml>")))), "String conversion did not produce expected result");

		object = Internal::SqlObject::Create("hierarchyid");
		Assert::AreEqual("0x06070809", object->ConvertToString(SqlTypes::SqlBinary(gcnew cli::array<Byte> { 6, 7, 8, 9 })), "String conversion did not produce expected result");

		object = Internal::SqlObject::Create("geography");
		Assert::AreEqual("0x06070809", object->ConvertToString(SqlTypes::SqlBinary(gcnew cli::array<Byte> { 6, 7, 8, 9 })), "String conversion did not produce expected result");

		object = Internal::SqlObject::Create("geometry");
		Assert::AreEqual("0x06070809", object->ConvertToString(SqlTypes::SqlBinary(gcnew cli::array<Byte> { 6, 7, 8, 9 })), "String conversion did not produce expected result");

		object = Internal::SqlObject::Create("udt");
		Assert::AreEqual("0x06070809", object->ConvertToString(SqlTypes::SqlBinary(gcnew cli::array<Byte> { 6, 7, 8, 9 })), "String conversion did not produce expected result");

		object = Internal::SqlObject::Create("sql_variant");
		Assert::AreEqual("[string]ewqunqweiujnb", object->ConvertToString(SqlTypes::SqlString("ewqunqweiujnb")), "String conversion did not produce expected result");
		Assert::AreEqual("[bigint]34532452352352345", object->ConvertToString(SqlTypes::SqlInt64(34532452352352345)), "String conversion did not produce expected result");
		Assert::AreEqual("[binary]0x06070809", object->ConvertToString(SqlTypes::SqlBinary(gcnew cli::array<Byte> { 6, 7, 8, 9 })), "String conversion did not produce expected result");
		Assert::AreEqual("[bit]False", object->ConvertToString(SqlTypes::SqlBoolean(false)), "String conversion did not produce expected result");
		Assert::AreEqual("[datetime2]2013-12-01 00:00:00.0000000", object->ConvertToString(DateTime(2013, 12, 01)), "String conversion did not produce expected result");
		Assert::AreEqual("[datetime]2013-12-01 11:12:13.323", object->ConvertToString(SqlTypes::SqlDateTime(DateTime(2013, 12, 1, 11, 12, 13, 323))), "String conversion did not produce expected result");
		Assert::AreEqual("[datetimeoffset]2015-11-02 10:09:26.3231234 +02:20", object->ConvertToString(DateTimeOffset(DateTime(635820557663231234), TimeSpan(2, 20, 0))), "String conversion did not produce expected result");
		Assert::AreEqual("[decimal]4.45678", object->ConvertToString(SqlTypes::SqlDecimal(Decimal::Parse("4.45678"))), "String conversion did not produce expected result");
		Assert::AreEqual("[float]4.5678", object->ConvertToString(SqlTypes::SqlDouble(4.5678)), "String conversion did not produce expected result");
		Assert::AreEqual("[int]213123", object->ConvertToString(SqlTypes::SqlInt32(213123)), "String conversion did not produce expected result");
		Assert::AreEqual("[money]12.1223", object->ConvertToString(SqlTypes::SqlMoney(12.1223)), "String conversion did not produce expected result");
		Assert::AreEqual("[real]54.65847", object->ConvertToString(SqlTypes::SqlSingle(54.65847)), "String conversion did not produce expected result");
		Assert::AreEqual("[smallint]2345", object->ConvertToString(SqlTypes::SqlInt16(2345)), "String conversion did not produce expected result");
		Assert::AreEqual("[tinyint]243", object->ConvertToString(SqlTypes::SqlByte(243)), "String conversion did not produce expected result");
		Assert::AreEqual("[uniqueidentifier]ce4e83d0-fdcf-4d49-82ad-b3c52eadd076", object->ConvertToString(SqlTypes::SqlGuid(gcnew cli::array<Byte> { 208, 131, 78, 206, 207, 253, 73, 77, 130, 173, 179, 197, 46, 173, 208, 118 })), "String conversion did not produce expected result");
	}
}}}}
