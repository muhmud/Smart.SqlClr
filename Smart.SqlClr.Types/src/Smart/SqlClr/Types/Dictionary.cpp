
#include <Smart/SqlClr/Types/Dictionary.hpp>

#include <Smart/SqlClr/Internal/SqlObject.hpp>
#include <Smart/SqlClr/Types/Enumerator.hpp>

#using <Smart.SqlClr.dll> as_friend
#using <System.Xml.dll>

namespace Smart { namespace SqlClr { namespace Types {
	using namespace System;
	using namespace System::Data::SqlTypes;
	using namespace Microsoft::SqlServer::Server;

	Dictionary^ Dictionary::create() {
		Dictionary^ o = gcnew Dictionary;
		o->m_isNull = 1;

		return o;
	}

	Dictionary^ Dictionary::create(String^ keyType, String^ valueType, bool sorted) {
		Dictionary^ o = gcnew Dictionary;
		if (sorted) {
			o->m_sqlDictionary = gcnew SqlSortedDictionaryImpl;
			o->m_isSorted = 1;
		} else {
			o->m_sqlDictionary = gcnew SqlDictionaryImpl;
			o->m_isSorted = 0;
		}

		try {
			o->m_keyType = Internal::SqlObject::Create(keyType);
		} catch (Exception^ e) {
			throw gcnew SmartSqlClrException(String::Format("Could not process type '{0}' specified for key: {1}", keyType, e->Message));
		}

		if (!o->m_keyType->IsSqlVariantCompatible) {
			throw gcnew SmartSqlClrException(String::Format("Data type '{0}' cannot be used for the key. A sql_variant compatible data type is required.", keyType));
		}

		if (valueType) {
			try {
				o->m_valueType = Internal::SqlObject::Create(valueType);
			} catch (Exception^ e) {
				throw gcnew SmartSqlClrException(String::Format("Could not process type '{0}' specified for value: {1}", valueType, e->Message));
			}
		}

		return o;
	}

	Dictionary^ Dictionary::create(Type^ keyType, Type^ valueType, bool sorted) {
		Dictionary^ o = gcnew Dictionary;
		if (sorted) {
			o->m_sqlDictionary = gcnew SqlSortedDictionaryImpl;
			o->m_isSorted = 1;
		} else {
			o->m_sqlDictionary = gcnew SqlDictionaryImpl;
			o->m_isSorted = 0;
		}

		o->m_keyType = Internal::SqlObject::Create(keyType);

		if (valueType) {
			o->m_valueType = Internal::SqlObject::Create(valueType);
		}

		return o;
	}

	void Dictionary::checkTypes(Object^ key, Object^ value) {
		if (!m_keyType->IsValueValid(key)) {
			throw gcnew SmartSqlClrException(String::Format("Invalid key value '{0}'. Values specified for a key must be '{1}'", key, m_keyType->SqlType));
		}

		if (!m_valueType && value) {
			throw gcnew SmartSqlClrException(String::Format("Invalid value '{0}'. Values must be of Dictionary type", value));
		}

		if (m_valueType && !m_valueType->IsValueValid(value)) {
			throw gcnew SmartSqlClrException(String::Format("Invalid value '{0}'. Values must be '{1}'", value, m_valueType->SqlType));
		}
	}

	void Dictionary::checkTypes(Dictionary^ dictionary) {
		checkTypes(dictionary, false);
	}

	bool Dictionary::checkTypes(Dictionary^ dictionary, bool noThrow) {
		if (m_keyType->SqlType != dictionary->m_keyType->SqlType) {
			if (!noThrow) {
				throw gcnew SmartSqlClrException(String::Format("Supplied dictionary does not have compatible key type. Current instance has key type '{0}', whereas other Dictionary has key type {1}", m_keyType->SqlType, dictionary->m_keyType->SqlType));
			}

			return false;
		}

		if ((m_valueType && !dictionary->m_valueType) || (!m_valueType && dictionary->m_valueType)) {
			if (!noThrow) {
				throw gcnew SmartSqlClrException("Supplied dictionary does not have compatible value type. A Dictionary of nested Dictionaries cannot be combined with a normal Dictionary");
			}

			return false;
		}

		if (m_valueType && dictionary->m_valueType && m_valueType->SqlType != dictionary->m_valueType->SqlType) {
			if (!noThrow) {
				throw gcnew SmartSqlClrException(String::Format("Supplied dictionary does not have compatible value type. Current instance has value type '{0}', whereas other Dictionary has value type {1}", m_valueType->SqlType, dictionary->m_valueType->SqlType));
			}

			return false;
		}

		return true;
	}

	void Dictionary::checkNestedTypes(Object^ key) {
		checkTypes(key, nullptr);

		if (m_valueType) {
			throw gcnew SmartSqlClrException(String::Format("Invalid value. Nested dictionary can not be supplied as values must be '{0}'", m_valueType->SqlType));
		}
	}

	void Dictionary::readHeader(IO::BinaryReader^ reader) {
		String^ keyType = reader->ReadString();
		String^ valueType = reader->ReadString();

		m_keyType = Internal::SqlObject::Create(keyType);
		m_valueType = !String::IsNullOrEmpty(valueType) ? Internal::SqlObject::Create(valueType) : nullptr;

		m_isCompressed = reader->ReadByte();
		m_isSorted = reader->ReadByte();
	}

	void Dictionary::readBody(IO::BinaryReader^ reader) {
		int count = reader->ReadInt32();
		if (count) {
			if (m_isSorted) {
				m_sqlDictionary = gcnew SqlSortedDictionaryImpl(count);
			} else {
				m_sqlDictionary = gcnew SqlDictionaryImpl(count);
			}

			if (m_isCompressed) {
				reader = gcnew IO::BinaryReader(gcnew IO::Compression::GZipStream(reader->BaseStream, IO::Compression::CompressionMode::Decompress, true));
			}
			
			for (int i = 0; i < count; ++i) {
				if (!IsNested.Value) {
					m_sqlDictionary->Add(m_keyType->Read(reader), m_valueType->Read(reader));
				} else {
					Object^ key = m_keyType->Read(reader);

					Dictionary^ dictionary = nullptr;
					Byte isNull = reader->ReadByte();
					if (!isNull) {
						dictionary = gcnew Dictionary;
						dictionary->readHeader(reader);
						dictionary->SetCompression(false);
						dictionary->readBody(reader);
						m_sqlDictionary->Add(key, dictionary);
					} else {
						m_sqlDictionary->Add(key, nullptr);
					}
				}
			}
		} else {
			m_sqlDictionary = gcnew SqlDictionaryImpl;
		}
	}

	Dictionary::SqlDictionary^ Dictionary::getSqlDictionary() {
		return m_sqlDictionary;
	}

	Internal::SqlObject^ Dictionary::getKeyType() {
		return m_keyType;
	}

	Internal::SqlObject^ Dictionary::getValueType() {
		return m_valueType;
	}

	Dictionary^ Dictionary::Copy(bool includeItems, bool useSortedImpl) {
		if (!IsNull) {
			Dictionary^ o = gcnew Dictionary;
			o->m_isCompressed = m_isCompressed;
			o->m_keyType = Internal::SqlObject::Create(m_keyType->SqlType);

			if (includeItems) {
				o->m_sqlDictionary = safe_cast<SqlDictionary^>(safe_cast<SqlDictionaryImpl^>(m_sqlDictionary)->Clone());
				o->m_isSorted = m_isSorted;
			} else {
				if (useSortedImpl) {
					o->m_sqlDictionary = gcnew SqlSortedDictionaryImpl;
					o->m_isSorted = 1;
				} else {
					o->m_sqlDictionary = gcnew SqlDictionaryImpl;
				}
			}

			if (m_valueType) {
				o->m_valueType = Internal::SqlObject::Create(m_valueType->SqlType);
			}

			return o;
		}

		return Null;
	}

	Dictionary^ Dictionary::NewOfType(Type^ keySqlDataType) {
		return create(keySqlDataType, nullptr, false);
	}
	
	Dictionary^ Dictionary::NewOfType(Type^ keySqlDataType, Type^ valueSqlDataType) {
		return create(keySqlDataType, valueSqlDataType, false);
	}

	Dictionary^ Dictionary::NewSortedOfType(Type^ keySqlDataType) {
		return create(keySqlDataType, nullptr, true);
	}
	
	Dictionary^ Dictionary::NewSortedOfType(Type^ keySqlDataType, Type^ valueSqlDataType) {
		return create(keySqlDataType, valueSqlDataType, true);
	}

	Dictionary^ Dictionary::New() {
		return create("string", "sql_variant", false);
	}

	Dictionary^ Dictionary::NewNested() {
		return create("string", nullptr, false);
	}

	Dictionary^ Dictionary::NewOf(SqlString keyType, SqlString valueType) {
		return create(keyType.Value, valueType.Value, false);
	}

	Dictionary^ Dictionary::NewNestedOf(SqlString keyType) {
		return create(keyType.Value, nullptr, false);
	}

	Dictionary^ Dictionary::NewSortedOf(SqlString keyType, SqlString valueType) {
		return create(keyType.Value, valueType.Value, true);
	}

	Dictionary^ Dictionary::NewSortedAndNestedOf(SqlString keyType) {
		return create(keyType.Value, nullptr, true);
	}

	void Dictionary::AddEntry(Object^ key, Object^ value) {
		if (!IsNull) {
			checkTypes(key, value);
			key = Internal::SqlObject::Clean(key);
			value = Internal::SqlObject::Clean(value);

			if (m_sqlDictionary->Contains(key)) {
				throw gcnew SmartSqlClrException(String::Format("Dictionary already contains an entry for key '{0}'", key));
			}

			m_sqlDictionary->Add(key, value);
		}
	}

	void Dictionary::AddNestedEntry(Object^ key, Dictionary^ value) {
		if (!IsNull) {
			checkNestedTypes(key);
			key = Internal::SqlObject::Clean(key);

			if (m_sqlDictionary->Contains(key)) {
				throw gcnew SmartSqlClrException(String::Format("Dictionary already contains an entry for key '{0}'", key));
			}

			m_sqlDictionary->Add(key, value);
		}
	}

	void Dictionary::AddEntryIfNotExists(Object^ key, Object^ value) {
		if (!IsNull) {
			checkTypes(key, value);
			key = Internal::SqlObject::Clean(key);
			value = Internal::SqlObject::Clean(value);

			if (!m_sqlDictionary->Contains(key)) {
				m_sqlDictionary->Add(key, value);
			}
		}
	}

	void Dictionary::AddNestedEntryIfNotExists(Object^ key, Dictionary^ value) {
		if (!IsNull) {
			checkNestedTypes(key);
			key = Internal::SqlObject::Clean(key);

			if (!m_sqlDictionary->Contains(key)) {
				m_sqlDictionary->Add(key, value);
			}
		}
	}

	void Dictionary::UpdateEntry(Object^ key, Object^ newValue) {
		if (!IsNull) {
			checkTypes(key, newValue);
			key = Internal::SqlObject::Clean(key);
			newValue = Internal::SqlObject::Clean(newValue);

			if (!m_sqlDictionary->Contains(key)) {
				throw gcnew SmartSqlClrException(String::Format("Dictionary does not contain an entry for key '{0}'", key));
			}

			m_sqlDictionary[key] = newValue;
		}
	}

	void Dictionary::UpdateNestedEntry(Object^ key, Dictionary^ newValue) {
		if (!IsNull) {
			checkNestedTypes(key);
			key = Internal::SqlObject::Clean(key);

			if (!m_sqlDictionary->Contains(key)) {
				throw gcnew SmartSqlClrException(String::Format("Dictionary does not contain an entry for key '{0}'", key));
			}

			m_sqlDictionary[key] = newValue;
		}
	}

	void Dictionary::UpdateEntryIfExists(Object^ key, Object^ newValue) {
		if (!IsNull) {
			checkTypes(key, newValue);
			key = Internal::SqlObject::Clean(key);
			newValue = Internal::SqlObject::Clean(newValue);

			if (m_sqlDictionary->Contains(key)) {
				m_sqlDictionary[key] = newValue;
			}
		}
	}

	void Dictionary::UpdateNestedEntryIfExists(Object^ key, Dictionary^ newValue) {
		if (!IsNull) {
			checkNestedTypes(key);
			key = Internal::SqlObject::Clean(key);

			if (m_sqlDictionary->Contains(key)) {
				m_sqlDictionary[key] = newValue;
			}
		}
	}

	void Dictionary::MergeEntry(Object^ key, Object^ value) {
		if (!IsNull) {
			checkTypes(key, value);
			key = Internal::SqlObject::Clean(key);
			value = Internal::SqlObject::Clean(value);

			if (!m_sqlDictionary->Contains(key)) {
				m_sqlDictionary->Add(key, value);
			} else {
				m_sqlDictionary[key] = value;
			}
		}
	}

	void Dictionary::MergeNestedEntry(Object^ key, Dictionary^ value) {
		if (!IsNull) {
			checkNestedTypes(key);
			key = Internal::SqlObject::Clean(key);

			if (!m_sqlDictionary->Contains(key)) {
				m_sqlDictionary->Add(key, value);
			} else {
				m_sqlDictionary[key] = value;
			}
		}
	}

	void Dictionary::RemoveEntry(Object^ key) {
		if (!IsNull) {
			checkTypes(key, nullptr);
			key = Internal::SqlObject::Clean(key);

			if (!m_sqlDictionary->Contains(key)) {
				throw gcnew SmartSqlClrException(String::Format("Dictionary does not contain an entry for key '{0}'", key));
			}

			m_sqlDictionary->Remove(key);
		}
	}

	void Dictionary::RemoveEntryIfExists(Object^ key) {
		if (!IsNull) {
			checkTypes(key, nullptr);
			key = Internal::SqlObject::Clean(key);

			if (m_sqlDictionary->Contains(key)) {
				m_sqlDictionary->Remove(key);
			}
		}
	}

	SqlBoolean Dictionary::ContainsKey(Object^ key) {
		if (!IsNull) {
			checkTypes(key, nullptr);
			key = Internal::SqlObject::Clean(key);

			return SqlBoolean(m_sqlDictionary->Contains(key));
		}

		return SqlBoolean::Null;
	}

	void Dictionary::AddDictionary(Dictionary^ dictionary) {
		if (!IsNull && this != dictionary) {
			checkTypes(dictionary);

			for each(Object^ key in dictionary->m_sqlDictionary->Keys) {
				if (!m_sqlDictionary->Contains(key)) {
					m_sqlDictionary->Add(key, dictionary->m_sqlDictionary[key]);
				}
			}
		}
	}

	void Dictionary::AddFromDictionary(Dictionary^ dictionary) {
		if (!IsNull && this != dictionary) {
			checkTypes(dictionary);

			for each(Object^ key in dictionary->m_sqlDictionary->Keys) {
				if (m_sqlDictionary->Contains(key)) {
					throw gcnew SmartSqlClrException(String::Format("Dictionary objects could not be combined. Duplicate key '{0}' was found.", key->ToString()));
				}

				m_sqlDictionary->Add(key, dictionary->m_sqlDictionary[key]);
			}
		}
	}

	void Dictionary::UpdateDictionary(Dictionary^ dictionary) {
		if (!IsNull && this != dictionary) {
			checkTypes(dictionary);

			for each(Object^ key in dictionary->m_sqlDictionary->Keys) {
				if (m_sqlDictionary->Contains(key)) {
					m_sqlDictionary[key] = dictionary->m_sqlDictionary[key];
				}
			}
		}
	}

	void Dictionary::RemoveDictionary(Dictionary^ dictionary) {
		if (!IsNull && this != dictionary) {
			checkTypes(dictionary);

			for each(Object^ key in dictionary->m_sqlDictionary->Keys) {
				if (m_sqlDictionary->Contains(key)) {
					m_sqlDictionary->Remove(key);
				}
			}
		}
	}

	void Dictionary::MergeDictionary(Dictionary^ dictionary) {
		if (!IsNull && this != dictionary) {
			checkTypes(dictionary);

			for each(Object^ key in dictionary->m_sqlDictionary->Keys) {
				m_sqlDictionary[key] = dictionary->m_sqlDictionary[key];
			}
		}
	}

	void Dictionary::Clear() {
		if (!IsNull) {
			m_sqlDictionary->Clear();
		}
	}

	Dictionary^ Dictionary::Clone() {
		return Copy(true, false);
	}

	void Dictionary::SetCompression(SqlBoolean compressed) {
		if (!IsNull) {
			m_isCompressed = compressed.Value ? 1 : 0;
		}
	}

	SqlBoolean Dictionary::Equals(Dictionary^ dictionary) {
		if (!IsNull) {
			if (checkTypes(dictionary, true)) {
				if (Count.Value == dictionary->Count.Value) {
					bool equal = true;
					for each(SqlDictionaryEntry entry in m_sqlDictionary) {
						if (!dictionary->m_sqlDictionary->Contains(entry.Key) || !entry.Value->Equals(dictionary->m_sqlDictionary[entry.Key])) {
							equal = false;
							break;
						}
					}

					return equal;
				}
			}
		}

		return false;
	}

	Object^ Dictionary::Get(Object^ key) {
		if (!IsNull) {
			checkTypes(key, nullptr);
			key = Internal::SqlObject::Clean(key);

			if (!m_sqlDictionary->Contains(key)) {
				throw gcnew SmartSqlClrException(String::Format("Dictionary does not contain an entry for key '{0}'", key));
			}

			return m_sqlDictionary[key];
		}

		return Null;
	}

	Object^ Dictionary::GetIfExists(Object^ key) {
		if (!IsNull) {
			checkTypes(key, nullptr);
			key = Internal::SqlObject::Clean(key);

			if (m_sqlDictionary->Contains(key)) {
				return m_sqlDictionary[key];
			}
		}

		return nullptr;
	}

	Dictionary^ Dictionary::GetDictionary(Object^ key) {
		if (!IsNull) {
			checkNestedTypes(key);
			key = Internal::SqlObject::Clean(key);

			if (!m_sqlDictionary->Contains(key)) {
				throw gcnew SmartSqlClrException(String::Format("Dictionary does not contain an entry for key '{0}'", key));
			}

			return safe_cast<Dictionary^>(m_sqlDictionary[key]);
		}

		return Null;
	}

	Dictionary^ Dictionary::GetDictionaryIfExists(Object^ key) {
		if (!IsNull) {
			checkNestedTypes(key);
			key = Internal::SqlObject::Clean(key);

			if (m_sqlDictionary->Contains(key)) {
				return safe_cast<Dictionary^>(m_sqlDictionary[key]);
			}
		}

		return Null;
	}

	Enumerator^ Dictionary::Keys::get() {
		return IsNull ? Enumerator::Null : Enumerator::New(m_sqlDictionary->Keys, m_keyType, m_isSorted == 1, m_isCompressed == 1);
	}

	Enumerator^ Dictionary::Values::get() {
		return IsNull ? Enumerator::Null : Enumerator::New(m_sqlDictionary->Values, m_valueType, m_isSorted == 1, m_isCompressed == 1);
	}

	SqlString Dictionary::KeyType::get() {
		return IsNull ? SqlString::Null : SqlString(m_keyType->SqlType);
	}

	SqlString Dictionary::ValueType::get() {
		return IsNull ? SqlString::Null : SqlString(m_valueType->SqlType);
	}

	SqlInt32 Dictionary::Count::get() {
		return IsNull ? SqlInt32::Null : SqlInt32(m_sqlDictionary->Count);
	}

	SqlBoolean Dictionary::IsCompressed::get() {
		return IsNull ? SqlBoolean::Null : SqlBoolean(m_isCompressed == 1);
	}

	SqlBoolean Dictionary::IsSorted::get() {
		return IsNull ? SqlBoolean::Null : SqlBoolean(m_isSorted == 1);
	}

	bool Dictionary::IsNull::get() {
		return m_isNull == 1;
	}

	SqlBoolean Dictionary::IsNested::get() {
		return IsNull ? SqlBoolean::Null : SqlBoolean(m_valueType == nullptr);
	}

	Dictionary^ Dictionary::Null::get() {
		return NullDictionary;
	}

	String^ Dictionary::ToString() {
		if (!IsNull) {
			Text::StringBuilder^ builder = gcnew Text::StringBuilder;

			Xml::XmlWriterSettings^ settings = gcnew Xml::XmlWriterSettings;
			settings->OmitXmlDeclaration = true;

			Xml::XmlWriter^ writer = Xml::XmlWriter::Create(builder, settings);

			writer->WriteStartElement(XML_ROOT_ELEMENT);
			writer->WriteAttributeString(XML_KEYTYPE_ATTRIBUTE, m_keyType->SqlType);

			if (m_valueType) {
				writer->WriteAttributeString(XML_VALUETYPE_ATTRIBUTE, m_valueType->SqlType);
			}

			writer->WriteAttributeString(XML_ISCOMPRESSED_ATTRIBUTE, IsCompressed.ToString());
			
			for each(SqlDictionaryEntry entry in m_sqlDictionary) {
				writer->WriteStartElement(XML_ENTRY_ELEMENT);
				writer->WriteAttributeString(XML_ENTRY_KEY_ATTRIBUTE, m_keyType->ConvertToString(entry.Key));

				if (!IsNested.Value) {
					writer->WriteString(m_valueType->ConvertToString(entry.Value));
				} else {
					writer->WriteRaw(entry.Value->ToString());
				}

				writer->WriteEndElement();
			}

			writer->WriteEndElement();
			writer->Flush();

			return builder->ToString();
		}

		return nullptr;
	}

	String^ Dictionary::ToSimpleString() {
		if (!IsNull) {
			Text::StringBuilder^ builder = gcnew Text::StringBuilder("{ ");
			bool start = true;

			for each(SqlDictionaryEntry entry in m_sqlDictionary) {
				builder->Append(!start ? ", " : String::Empty);
				builder->Append(m_keyType->ConvertToString(entry.Key));
				builder->Append(" -> ");

				if (!IsNested.Value) {
					builder->Append(m_valueType->ConvertToString(entry.Value));
				} else {
					builder->Append(safe_cast<Dictionary^>(entry.Value)->ToSimpleString());
				}

				start = false;
			}

			builder->Append(" }");
			return builder->ToString();
		}

		return nullptr;
	}

	Dictionary^ Dictionary::Parse(SqlString sqlValue) {
		String^ value = sqlValue.Value;
		Xml::XmlReader^ reader = Xml::XmlReader::Create(gcnew IO::StringReader(value));

		if (reader->IsStartElement(XML_ROOT_ELEMENT)) {
			String^ keyType = reader->GetAttribute(XML_KEYTYPE_ATTRIBUTE);
			String^ valueType = reader->GetAttribute(XML_VALUETYPE_ATTRIBUTE);
			
			String^ compressedString = reader->GetAttribute(XML_ISCOMPRESSED_ATTRIBUTE);
			bool compressed = compressedString ? Boolean::Parse(compressedString) : false;

			Dictionary^ dictionary = nullptr;
			if (!String::IsNullOrEmpty(valueType)) {
				dictionary = NewOf(keyType, valueType);
			} else {
				dictionary = NewNestedOf(keyType);
			}

			dictionary->SetCompression(compressed);

			if (reader->ReadToFollowing(XML_ENTRY_ELEMENT)) {
				do {
					String^ key = reader->GetAttribute(XML_ENTRY_KEY_ATTRIBUTE);
					if (!dictionary->IsNested.Value) {
						dictionary->AddEntry(dictionary->m_keyType->Parse(key), dictionary->m_valueType->Parse(reader->ReadString()));
					} else {
						dictionary->AddNestedEntry(dictionary->m_keyType->Parse(key), Dictionary::Parse(reader->ReadInnerXml()));
					}
				} while (reader->ReadToNextSibling(XML_ENTRY_ELEMENT));
			}

			return dictionary;
		}
		
		return Null;
	}

	void Dictionary::Read(IO::BinaryReader^ reader) {
		m_isNull = reader->ReadByte();
		if (!m_isNull) {
			readHeader(reader);
			readBody(reader);
		}
	}

	void Dictionary::Write(IO::BinaryWriter^ writer) {
		writer->Write(m_isNull);
		if (!m_isNull) {
			writer->Write(m_keyType->SqlType);
			if (m_valueType) {
				writer->Write(m_valueType->SqlType);
			} else {
				writer->Write(System::String::Empty);
			}
			writer->Write(m_isCompressed);
			writer->Write(m_isSorted);

			writer->Write(m_sqlDictionary->Count);

			if (m_sqlDictionary->Count) {
				IO::Compression::GZipStream^ gzip = nullptr;
				if (m_isCompressed) {
					gzip = gcnew IO::Compression::GZipStream(writer->BaseStream, IO::Compression::CompressionMode::Compress, true);
					writer = gcnew IO::BinaryWriter(gzip);
				}

				for each(SqlDictionaryEntry entry in m_sqlDictionary) {
					m_keyType->Write(entry.Key, writer);

					if (!IsNested.Value) {
						m_valueType->Write(entry.Value, writer);
					} else {
						Dictionary^ dictionary = safe_cast<Dictionary^>(entry.Value);
						dictionary->SetCompression(false);
						dictionary->Write(writer);
					}
				}

				if (m_isCompressed) {
					writer->Flush();
					gzip->Close();
				}
			}
		}
	}
}}}
