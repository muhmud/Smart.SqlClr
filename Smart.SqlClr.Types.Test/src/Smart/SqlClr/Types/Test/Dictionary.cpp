
#include <Smart/SqlClr/Types/Test/Dictionary.hpp>

#using <System.dll>
#using <System.Data.dll>
#using <System.Xml.dll>

namespace Smart { namespace SqlClr { namespace Types { namespace Test { 
	using namespace System;
	using namespace System::Data;
	using namespace NUnit::Framework;

	void Dictionary::TestConstruction() {
		Types::Dictionary^ dictionary = Types::Dictionary::New();
		Types::Dictionary^ testDictionary = nullptr;

		Assert::AreEqual("string", dictionary->KeyType.Value, "Dictionary did not have expected key type");
		Assert::AreEqual("sql_variant", dictionary->ValueType.Value, "Dictionary did not have expected value type");

		dictionary = Types::Dictionary::NewOf("string", "sql_variant");

		Assert::AreEqual("string", dictionary->KeyType.Value, "Dictionary did not have expected key type");
		Assert::AreEqual("sql_variant", dictionary->ValueType.Value, "Dictionary did not have expected value type");

		dictionary = Types::Dictionary::NewOf("int", "string");

		Assert::AreEqual("int", dictionary->KeyType.Value, "Dictionary did not have expected key type");
		Assert::AreEqual("string", dictionary->ValueType.Value, "Dictionary did not have expected value type");

		try {
			dictionary->AddNestedEntry(SqlTypes::SqlInt32(234), Types::Dictionary::New());

			Assert::True(false, "Adding nested dictionary to non-nested type should throw");
		} catch (Exception^) { }

		try {
			dictionary = Types::Dictionary::NewOf("int2", "string");

			Assert::True(false, "Creation of dictionary with invalid type did not throw");
		} catch(Exception^) { }

		try {
			dictionary = Types::Dictionary::NewOf("int", "string2");

			Assert::True(false, "Creation of dictionary with invalid type did not throw");
		} catch(Exception^) { }

		try {
			dictionary = Types::Dictionary::NewOf(nullptr, "decimal(38, 12)");

			Assert::True(false, "Creation of dictionary with invalid type did not throw");
		} catch(Exception^) { }

		try {
			dictionary = Types::Dictionary::NewOf("string", nullptr);

			Assert::True(false, "Creation of dictionary with invalid type did not throw");
		} catch(Exception^) { }

		try {
			dictionary = Types::Dictionary::NewOf(nullptr, nullptr);

			Assert::True(false, "Creation of dictionary with invalid types did not throw");
		} catch(Exception^) { }

		dictionary = Types::Dictionary::NewNested();
		testDictionary = Types::Dictionary::New();testDictionary->AddEntry(SqlTypes::SqlString("fghiu"), SqlTypes::SqlInt64(43434343));
		dictionary->AddNestedEntry(SqlTypes::SqlString("abcd"), testDictionary);

		testDictionary = Types::Dictionary::NewOf("int", "decimal"); testDictionary->AddEntry(SqlTypes::SqlInt32(12), SqlTypes::SqlDecimal::Parse("1.2"));testDictionary->AddEntry(SqlTypes::SqlInt32(14), SqlTypes::SqlDecimal::Parse("12"));
		dictionary->AddNestedEntry(SqlTypes::SqlString("abcddefkjn"), testDictionary);

		Assert::AreEqual(2, dictionary->Count.Value, "Dictionary did not contain correct number of items");
		Assert::AreEqual(1, dictionary->GetDictionary(SqlTypes::SqlString("abcd"))->Count.Value, "Dictionary did not contain correct number of items");
		Assert::AreEqual(SqlTypes::SqlInt64(43434343), dictionary->GetDictionary(SqlTypes::SqlString("abcd"))->Get(SqlTypes::SqlString("fghiu")), "Nested dictionary did not contain expected value");
		Assert::AreEqual(2, dictionary->GetDictionary(SqlTypes::SqlString("abcddefkjn"))->Count.Value, "Dictionary did not contain correct number of items");
		Assert::AreEqual(SqlTypes::SqlDecimal::Parse("1.2"), dictionary->GetDictionary(SqlTypes::SqlString("abcddefkjn"))->Get(SqlTypes::SqlInt32(12)), "Dictionary did not contain correct number of items");
		Assert::AreEqual(SqlTypes::SqlDecimal::Parse("12"), dictionary->GetDictionary(SqlTypes::SqlString("abcddefkjn"))->Get(SqlTypes::SqlInt32(14)), "Dictionary did not contain correct number of items");

		try {
			dictionary->AddEntry(SqlTypes::SqlString("abjdsifcd"), SqlTypes::SqlString("q3w432fd"));

			Assert::True(false, "Adding non-nested object to nested dictionary should throw");
		} catch (Exception^) { }

		DateTime now = DateTime::Now;
		DateTime nowNext = now.AddTicks(1);
		DateTime nowNext2 = nowNext.AddTicks(1);
		Types::Dictionary^ testDictionary3 = Types::Dictionary::NewOf("smalldatetime", "datetime2");testDictionary3->AddEntry(SqlTypes::SqlDateTime(nowNext), nowNext2);
		Types::Dictionary^ testDictionary2 = Types::Dictionary::NewNestedOf("int");testDictionary2->AddNestedEntry(SqlTypes::SqlInt32(13), testDictionary3);
		testDictionary = Types::Dictionary::NewNested();testDictionary->AddNestedEntry(SqlTypes::SqlString("abc"), testDictionary2);
		dictionary = Types::Dictionary::NewNestedOf("datetime");
		dictionary->AddNestedEntry(SqlTypes::SqlDateTime(now), testDictionary);

		Assert::AreEqual(1, dictionary->Count.Value, "Dictionary does not contain correct number of items");
		Assert::True(dictionary->ContainsKey(SqlTypes::SqlDateTime(now)).Value, "Dictionary did not contain expected key");
		Assert::AreEqual(1, dictionary->GetDictionary(SqlTypes::SqlDateTime(now))->Count.Value, "Dictionary does not contain correct number of items");
		Assert::True(dictionary->GetDictionary(SqlTypes::SqlDateTime(now))->ContainsKey(SqlTypes::SqlString("abc")).Value, "Dictionary did not contain expected key");
		Assert::AreEqual(1, dictionary->GetDictionary(SqlTypes::SqlDateTime(now))->GetDictionary(SqlTypes::SqlString("abc"))->Count.Value, "Dictionary does not contain correct number of items");
		Assert::True(dictionary->GetDictionary(SqlTypes::SqlDateTime(now))->GetDictionary(SqlTypes::SqlString("abc"))->ContainsKey(SqlTypes::SqlInt32(13)).Value, "Dictionary did not contain expected key");
		Assert::AreEqual(1, dictionary->GetDictionary(SqlTypes::SqlDateTime(now))->GetDictionary(SqlTypes::SqlString("abc"))->GetDictionary(SqlTypes::SqlInt32(13))->Count.Value, "Dictionary does not contain correct number of items");
		Assert::True(dictionary->GetDictionary(SqlTypes::SqlDateTime(now))->GetDictionary(SqlTypes::SqlString("abc"))->GetDictionary(SqlTypes::SqlInt32(13))->ContainsKey(SqlTypes::SqlDateTime(nowNext)).Value, "Dictionary did not contain expected key");
		Assert::AreEqual(nowNext2, dictionary->GetDictionary(SqlTypes::SqlDateTime(now))->GetDictionary(SqlTypes::SqlString("abc"))->GetDictionary(SqlTypes::SqlInt32(13))->Get(SqlTypes::SqlDateTime(nowNext)), "Dictionary did not contain expected value");
	}

	void Dictionary::TestItemManagement() {
		Types::Dictionary^ dictionary = Types::Dictionary::New();
		Assert::False(dictionary->IsNull, "Dictionary should not be Null");
		Assert::False(dictionary->IsCompressed.Value, "Dictionary has incorrect compressed status");

		dictionary->AddEntry(SqlTypes::SqlString("hello"), SqlTypes::SqlString("world"));
		Assert::AreEqual(SqlTypes::SqlString("world"), dictionary->Get(SqlTypes::SqlString("hello")), "Dictionary item operation failed");
		Assert::False(dictionary->IsNull, "Dictionary should not be Null");
		Assert::False(dictionary->IsCompressed.Value, "Dictionary has incorrect compressed status");

		dictionary->AddEntry(SqlTypes::SqlString("world"), SqlTypes::SqlInt32(2));
		Assert::AreEqual(SqlTypes::SqlInt32(2), dictionary->Get(SqlTypes::SqlString("world")), "Dictionary item operation failed");

		try {
			dictionary->AddEntry(SqlTypes::SqlInt32(2), SqlTypes::SqlString("world"));

			Assert::True(false, "Invalid key type should have thrown");
		} catch (Exception^) { }

		dictionary->AddEntry(SqlTypes::SqlString::Null, SqlTypes::SqlDouble(1.23));

		try {
			dictionary->AddEntry(SqlTypes::SqlString::Null, SqlTypes::SqlString("world2"));

			Assert::True(false, "Duplicate key should have thrown");
		} catch (Exception^) { }

		Assert::True(dictionary->ContainsKey(SqlTypes::SqlString::Null).Value, "Dictionary did not contain expected value");
		Assert::AreEqual(3, dictionary->Count.Value, "Dictionary does not contain correct number of items");

		dictionary->Clear();
		Assert::AreEqual(0, dictionary->Count.Value, "Dictionary does not contain correct number of items");
		Assert::False(dictionary->IsNull, "Dictionary should not be Null");
		Assert::False(dictionary->IsCompressed.Value, "Dictionary has incorrect compressed status");

		try {
			dictionary->AddEntry(SqlTypes::SqlInt32::Null, SqlTypes::SqlString("world"));

			Assert::True(false, "Invalid key type should have thrown");
		} catch (Exception^) { }

		dictionary->AddEntry(SqlTypes::SqlString::Null, SqlTypes::SqlDecimal::Null);
		Assert::AreEqual(SqlTypes::SqlDecimal::Null, dictionary->Get(SqlTypes::SqlString::Null), "Dictionary did not contain expected value"); 

		Assert::AreEqual(1, dictionary->Count.Value, "Dictionary did not contain expected number of items");
		dictionary->AddEntryIfNotExists(SqlTypes::SqlString::Null, SqlTypes::SqlDouble(2.0));
		Assert::AreEqual(1, dictionary->Count.Value, "Dictionary did not contain expected number of items");
		Assert::AreEqual(SqlTypes::SqlDecimal::Null, dictionary->Get(SqlTypes::SqlString::Null), "Dictionary did not contain expected value"); 

		dictionary->AddEntryIfNotExists(SqlTypes::SqlString("abc"), SqlTypes::SqlDouble(3.0));
		Assert::AreEqual(2, dictionary->Count.Value, "Dictionary did not contain expected number of items");
		Assert::AreEqual(SqlTypes::SqlDouble(3.0), dictionary->Get(SqlTypes::SqlString("abc")), "Dictionary did not contain expected value"); 

		try {
			dictionary->AddEntryIfNotExists(SqlTypes::SqlInt32(999), SqlTypes::SqlString("world234"));

			Assert::True(false, "Invalid key type should have thrown");
		} catch (Exception^) { }

		dictionary->UpdateEntry(SqlTypes::SqlString("abc"), SqlTypes::SqlDouble(4.0));
		Assert::AreEqual(2, dictionary->Count.Value, "Dictionary did not contain expected number of items");
		Assert::AreEqual(SqlTypes::SqlDouble(4.0), dictionary->Get(SqlTypes::SqlString("abc")), "Dictionary did not contain expected value"); 

		try {
			dictionary->UpdateEntry(SqlTypes::SqlDecimal(3.4), SqlTypes::SqlString("world234"));

			Assert::True(false, "Invalid key type should have thrown");
		} catch (Exception^) { }

		try {
			dictionary->UpdateEntry(SqlTypes::SqlString("abcd"), SqlTypes::SqlString("world234"));

			Assert::True(false, "Invalid key should have thrown");
		} catch (Exception^) { }

		dictionary->UpdateEntryIfExists(SqlTypes::SqlString("abc"), SqlTypes::SqlDouble(5.0));
		Assert::AreEqual(2, dictionary->Count.Value, "Dictionary did not contain expected number of items");
		Assert::AreEqual(SqlTypes::SqlDouble(5.0), dictionary->Get(SqlTypes::SqlString("abc")), "Dictionary did not contain expected value"); 

		dictionary->UpdateEntryIfExists(SqlTypes::SqlString("abcd"), SqlTypes::SqlDouble(6.0));
		Assert::AreEqual(2, dictionary->Count.Value, "Dictionary did not contain expected number of items");

		try {
			dictionary->UpdateEntryIfExists(SqlTypes::SqlSingle(3.4), SqlTypes::SqlString("world234"));

			Assert::True(false, "Invalid key type should have thrown");
		} catch (Exception^) { }

		dictionary->MergeEntry(SqlTypes::SqlString("abcd"), SqlTypes::SqlDouble(6.0));
		Assert::AreEqual(3, dictionary->Count.Value, "Dictionary did not contain expected number of items");
		Assert::AreEqual(SqlTypes::SqlDouble(6.0), dictionary->Get(SqlTypes::SqlString("abcd")), "Dictionary did not contain expected value"); 

		dictionary->MergeEntry(SqlTypes::SqlString("abcd"), SqlTypes::SqlDouble(7.0));
		Assert::AreEqual(3, dictionary->Count.Value, "Dictionary did not contain expected number of items");
		Assert::AreEqual(SqlTypes::SqlDouble(7.0), dictionary->Get(SqlTypes::SqlString("abcd")), "Dictionary did not contain expected value"); 

		try {
			dictionary->MergeEntry(SqlTypes::SqlSingle(4.5), SqlTypes::SqlString("world234"));

			Assert::True(false, "Invalid key type should have thrown");
		} catch (Exception^) { }

		Assert::True(dictionary->ContainsKey(SqlTypes::SqlString("abcd")).Value, "Dictionary does not contain expected key");
		dictionary->RemoveEntry(SqlTypes::SqlString("abcd"));
		Assert::AreEqual(2, dictionary->Count.Value, "Dictionary did not contain expected number of items");
		Assert::False(dictionary->ContainsKey(SqlTypes::SqlString("abcd")).Value, "Dictionary contains unexpected key");

		try {
			dictionary->RemoveEntry(SqlTypes::SqlString("abcd"));

			Assert::True(false, "Invalid key should have thrown");
		} catch (Exception^) { }

		try {
			dictionary->RemoveEntry(SqlTypes::SqlInt64(45));

			Assert::True(false, "Invalid key type should have thrown");
		} catch (Exception^) { }

		dictionary->RemoveEntryIfExists(SqlTypes::SqlString("abcd"));
		Assert::AreEqual(2, dictionary->Count.Value, "Dictionary did not contain expected number of items");
		Assert::False(dictionary->ContainsKey(SqlTypes::SqlString("abcd")).Value, "Dictionary contains unexpected key");

		Assert::True(dictionary->ContainsKey(SqlTypes::SqlString("abc")).Value, "Dictionary does not contain expected key");
		dictionary->RemoveEntryIfExists(SqlTypes::SqlString("abc"));
		Assert::AreEqual(1, dictionary->Count.Value, "Dictionary did not contain expected number of items");
		Assert::False(dictionary->ContainsKey(SqlTypes::SqlString("abc")).Value, "Dictionary contains unexpected key");

		try {
			dictionary->RemoveEntryIfExists(SqlTypes::SqlInt16(45));

			Assert::True(false, "Invalid key type should have thrown");
		} catch (Exception^) { }

		Assert::True(dictionary->ContainsKey(SqlTypes::SqlString::Null).Value, "Dictionary did not contain expected value");

		Types::Dictionary^ dictionary2 = Types::Dictionary::New();

		dictionary2->AddEntry(SqlTypes::SqlString("ghijk"), SqlTypes::SqlInt16(45));
		dictionary2->AddEntry(SqlTypes::SqlString("ghijkl"), SqlTypes::SqlInt64(3242));
		dictionary->AddDictionary(dictionary2);

		Assert::AreEqual(3, dictionary->Count.Value, "Dictionary did not contain expected number of items");
		Assert::True(dictionary->ContainsKey(SqlTypes::SqlString::Null).Value, "Dictionary does not contain expected key");
		Assert::True(dictionary->ContainsKey(SqlTypes::SqlString("ghijk")).Value, "Dictionary does not contain expected key");
		Assert::True(dictionary->ContainsKey(SqlTypes::SqlString("ghijkl")).Value, "Dictionary does not contain expected key");

		dictionary2->AddEntry(SqlTypes::SqlString("lkjh"), SqlTypes::SqlInt64(9876));

		dictionary->AddDictionary(dictionary2);

		Assert::AreEqual(4, dictionary->Count.Value, "Dictionary did not contain expected number of items");
		Assert::True(dictionary->ContainsKey(SqlTypes::SqlString::Null).Value, "Dictionary does not contain expected key");
		Assert::True(dictionary->ContainsKey(SqlTypes::SqlString("ghijk")).Value, "Dictionary does not contain expected key");
		Assert::True(dictionary->ContainsKey(SqlTypes::SqlString("ghijkl")).Value, "Dictionary does not contain expected key");
		Assert::True(dictionary->ContainsKey(SqlTypes::SqlString("lkjh")).Value, "Dictionary does not contain expected key");

		Assert::AreEqual(3, dictionary2->Count.Value, "Dictionary did not contain expected number of items");
		dictionary2->AddDictionary(dictionary);
		Assert::AreEqual(4, dictionary2->Count.Value, "Dictionary did not contain expected number of items");

		dictionary2 = Types::Dictionary::NewOf("int", "sql_variant");
		dictionary2->AddEntry(SqlTypes::SqlInt32(2), SqlTypes::SqlInt32(2));

		try {
			dictionary->AddDictionary(dictionary2);

			Assert::True(false, "Combining two dictionaries with incompatible types should have thrown");
		} catch (Exception^) { }

		dictionary2 = Types::Dictionary::NewOf("string", "int");
		dictionary2->AddEntry(SqlTypes::SqlString("2"), SqlTypes::SqlInt32(2));

		try {
			dictionary->AddDictionary(dictionary2);

			Assert::True(false, "Combining two dictionaries with incompatible types should have thrown");
		} catch (Exception^) { }

		dictionary2 = Types::Dictionary::NewOf("string", "sql_variant");
		dictionary2->AddEntry(SqlTypes::SqlString("2"), SqlTypes::SqlInt32(2));

		dictionary->AddDictionary(dictionary2);

		Assert::AreEqual(5, dictionary->Count.Value, "Dictionary did not contain expected number of items");
		Assert::True(dictionary->ContainsKey(SqlTypes::SqlString("2")).Value, "Dictionary does not contain expected key");
		
		dictionary2 = Types::Dictionary::New();
		dictionary2->AddEntry(SqlTypes::SqlString("poiu"), SqlTypes::SqlBinary(gcnew cli::array<Byte> { 2, 3, 4 }));
		dictionary2->AddEntry(SqlTypes::SqlString::Null, SqlTypes::SqlInt32(54));
		dictionary2->AddEntry(SqlTypes::SqlString("lkjh"), SqlTypes::SqlDecimal(65.43));

		dictionary->UpdateDictionary(dictionary2);
		Assert::AreEqual(5, dictionary->Count.Value, "Dictionary did not contain expected number of items");
		Assert::AreEqual(SqlTypes::SqlInt32(54), dictionary->Get(SqlTypes::SqlString::Null), "Dictionary did not contain expected value for key");
		Assert::AreEqual(SqlTypes::SqlDecimal(65.43), dictionary->Get(SqlTypes::SqlString("lkjh")), "Dictionary did not contain expected value for key");
		Assert::False(dictionary->ContainsKey(SqlTypes::SqlString("poiu")).Value, "Dictionary contains unexpected value");
		Assert::AreEqual(SqlTypes::SqlInt16(45), dictionary->Get(SqlTypes::SqlString("ghijk")), "Dictionary did not contain expected value for key");
		Assert::AreEqual(SqlTypes::SqlInt64(3242), dictionary->Get(SqlTypes::SqlString("ghijkl")), "Dictionary did not contain expected value for key");
		Assert::AreEqual(SqlTypes::SqlInt32(2), dictionary->Get(SqlTypes::SqlString("2")), "Dictionary did not contain expected value for key");

		dictionary2 = Types::Dictionary::NewOf("decimal", "varchar(max)");

		try {
			dictionary->UpdateDictionary(dictionary2);

			Assert::True(false, "Combining two dictionaries with incompatible types should have thrown");
		} catch (Exception^) { }

		dictionary2 = Types::Dictionary::New();
		dictionary2->AddEntry(SqlTypes::SqlString::Null, SqlTypes::SqlString::Null);
		dictionary2->AddEntry(SqlTypes::SqlString("33"), SqlTypes::SqlInt16(12));
		dictionary2->AddEntry(SqlTypes::SqlString("ghijk"), SqlTypes::SqlInt16(45));

		// Try self removal
		dictionary->RemoveDictionary(dictionary2);
		Assert::AreEqual(3, dictionary->Count.Value, "Dictionary did not contain expected number of items");
		Assert::False(dictionary->ContainsKey(SqlTypes::SqlString::Null).Value, "Dictionary contained unexpected key");
		Assert::False(dictionary->ContainsKey(SqlTypes::SqlString("ghijk")).Value, "Dictionary contained unexpected key");

		dictionary2 = Types::Dictionary::NewOf("int", "varbinary(max)");

		try {
			dictionary->RemoveDictionary(dictionary2);

			Assert::True(false, "Combining two dictionaries with incompatible types should have thrown");
		} catch (Exception^) { }

		dictionary2 = Types::Dictionary::New();
		dictionary2->AddEntry(SqlTypes::SqlString("lkjh"), SqlTypes::SqlInt64(256));
		dictionary2->AddEntry(SqlTypes::SqlString::Null, SqlTypes::SqlString("abc"));
		dictionary2->AddEntry(SqlTypes::SqlString("ghijk"), SqlTypes::SqlInt32(3));

		dictionary->MergeDictionary(dictionary2);
		Assert::AreEqual(5, dictionary->Count.Value, "Dictionary did not contain expected number of items");
		Assert::AreEqual(SqlTypes::SqlInt64(256), dictionary->Get(SqlTypes::SqlString("lkjh")), "Dictionary did not contain expected item");
		Assert::AreEqual(SqlTypes::SqlString("abc"), dictionary->Get(SqlTypes::SqlString::Null), "Dictionary did not contain expected item");
		Assert::AreEqual(SqlTypes::SqlInt32(3), dictionary->Get(SqlTypes::SqlString("ghijk")), "Dictionary did not contain expected item");
		Assert::AreEqual(SqlTypes::SqlInt64(3242), dictionary->Get(SqlTypes::SqlString("ghijkl")), "Dictionary did not contain expected value for key");
		Assert::AreEqual(SqlTypes::SqlInt32(2), dictionary->Get(SqlTypes::SqlString("2")), "Dictionary did not contain expected value for key");

		dictionary2 = Types::Dictionary::NewOf("binary (1)", "varchar(2)");
		dictionary2->AddEntry(SqlTypes::SqlBinary(gcnew cli::array<Byte> { 23 }), SqlTypes::SqlString("ab"));

		try {
			dictionary->RemoveDictionary(dictionary2);

			Assert::True(false, "Combining two dictionaries with incompatible types should have thrown");
		} catch (Exception^) { }

		dictionary->Clear();
		Assert::AreEqual(0, dictionary->Count.Value, "Dictionary was found not to be empty");

		dictionary->AddEntry(SqlTypes::SqlString("abcd"), SqlTypes::SqlInt32(234));
		dictionary->AddEntry(SqlTypes::SqlString("efgh"), SqlTypes::SqlInt32(567));

		dictionary->AddDictionary(dictionary);
		Assert::AreEqual(2, dictionary->Count.Value, "Dictionary did not contain expected number of items");
		Assert::AreEqual(SqlTypes::SqlInt32(234), dictionary->Get(SqlTypes::SqlString("abcd")), "Dictionary did not contain expected item");
		Assert::AreEqual(SqlTypes::SqlInt32(567), dictionary->Get(SqlTypes::SqlString("efgh")), "Dictionary did not contain expected item");

		dictionary->UpdateDictionary(dictionary);
		Assert::AreEqual(2, dictionary->Count.Value, "Dictionary did not contain expected number of items");
		Assert::AreEqual(SqlTypes::SqlInt32(234), dictionary->Get(SqlTypes::SqlString("abcd")), "Dictionary did not contain expected item");
		Assert::AreEqual(SqlTypes::SqlInt32(567), dictionary->Get(SqlTypes::SqlString("efgh")), "Dictionary did not contain expected item");

		dictionary->MergeDictionary(dictionary);
		Assert::AreEqual(2, dictionary->Count.Value, "Dictionary did not contain expected number of items");
		Assert::AreEqual(SqlTypes::SqlInt32(234), dictionary->Get(SqlTypes::SqlString("abcd")), "Dictionary did not contain expected item");
		Assert::AreEqual(SqlTypes::SqlInt32(567), dictionary->Get(SqlTypes::SqlString("efgh")), "Dictionary did not contain expected item");

		dictionary->RemoveDictionary(dictionary);
		Assert::AreEqual(2, dictionary->Count.Value, "Dictionary did not contain expected number of items");
		Assert::AreEqual(SqlTypes::SqlInt32(234), dictionary->Get(SqlTypes::SqlString("abcd")), "Dictionary did not contain expected item");
		Assert::AreEqual(SqlTypes::SqlInt32(567), dictionary->Get(SqlTypes::SqlString("efgh")), "Dictionary did not contain expected item");

		dictionary = Types::Dictionary::NewNested();
		Types::Dictionary^ testDictionary = Types::Dictionary::New(); 
		testDictionary->AddEntry(SqlTypes::SqlString("42389h08efh"), SqlTypes::SqlInt64(2130987));
		testDictionary->AddEntry(SqlTypes::SqlString("fdi8n4f09n2493j"), SqlTypes::SqlInt16(32));
		dictionary->AddNestedEntry(SqlTypes::SqlString("aboijew"), testDictionary);

		Assert::AreEqual(1, dictionary->Count.Value, "Dictionary does not contain expected number of items");
		Assert::AreEqual(2, dictionary->GetDictionary(SqlTypes::SqlString("aboijew"))->Count.Value, "Nested Dictionary did not contain expected number of items");
		Assert::AreEqual(SqlTypes::SqlInt64(2130987), dictionary->GetDictionary(SqlTypes::SqlString("aboijew"))->Get(SqlTypes::SqlString("42389h08efh")), "Nested dictionary did not contain expected item");
		Assert::AreEqual(SqlTypes::SqlInt16(32), dictionary->GetDictionary(SqlTypes::SqlString("aboijew"))->Get(SqlTypes::SqlString("fdi8n4f09n2493j")), "Nested dictionary did not contain expected item");

		try {
			dictionary->AddEntry(SqlTypes::SqlString("dwfkjn"), SqlTypes::SqlInt32(23849));

			Assert::True(false, "Adding non-nested entry to a nested dictionary should throw");
		} catch (Exception^) { }

		try {
			testDictionary = Types::Dictionary::New();testDictionary->AddEntry(SqlTypes::SqlString("23r87b"), SqlTypes::SqlInt32(2130987));
			dictionary->AddNestedEntry(SqlTypes::SqlInt32(12389), testDictionary);

			Assert::True(false, "Adding nested entry with invalid key type should throw");
		} catch (Exception^) { }

		testDictionary = Types::Dictionary::New();testDictionary->AddEntry(SqlTypes::SqlString("23r87b"), SqlTypes::SqlInt32(2130987));
		dictionary->AddNestedEntry(SqlTypes::SqlString("239j"), testDictionary);

		Assert::AreEqual(2, dictionary->Count.Value, "Dictionary did not contain right number of items");
		Assert::AreEqual(SqlTypes::SqlInt32(2130987), dictionary->GetDictionary(SqlTypes::SqlString("239j"))->Get(SqlTypes::SqlString("23r87b")), "Dictionary did not contain expected item value");
		
		dictionary->GetDictionary(SqlTypes::SqlString("239j"))->AddEntry(SqlTypes::SqlString("28efh"), SqlTypes::SqlInt64(129837));

		Assert::AreEqual(2, dictionary->GetDictionary(SqlTypes::SqlString("239j"))->Count.Value, "Dictionary did not contain expected number of items");
		Assert::AreEqual(SqlTypes::SqlInt64(129837), dictionary->GetDictionary(SqlTypes::SqlString("239j"))->Get(SqlTypes::SqlString("28efh")), "Dictionary did not contain expected item");

		try {
			testDictionary = Types::Dictionary::New(); testDictionary->AddEntry(SqlTypes::SqlString("3984f2h"), SqlTypes::SqlInt32(43287));
			dictionary->AddNestedEntry(SqlTypes::SqlString("239j"), testDictionary);

			Assert::True(false, "Adding nested entry with key already used should throw");
		} catch (Exception^) { }

		testDictionary = Types::Dictionary::New(); testDictionary->AddEntry(SqlTypes::SqlString("3984f2h"), SqlTypes::SqlInt32(43287));
		dictionary->AddNestedEntryIfNotExists(SqlTypes::SqlString("239j"), testDictionary);

		Assert::AreEqual(2, dictionary->GetDictionary(SqlTypes::SqlString("239j"))->Count.Value, "Dictionary did not contain expected number of items");
		Assert::AreEqual(SqlTypes::SqlInt64(129837), dictionary->GetDictionary(SqlTypes::SqlString("239j"))->Get(SqlTypes::SqlString("28efh")), "Dictionary did not contain expected item");

		try {
			testDictionary = Types::Dictionary::New(); testDictionary->AddEntry(SqlTypes::SqlString("23r87b1234"), SqlTypes::SqlInt32(3456));
			dictionary->AddNestedEntryIfNotExists(SqlTypes::SqlInt32(12389), testDictionary);

			Assert::True(false, "Adding nested entry with invalid key type should throw");
		} catch (Exception^) { }

		try {
			dictionary->UpdateEntry(SqlTypes::SqlString("239j"), SqlTypes::SqlString("239j"));

			Assert::True(false, "Updating a nested dictionary with a non nested entry should throw");
		} catch (Exception^) { }

		Types::Dictionary^ value = Types::Dictionary::New();
		value->AddEntry(SqlTypes::SqlString("SEWRDFSDF"), SqlTypes::SqlInt32(234234));
		dictionary->UpdateNestedEntry(SqlTypes::SqlString("239j"), value);

		Assert::True(dictionary->ContainsKey(SqlTypes::SqlString("239j")).Value, "Dictionary did not contain expected entry");
		Assert::False(dictionary->GetDictionary(SqlTypes::SqlString("239j"))->ContainsKey(SqlTypes::SqlString("3984f2h")).Value, "Dictionary did contained unexpected entry");
		Assert::True(dictionary->GetDictionary(SqlTypes::SqlString("239j"))->ContainsKey(SqlTypes::SqlString("SEWRDFSDF")).Value, "Dictionary did not contain expected entry");
		Assert::AreEqual(SqlTypes::SqlInt32(234234), dictionary->GetDictionary(SqlTypes::SqlString("239j"))->Get(SqlTypes::SqlString("SEWRDFSDF")), "Dictionary did not contain expected value");

		Assert::AreEqual(2, dictionary->Count.Value, "Dictionary does not contain expected number of items");
		Assert::AreEqual(2, dictionary->GetDictionary(SqlTypes::SqlString("aboijew"))->Count.Value, "Nested Dictionary did not contain expected number of items");
		Assert::AreEqual(SqlTypes::SqlInt64(2130987), dictionary->GetDictionary(SqlTypes::SqlString("aboijew"))->Get(SqlTypes::SqlString("42389h08efh")), "Nested dictionary did not contain expected item");
		Assert::AreEqual(SqlTypes::SqlInt16(32), dictionary->GetDictionary(SqlTypes::SqlString("aboijew"))->Get(SqlTypes::SqlString("fdi8n4f09n2493j")), "Nested dictionary did not contain expected item");
	}

	void Dictionary::TestProperties() {
		Types::Dictionary^ dictionary = Types::Dictionary::New();
		Assert::False(dictionary->IsNull, "Dictionary did not have correct property value");
		Assert::False(dictionary->IsCompressed.Value, "Dictionary did not have correct property value");
		Assert::AreEqual(0, dictionary->Count.Value, "Dictionary did not have correct property value");
		Assert::AreEqual("string", dictionary->KeyType.Value, "Dictionary did not have correct property value");
		Assert::AreEqual("sql_variant", dictionary->ValueType.Value, "Dictionary did not have correct property value");

		dictionary = Types::Dictionary::NewOf("smallint", "varbinary(2000)");
		Assert::False(dictionary->IsNull, "Dictionary did not have correct property value");
		Assert::False(dictionary->IsCompressed.Value, "Dictionary did not have correct property value");
		Assert::AreEqual(0, dictionary->Count.Value, "Dictionary did not have correct property value");
		Assert::AreEqual("smallint", dictionary->KeyType.Value, "Dictionary did not have correct property value");
		Assert::AreEqual("varbinary(2000)", dictionary->ValueType.Value, "Dictionary did not have correct property value");

		dictionary->SetCompression(SqlTypes::SqlBoolean(true));
		Assert::True(dictionary->IsCompressed.Value, "Dictionary did not have correct property value");

		dictionary->SetCompression(SqlTypes::SqlBoolean(false));
		Assert::False(dictionary->IsCompressed.Value, "Dictionary did not have correct property value");

		dictionary = Types::Dictionary::Null;
		Assert::True(dictionary->IsNull, "Dictionary was found not be to null");
		Assert::AreEqual(Types::Dictionary::Null, dictionary, "Dictionary was not found to equal null");
	}

	void Dictionary::TestEquality() {
		Types::Dictionary^ dictionary = Types::Dictionary::New();
		Types::Dictionary^ dictionary2 = Types::Dictionary::NewOf("int", "int");

		Assert::False(dictionary->Equals(dictionary2).Value, "Dictionaries should not be equal");
		Assert::False(dictionary2->Equals(dictionary).Value, "Dictionaries should not be equal");

		dictionary2->AddEntry(SqlTypes::SqlInt32(1), SqlTypes::SqlInt32(54));
		dictionary->AddEntry(SqlTypes::SqlString("1"), SqlTypes::SqlInt32(54));

		Assert::False(dictionary->Equals(dictionary2).Value, "Dictionaries should not be equal");
		Assert::False(dictionary2->Equals(dictionary).Value, "Dictionaries should not be equal");

		dictionary->AddEntry(SqlTypes::SqlString("abjd"), SqlTypes::SqlDecimal::Parse("2.3"));
		dictionary2 = Types::Dictionary::New();
		Assert::False(dictionary->Equals(dictionary2).Value, "Dictionaries should not be equal");
		Assert::False(dictionary2->Equals(dictionary).Value, "Dictionaries should not be equal");

		dictionary2->AddEntry(SqlTypes::SqlString("1"), SqlTypes::SqlInt32(54));
		Assert::False(dictionary->Equals(dictionary2).Value, "Dictionaries should not be equal");
		Assert::False(dictionary2->Equals(dictionary).Value, "Dictionaries should not be equal");

		dictionary2->AddEntry(SqlTypes::SqlString("abjd"), SqlTypes::SqlDecimal::Parse("2.3"));
		Assert::True(dictionary->Equals(dictionary2).Value, "Dictionaries should be equal");
		Assert::True(dictionary2->Equals(dictionary).Value, "Dictionaries should be equal");

		dictionary2->AddEntry(SqlTypes::SqlString("jhgf"), SqlTypes::SqlBinary(gcnew cli::array<Byte> { 23, 34, 45, 56 }));
		Assert::False(dictionary->Equals(dictionary2).Value, "Dictionaries should not be equal");
		Assert::False(dictionary2->Equals(dictionary).Value, "Dictionaries should not be equal");

		dictionary->AddEntry(SqlTypes::SqlString("jhgf"), SqlTypes::SqlBinary(gcnew cli::array<Byte> { 23, 34, 45, 56 }));
		Assert::True(dictionary->Equals(dictionary2).Value, "Dictionaries should be equal");
		Assert::True(dictionary2->Equals(dictionary).Value, "Dictionaries should be equal");
	}

	void Dictionary::TestStringConversion() {
		Types::Dictionary^ dictionary = Types::Dictionary::New();
		dictionary->AddEntry(SqlTypes::SqlString("abcdf"), SqlTypes::SqlString("ghij"));
		dictionary->AddEntry(SqlTypes::SqlString("ghij"), SqlTypes::SqlInt32(4354325));

		String^ dictionaryString = "<Dictionary keyType=\"string\" valueType=\"sql_variant\" isCompressed=\"False\"><Entry key=\"ghij\">[int]4354325</Entry><Entry key=\"abcdf\">[string]ghij</Entry></Dictionary>";
		Assert::AreEqual(dictionaryString, dictionary->ToString(), "Dictionary strings should be equal");

		Types::Dictionary^ dictionary2 = Types::Dictionary::Parse(dictionaryString);
		Assert::True(dictionary->Equals(dictionary2).Value, "Dictionary should be equal");
		Assert::True(dictionary2->Equals(dictionary).Value, "Dictionary should be equal");
		
		dictionary = Types::Dictionary::NewOf("string", "Int");
		dictionary2 = Types::Dictionary::NewOf("String", "int");

		Assert::AreEqual(dictionary->ToString(), dictionary2->ToString(), "Dictionary strings should be equal");

		dictionary->AddEntry(SqlTypes::SqlString("iuyt"), SqlTypes::SqlInt32(5));
		dictionary->AddEntry(SqlTypes::SqlString("poiu"), SqlTypes::SqlInt32(6));

		Assert::AreNotEqual(dictionary->ToString(), dictionary2->ToString(), "Dictionary strings should not be equal");

		dictionary2->AddEntry(SqlTypes::SqlString("iuyt"), SqlTypes::SqlInt32(5));
		Assert::AreNotEqual(dictionary->ToString(), dictionary2->ToString(), "Dictionary strings should not be equal");
		dictionary2->AddEntry(SqlTypes::SqlString("poiu"), SqlTypes::SqlInt32(6));
		Assert::AreEqual(dictionary->ToString(), dictionary2->ToString(), "Dictionary strings should be equal");
	}

	void Dictionary::TestReadingAndWriting() {
		Types::Dictionary^ dictionary = Types::Dictionary::New();

		dictionary->AddEntry(SqlTypes::SqlString("abcd"), SqlTypes::SqlString("abcd"));
		dictionary->AddEntry(SqlTypes::SqlString("efgh"), SqlTypes::SqlString("efgh"));
		dictionary->AddEntry(SqlTypes::SqlString("qwerty"), SqlTypes::SqlInt32(324324));

		IO::MemoryStream^ ms = gcnew IO::MemoryStream;
		IO::BinaryWriter^ writer = gcnew IO::BinaryWriter(ms);

		dictionary->Write(writer);
		writer->Flush();

		ms->Seek(0, IO::SeekOrigin::Begin);
		IO::BinaryReader^ reader = gcnew IO::BinaryReader(ms);

		Types::Dictionary^ dictionary2 = gcnew Types::Dictionary;
		dictionary2->Read(reader);

		Assert::False(dictionary->IsCompressed.Value, "Dictionary should be uncompressed");
		Assert::False(dictionary2->IsCompressed.Value, "Dictionary should be uncompressed");
		Assert::True(dictionary->Equals(dictionary2).Value, "Dictionaries should be equal");
		Assert::True(dictionary2->Equals(dictionary).Value, "Dictionaries should be equal");
		
		ms->Seek(0, IO::SeekOrigin::Begin);
		dictionary->SetCompression(true);

		dictionary->Write(writer);

		ms->Seek(0, IO::SeekOrigin::Begin);
		dictionary2->Read(reader);

		Assert::True(dictionary->IsCompressed.Value, "Dictionary should be uncompressed");
		Assert::True(dictionary2->IsCompressed.Value, "Dictionary should be uncompressed");
		Assert::True(dictionary->Equals(dictionary2).Value, "Dictionaries should be equal");
		Assert::True(dictionary2->Equals(dictionary).Value, "Dictionaries should be equal");

		ms->Seek(0, IO::SeekOrigin::Begin);
		dictionary->SetCompression(false);

		dictionary->Write(writer);

		ms->Seek(0, IO::SeekOrigin::Begin);
		dictionary2->Read(reader);

		Assert::False(dictionary->IsCompressed.Value, "Dictionary should not be uncompressed");
		Assert::False(dictionary2->IsCompressed.Value, "Dictionary should not be uncompressed");
		Assert::True(dictionary->Equals(dictionary2).Value, "Dictionaries should be equal");
		Assert::True(dictionary2->Equals(dictionary).Value, "Dictionaries should be equal");

	}
}}}}