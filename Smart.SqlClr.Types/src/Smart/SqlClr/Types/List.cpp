
#include <Smart/SqlClr/Types/List.hpp>

#include <Smart/SqlClr/Internal/SqlObject.hpp>
#include <Smart/SqlClr/Types/Enumerator.hpp>
#include <Smart/SqlClr/Types/Variant.hpp>

#using <Smart.SqlClr.dll> as_friend
#using <System.Xml.dll>

namespace Smart { namespace SqlClr { namespace Types {
	using namespace System;
	using namespace System::Data::SqlTypes;
	using namespace Microsoft::SqlServer::Server;

	List^ List::create() {
		List^ o = gcnew List;
		o->m_isNull = 1;

		return o;
	}

	List^ List::create(String^ valueType) {
		List^ o = gcnew List;
		o->m_isSorted = 0;
		o->m_sqlList = gcnew SqlListImpl;
		
		if (valueType) {
			try {
				o->m_valueType = Internal::SqlObject::Create(valueType);
			} catch (Exception^ e) {
				throw gcnew SmartSqlClrException(String::Format("Could not process type '{0}': {1}", valueType, e->Message));
			}

			if (!o->m_valueType->IsSqlVariantCompatible) {
				throw gcnew SmartSqlClrException(String::Format("Could not process type '{0}': Specified value type must be sql_variant compatible. Use a Variant collection if necessary", valueType));
			}
		}

		return o;
	}

	List^ List::create(Type^ sqlDataType) {
		List^ o = gcnew List;
		o->m_isSorted = 0;
		o->m_sqlList = gcnew SqlListImpl;		
		o->m_valueType = Internal::SqlObject::Create(sqlDataType);

		return o;
	}

	void List::checkTypes(Object^ value) {
		if (!m_valueType && value) {
			throw gcnew SmartSqlClrException(String::Format("Invalid value '{0}'. Values must be of Variant type", value));
		}

		if (m_valueType && !m_valueType->IsValueValid(value)) {
			throw gcnew SmartSqlClrException(String::Format("Invalid value '{0}'. Values must be '{1}'", value, m_valueType->SqlType));
		}
	}

	void List::checkTypes(List^ list) {
		checkTypes(list, false);
	}

	bool List::checkTypes(List^ list, bool noThrow) {
		if ((m_valueType && !list->m_valueType) || (!m_valueType && list->m_valueType)) {
			if (!noThrow) {
				throw gcnew SmartSqlClrException("Supplied List does not have compatible value type. An List of Variants cannot be combined with a normal List");
			}

			return false;
		}

		if (m_valueType && list->m_valueType && m_valueType->SqlType != list->m_valueType->SqlType) {
			if (!noThrow) {
				throw gcnew SmartSqlClrException(String::Format("Supplied List does not have compatible value type. Current instance has value type '{0}', whereas other List has value type {1}", m_valueType->SqlType, list->m_valueType->SqlType));
			}

			return false;
		}

		return true;
	}

	void List::checkVariantTypes() {
		if (m_valueType) {
			throw gcnew SmartSqlClrException(String::Format("Invalid value. Variant List can not be supplied as values must be '{0}'", m_valueType->SqlType));
		}
	}

	void List::checkNonVariantTypes() {
		if (!m_valueType) {
			throw gcnew SmartSqlClrException(String::Format("Invalid value. Normal values can not be used with Variant Lists"));
		}
	}

	void List::readHeader(IO::BinaryReader^ reader) {
		String^ valueType = reader->ReadString();
		m_valueType = !String::IsNullOrEmpty(valueType) ? Internal::SqlObject::Create(valueType) : nullptr;

		m_isCompressed = reader->ReadByte();
		m_isSorted = reader->ReadByte();
	}

	void List::readBody(IO::BinaryReader^ reader) {
		int count = reader->ReadInt32();
		if (count) {
			m_sqlList = gcnew SqlListImpl(count);
			
			if (m_isCompressed) {
				reader = gcnew IO::BinaryReader(gcnew IO::Compression::GZipStream(reader->BaseStream, IO::Compression::CompressionMode::Decompress, true));
			}
			
			for (int i = 0; i < count; ++i) {
				if (!IsVariant.Value) {
					m_sqlList->Add(m_valueType->Read(reader));
				} else {
					Variant^ variant = gcnew Variant;
					variant->Read(reader);

					m_sqlList->Add(variant);
				}
			}
		} else {
			m_sqlList = gcnew SqlListImpl;
		}
	}
	
	List::SqlList^ List::getSqlList() {
		return m_sqlList;
	}

	Internal::SqlObject^ List::getValueType() {
		return m_valueType;
	}

	List^ List::Copy(bool includeItems) {
		if (!IsNull) {
			List^ list = gcnew List;
			list->m_isCompressed = m_isCompressed;
			list->m_isReadOnly = m_isReadOnly;

			if (includeItems) {
				list->m_sqlList = safe_cast<SqlList^>(safe_cast<SqlListImpl^>(m_sqlList)->Clone());
				list->m_isSorted = m_isSorted;
			} else {
				list->m_sqlList = gcnew SqlListImpl;
			}

			if (m_valueType) {
				list->m_valueType = Internal::SqlObject::Create(m_valueType->SqlType);
			}

			return list;
		}

		return Null;
	}

	List^ List::NewOfType(Type^ sqlDataType) {
		return create(sqlDataType);
	}

	List^ List::New() {
		return create(Internal::SqlObject::SqlVariant);
	}

	List^ List::NewOfVariant() {
		return create(safe_cast<String^>(nullptr));
	}

	List^ List::NewOf(SqlString valueType) {
		return create(valueType.Value);
	}

	void List::AddItem(Object^ value) {
		if (!IsNull) {
			checkTypes(value);
			value = Internal::SqlObject::Clean(value);
			
			m_sqlList->Add(value);
			m_isSorted = 0;
		}
	}

	void List::AddVariant(Variant^ value) {
		if (!IsNull) {
			checkVariantTypes();

			m_sqlList->Add(value);
			m_isSorted = 0;
		}
	}

	void List::AddIfNotExists(Object^ value) {
		if (!IsNull) {
			checkTypes(value);
			value = Internal::SqlObject::Clean(value);

			if (!m_sqlList->Contains(value)) {
				m_sqlList->Add(value);
				m_isSorted = 0;
			}
		}
	}

	void List::InsertAt(int index, Object^ value) {
		if (!IsNull) {
			checkTypes(value);
			value = Internal::SqlObject::Clean(value);

			m_sqlList->Insert(index, value);
			m_isSorted = 0;
		}
	}

	void List::InsertVariantAt(int index, Variant^ value) {
		if (!IsNull) {
			checkVariantTypes();

			m_sqlList->Insert(index, value);
			m_isSorted = 0;
		}
	}

	void List::Remove(Object^ value) {
		if (!IsNull) {
			checkTypes(value);
			value = Internal::SqlObject::Clean(value);

			if (!m_sqlList->Contains(value)) {
				throw gcnew SmartSqlClrException(String::Format("List does not contain an item for '{0}'", value));
			}

			m_sqlList->Remove(value);
			m_isSorted = 0;
		}
	}

	void List::RemoveIfExists(Object^ value) {
		if (!IsNull) {
			checkTypes(value);
			value = Internal::SqlObject::Clean(value);

			if (m_sqlList->Contains(value)) {
				m_sqlList->Remove(value);
				m_isSorted = 0;
			}
		}
	}

	void List::RemoveAll(Object^ value) {
		if (!IsNull) {
			checkTypes(value);
			value = Internal::SqlObject::Clean(value);

			while (m_sqlList->Contains(value)) {
				m_sqlList->Remove(value);
				m_isSorted = 0;
			}
		}
	}

	void List::RemoveAt(int index) {
		if (!IsNull) {
			if (index < 0 || index >= m_sqlList->Count) {
				throw gcnew SmartSqlClrException(String::Format("Invalid index. List only contains {0} items", m_sqlList->Count));
			}

			m_sqlList->RemoveAt(index);
			m_isSorted = 0;
		}
	}

	SqlInt32 List::IndexOf(Object^ value) {
		if (!IsNull) {
			checkTypes(value);
			value = Internal::SqlObject::Clean(value);

			return m_sqlList->IndexOf(value);
		}

		return SqlInt32::Null;
	}

	SqlBoolean List::ContainsValue(Object^ value) {
		if (!IsNull) {
			checkTypes(value);
			value = Internal::SqlObject::Clean(value);

			return m_sqlList->Contains(value);
		}

		return SqlBoolean::Null;
	}

	void List::AddFromList(List^ a) {
		if (!IsNull) {
			checkTypes(a);

			if (a->m_sqlList->Count > 0) {
				m_isSorted = 0;

				for each(Object^ value in a->m_sqlList) {
					m_sqlList->Add(value);	
				}
			}
		}
	}

	void List::Clear() {
		if (!IsNull) {
			m_sqlList->Clear();
			m_isSorted = 0;
		}
	}

	void List::Sort() {
		if (!IsNull && !IsSorted.Value) {
			checkNonVariantTypes();

			safe_cast<SqlListImpl^>(m_sqlList)->Sort();
			m_isSorted = 1;
		}
	}

	List^ List::Clone() {
		return Copy(true);
	}

	void List::SetCompression(SqlBoolean compressed) {
		if (!IsNull) {
			if (!compressed.IsNull) {
				m_isCompressed = compressed.Value ? 1 : 0;
			}
		}
	}

	SqlBoolean List::Equals(List^ list) {
		if (!IsNull && !list->IsNull) {
			if (m_sqlList->Count == list->m_sqlList->Count) {
				bool equal = true;
				for (int i = 0; i < m_sqlList->Count; ++i) {
					if (!m_sqlList[i]->Equals(list->m_sqlList[i])) {
						equal = false;
						break;
					}
				}

				return equal;
			}

			return false;
		}

		return SqlBoolean::Null;
	}

	Object^ List::Get(int index) {
		if (!IsNull) {
			checkNonVariantTypes();

			if (index < 0 || index >= m_sqlList->Count) {
				throw gcnew SmartSqlClrException(String::Format("Invalid index '{0}'. List contains {1} items.", index, m_sqlList->Count));
			}

			return m_sqlList[index];
		}

		return Null;
	}

	Variant^ List::GetVariant(int index) {
		if (!IsNull) {
			checkVariantTypes();

			if (index < 0 || index >= m_sqlList->Count) {
				throw gcnew SmartSqlClrException(String::Format("Invalid index '{0}'. List contains {1} items.", index, m_sqlList->Count));
			}

			return safe_cast<Variant^>(m_sqlList[index]);
		}

		return Variant::Null;
	}

	Enumerator^ List::Items::get() {
		return IsNull ? Enumerator::Null : Enumerator::New(m_sqlList, m_valueType, m_isSorted == 1, m_isCompressed == 1);
	}

	SqlString List::ValueType::get() {
		return IsNull ? SqlString::Null : SqlString(m_valueType->SqlType);
	}

	SqlInt32 List::Count::get() {
		return IsNull ? SqlInt32::Null : SqlInt32(m_sqlList->Count);
	}

	SqlBoolean List::IsCompressed::get() {
		return IsNull ? SqlBoolean::Null : SqlBoolean(m_isCompressed);
	}

	bool List::IsNull::get() {
		return m_isNull == 1;
	}

	SqlBoolean List::IsVariant::get() {
		return IsNull ? SqlBoolean::Null : SqlBoolean(m_valueType == nullptr);
	}
	
	SqlBoolean List::IsSorted::get() {
		return IsNull ? SqlBoolean::Null : SqlBoolean(m_isSorted);
	}

	List^ List::Null::get() {
		return NullList;
	}

	String^ List::ToString() {
		if (!IsNull) {
			Text::StringBuilder^ builder = gcnew Text::StringBuilder;

			Xml::XmlWriterSettings^ settings = gcnew Xml::XmlWriterSettings;
			settings->OmitXmlDeclaration = true;

			Xml::XmlWriter^ writer = Xml::XmlWriter::Create(builder, settings);

			writer->WriteStartElement(XML_ROOT_ELEMENT);
			if (m_valueType) {
				writer->WriteAttributeString(XML_VALUETYPE_ATTRIBUTE, m_valueType->SqlType);
			}

			writer->WriteAttributeString(XML_ISCOMPRESSED_ATTRIBUTE, IsCompressed.ToString());
			writer->WriteAttributeString(XML_ISSORTED_ATTRIBUTE, IsSorted.ToString());
			
			for each(Object^ value in m_sqlList) {
				writer->WriteStartElement(XML_ITEM_ELEMENT);

				if (!IsVariant.Value) {
					writer->WriteString(m_valueType->ConvertToString(value));
				} else {
					writer->WriteRaw(value->ToString());
				}

				writer->WriteEndElement();
			}

			writer->WriteEndElement();
			writer->Flush();

			return builder->ToString();
		}

		return nullptr;
	}

	String^ List::ToSimpleString() {
		if (!IsNull) {
			Text::StringBuilder^ builder = gcnew Text::StringBuilder("[ ");
			bool start = true;

			for each(Object^ value in m_sqlList) {
				builder->Append(!start ? ", " : String::Empty);
				if (!IsVariant.Value) {
					builder->Append(m_valueType->ConvertToString(value, false));
				} else {
					builder->Append(safe_cast<List^>(value)->ToSimpleString());
				}

				start = false;
			}

			builder->Append(" ]");
			return builder->ToString();
		}

		return nullptr;
	}

	List^ List::Parse(SqlString sqlValue) {
		String^ value = sqlValue.Value;
		Xml::XmlReader^ reader = Xml::XmlReader::Create(gcnew IO::StringReader(value));

		if (reader->IsStartElement(XML_ROOT_ELEMENT)) {
			String^ valueType = reader->GetAttribute(XML_VALUETYPE_ATTRIBUTE);
			
			String^ compressedString = reader->GetAttribute(XML_ISCOMPRESSED_ATTRIBUTE);
			bool compressed = compressedString ? Boolean::Parse(compressedString) : false;

			String^ sortedString = reader->GetAttribute(XML_ISSORTED_ATTRIBUTE);
			bool sorted = sortedString ? Boolean::Parse(sortedString) : false;

			List^ list = nullptr;
			if (!String::IsNullOrEmpty(valueType) && valueType->ToLower() != "variant") {
				list = NewOf(valueType);
			} else {
				list = NewOfVariant();
			}

			list->SetCompression(compressed);
			list->m_isSorted = sorted ? 1 : 0;

			if (reader->ReadToFollowing(XML_ITEM_ELEMENT)) {
				do {
					if (!list->IsVariant.Value) {
						list->AddItem(list->m_valueType->Parse(reader->ReadString()));
					} else {
						list->AddVariant(Variant::Parse(reader->ReadInnerXml()));
					}
				} while (reader->ReadToNextSibling(XML_ITEM_ELEMENT));
			}

			return list;
		}

		return Null;
	}

	void List::Read(IO::BinaryReader^ reader) {
		m_isNull = reader->ReadByte();
		if (!m_isNull) {
			readHeader(reader);
			readBody(reader);
		}
	}

	void List::Write(IO::BinaryWriter^ writer) {
		writer->Write(m_isNull);
		if (!m_isNull) {
			if (IsVariant.Value) {
				writer->Write(String::Empty);
			} else {
				writer->Write(m_valueType->SqlType);
			}

			writer->Write(m_isCompressed);
			writer->Write(m_isSorted);

			writer->Write(m_sqlList->Count);

			if (m_sqlList->Count) {
				IO::Compression::GZipStream^ gzip = nullptr;
				if (m_isCompressed) {
					gzip = gcnew IO::Compression::GZipStream(writer->BaseStream, IO::Compression::CompressionMode::Compress, true);
					writer = gcnew IO::BinaryWriter(gzip);
				}

				for each(Object^ value in m_sqlList) {
					if (!IsVariant.Value) {
						m_valueType->Write(value, writer);
					} else {
						Variant^ v = safe_cast<Variant^>(value);
						v->Write(writer);
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
