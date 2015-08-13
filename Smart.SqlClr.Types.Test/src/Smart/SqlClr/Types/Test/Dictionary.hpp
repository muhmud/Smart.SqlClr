
#ifndef _SMART_SQLCLR_TYPES_TEST__DICTIONARY_HPP_
#define _SMART_SQLCLR_TYPES_TEST__DICTIONARY_HPP_

#using <Smart.SqlClr.Types.dll> as_friend

namespace Smart { namespace SqlClr { namespace Types { namespace Test { 
	using namespace System;
	using namespace NUnit::Framework;

	[TestFixture]
	public ref class Dictionary {
	public:
		[Test]
		void TestConstruction();

		[Test]
		void TestItemManagement();

		[Test]
		void TestProperties();

		[Test]
		void TestEquality();

		[Test]
		void TestStringConversion();

		[Test]
		void TestReadingAndWriting();
	};
}}}}

#endif // _SMART_SQLCLR_TYPES_TEST__DICTIONARY_HPP_
