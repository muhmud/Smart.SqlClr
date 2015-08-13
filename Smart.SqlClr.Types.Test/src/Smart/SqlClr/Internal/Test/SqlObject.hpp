
#ifndef _SMART_SQLCLR_INTERNAL_TEST__SQLOBJECT_HPP_
#define _SMART_SQLCLR_INTERNAL_TEST__SQLOBJECT_HPP_

#using <Smart.SqlClr.Types.dll> as_friend

namespace Smart { namespace SqlClr { namespace Internal { namespace Test { 
	using namespace System;
	using namespace NUnit::Framework;

	[TestFixture]
	public ref class SqlObject {
	private:
		void assertSqlTypes(String^ expectedSqlType, Type^ expectedSqlDataType, bool expectedSqlVariantCompatible, int expectedPrecision, int expectedScale, int expectedLength, Internal::SqlObject^ object);

	public:
		[Test]
		void TestConstruction();

		[Test]
		void TestValidity();

		[Test]
		void TestGettingValue();

		[Test]
		void TestParsing();

		[Test]
		void TestReadingAndWriting();

		[Test]
		void TestStringConversion();
	};
}}}}

#endif // _SMART_SQLCLR_INTERNAL_TEST__SQLOBJECT_HPP_
