
#include <Smart/SqlClr/Utils/CollectionAlgorithm.hpp>

#include <Smart/SqlClr/Types/List.hpp>
#include <Smart/SqlClr/Types/Enumerator.hpp>
#include <Smart/SqlClr/Types/Dictionary.hpp>
#include <Smart/SqlClr/Internal/SqlObject.hpp>

#using <Smart.SqlClr.dll> as_friend

namespace Smart { namespace SqlClr { namespace Utils {
	using namespace System;
	using namespace System::Data::SqlTypes;
	using namespace Microsoft::SqlServer::Server;

	Object^ CollectionAlgorithm::Sum(Types::Enumerator^ enumerator) {
		if (enumerator && !enumerator->IsNull && enumerator->Count.Value > 0) {
			if (!enumerator->getItemType()->IsNumeric) {
				throw gcnew SmartSqlClrException("This algorithm can only be used with lists that use a numeric data type");
			}

			Types::Enumerator::SqlEnumerator^ sqlEnumerator = enumerator->GetSqlEnumerator();
			sqlEnumerator->MoveNext();
			Object^ result = sqlEnumerator->Current;
			Reflection::MethodInfo^ addMethod = result->GetType()->GetMethod("Add");
			while (sqlEnumerator->MoveNext()) {
				result = addMethod->Invoke(nullptr, gcnew cli::array<Object^> { result, sqlEnumerator->Current });
			}

			return result;
		}

		return nullptr;
	}

	SqlInt32 CollectionAlgorithm::CountDistinct(Types::Enumerator^ enumerator) {
		if (enumerator && !enumerator->IsNull) {
			if (enumerator->Count.Value > 0) {
				if (enumerator->IsSorted.Value) {
					Types::Enumerator::SqlEnumerator^ sqlEnumerator = enumerator->GetSqlEnumerator();
					sqlEnumerator->MoveNext();
					Object^ current = sqlEnumerator->Current;
					int result = 1;
					while (sqlEnumerator->MoveNext()) {
						Object^ next = sqlEnumerator->Current;
						if (!current->Equals(next)) {
							++result;
							current = next;
						}
					}

					return result;
				} else {
					return CountDistinct(SortCopyEnumerator(enumerator));
				}
			}

			return 0;
		}

		return SqlInt32::Null;
	}

	Object^ CollectionAlgorithm::Avg(Types::Enumerator^ enumerator) {
		Object^ result = Sum(enumerator);
		if (result) {
			Reflection::MethodInfo^ divideMethod = result->GetType()->GetMethod("Divide");
			result = divideMethod->Invoke(nullptr, gcnew cli::array<Object^> { result, enumerator->Count.Value });

			return result;
		}

		return nullptr;
	}

	Object^ CollectionAlgorithm::Product(Types::Enumerator^ enumerator) {
		if (enumerator && !enumerator->IsNull && enumerator->Count.Value > 0) {
			if (!enumerator->getItemType()->IsNumeric) {
				throw gcnew SmartSqlClrException("This algorithm can only be used with collections that use a numeric data type");
			}

			Types::Enumerator::SqlEnumerator^ sqlEnumerator = enumerator->GetSqlEnumerator();
			sqlEnumerator->MoveNext();
			Object^ result = sqlEnumerator->Current;
			Reflection::MethodInfo^ multiplyMethod = result->GetType()->GetMethod("Multiply");
			while (sqlEnumerator->MoveNext()) {
				result = multiplyMethod->Invoke(nullptr, gcnew cli::array<Object^> { result, sqlEnumerator->Current });
			}

			return result;
		}

		return nullptr;
	}

	Object^ CollectionAlgorithm::Min(Types::Enumerator^ enumerator) {
		if (enumerator && !enumerator->IsNull && enumerator->Count.Value > 0) {
			Types::Enumerator::SqlEnumerator^ sqlEnumerator = enumerator->GetSqlEnumerator();
			sqlEnumerator->MoveNext();

			if (!enumerator->IsSorted.Value) {
				IComparable^ result = safe_cast<IComparable^>(sqlEnumerator->Current);
				while(sqlEnumerator->MoveNext()) {
					Object^ current = sqlEnumerator->Current;
					if (result->CompareTo(current) > 0) {
						result = safe_cast<IComparable^>(current);
					}
				}

				return result;
			}

			return sqlEnumerator->Current;
		}

		return nullptr;
	}

	Object^ CollectionAlgorithm::Max(Types::Enumerator^ enumerator) {
		if (enumerator && !enumerator->IsNull) {
			int count = enumerator->Count.Value;
			if (count > 0) {
				if (enumerator->IsSorted.Value) {
					if (enumerator->GetSqlEnumerable()->GetType()->GetInterface("System.Collections.IList") != nullptr) {
						return safe_cast<System::Collections::IList^>(enumerator->GetSqlEnumerable())[count - 1];
					}
				}

				Types::Enumerator::SqlEnumerator^ sqlEnumerator = enumerator->GetSqlEnumerator();
				sqlEnumerator->MoveNext();

				if (!enumerator->IsSorted.Value) {
					IComparable^ result = safe_cast<IComparable^>(sqlEnumerator->Current);
					while(sqlEnumerator->MoveNext()) {
						Object^ current = sqlEnumerator->Current;
						if (result->CompareTo(current) < 0) {
							result = safe_cast<IComparable^>(current);
						}
					}

					return result;
				}

				// Sorted but not a list
				while (sqlEnumerator->MoveNext());

				return sqlEnumerator->Current;
			}
		}

		return nullptr;
	}

	Object^ CollectionAlgorithm::Median(Types::Enumerator^ enumerator) {
		return nullptr;
	}

	SqlString CollectionAlgorithm::Concatenate(Types::Enumerator^ enumerator) {
		return ConcatenateWithSeparator(enumerator, SqlString::Null);
	}

	SqlString CollectionAlgorithm::ConcatenateWithSeparator(Types::Enumerator^ enumerator, SqlString separator) {
		if (enumerator && !enumerator->IsNull && enumerator->Count.Value > 0) {
			if (enumerator->getItemType()->SqlDataType != SqlString::typeid) {
				throw gcnew SmartSqlClrException("This algorithm can only be used with lists that use a string data type");
			}

			Types::Enumerator::SqlEnumerator^ sqlEnumerator = enumerator->GetSqlEnumerator();
			sqlEnumerator->MoveNext();
			Text::StringBuilder^ builder = gcnew Text::StringBuilder(safe_cast<SqlString^>(sqlEnumerator->Current)->Value);
			while (sqlEnumerator->MoveNext()) {
				if (!separator.IsNull) {
					builder->Append(separator.Value);
				}

				builder->Append(safe_cast<SqlString^>(sqlEnumerator->Current)->Value);
			}

			return builder->ToString();
		}

		return SqlString::Null;
	}

	Types::List^ CollectionAlgorithm::RemoveDuplicates(Types::List^ list) {
		if (list && !list->IsNull) {
			if (list->Count.Value > 0) {
				if (list->IsSorted.Value) {
					Types::List::SqlList^ sqlList = list->getSqlList();
					Object^ current = sqlList[0];

					Types::List^ result = list->Copy(false);
					Types::List::SqlList^ resultSqlList = result->getSqlList();
					resultSqlList->Add(current);

					for (int i = 1; i < sqlList->Count; ++i) {
						Object^ next = sqlList[i];
						if (!current->Equals(next)) {
							current = next;
							resultSqlList->Add(current);
						}
					}

					return result;
				} else {
					// Expensive!
					Types::List^ clone = list->Clone();
					clone->Sort();
					return RemoveDuplicates(clone);
				}
			}

			return list->Clone();
		}

		return Types::List::Null;
	}

	Types::List^ CollectionAlgorithm::SortCopyList(Types::List^ list) {
		if (list && !list->IsNull) {
			Types::List^ clone = list->Clone();
			clone->Sort();
			return clone;
		}

		return Types::List::Null;
	}

	Types::Dictionary^ CollectionAlgorithm::SortCopyDictionary(Types::Dictionary^ dictionary) {
		if (dictionary && !dictionary->IsNull) {
			Types::Dictionary^ clone = dictionary->Copy(false, true);

			for each(Types::Dictionary::SqlDictionaryEntry entry in dictionary->getSqlDictionary()) {
				clone->getSqlDictionary()->Add(entry.Key, entry.Value);
			}

			return clone;
		}

		return Types::Dictionary::Null;
	}

	Types::Enumerator^ CollectionAlgorithm::SortCopyEnumerator(Types::Enumerator^ enumerator) {
		if (enumerator && !enumerator->IsNull) {
			Types::List^ list = ConvertEnumeratorToList(enumerator);
			list->Sort();

			return list->Items;
		}

		return Types::Enumerator::Null;
	}

	Types::List^ CollectionAlgorithm::ConvertEnumeratorToList(Types::Enumerator^ enumerator) {
		if (enumerator && !enumerator->IsNull) {
			Types::List^ list = Types::List::NewOfType(enumerator->getItemType()->SqlDataType);
			for each(Object^ o in enumerator->GetSqlEnumerable()) {
				list->getSqlList()->Add(o);
			}

			return list;
		}

		return Types::List::Null;
	}
}}}
