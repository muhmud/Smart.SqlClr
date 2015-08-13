
#ifndef _SMART_SQLCLR_UTILS_TEST__BINARY_HPP_
#define _SMART_SQLCLR_UTILS_TEST__BINARY_HPP_

namespace Smart { namespace SqlClr { namespace Utils { namespace Test { 
	using namespace System;
	using namespace NUnit::Framework;

	[TestFixture]
	public ref class Binary {
	public:
		[Test]
		void TestToHex();

		[Test]
		void TestToBinary();
	};
}}}}

#endif // _SMART_SQLCLR_UTILS_TEST__BINARY_HPP_
