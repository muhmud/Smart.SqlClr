
#include <Smart/SqlClr/Types/Enumerator.hpp>

#include <Smart/SqlClr/Internal/SqlObject.hpp>

#using <Smart.SqlClr.dll> as_friend
#using <System.Xml.dll>

namespace Smart { namespace SqlClr { namespace Types {
	using namespace System;
	using namespace System::Data::SqlTypes;
	using namespace Microsoft::SqlServer::Server;

	Enumerator^ Enumerator::New(SqlEnumerable^ sqlEnumerable, Internal::SqlObject^ itemType, bool isSorted, bool isCompressed) {
		Enumerator^ enumerator = gcnew Enumerator;
		enumerator->m_sqlEnumerable = sqlEnumerable;
		enumerator->m_sqlEnumerator = enumerator->m_sqlEnumerable->GetEnumerator();
		enumerator->m_itemType = itemType;
		enumerator->m_isSorted = isSorted ? 1 : 0;
		enumerator->m_isCompressed = isCompressed ? 1 : 0;
		enumerator->m_hasNext = 0;
		enumerator->m_position = 0;
		
		if (enumerator->Count.Value > 0) {
			enumerator->m_sqlEnumerator->MoveNext();
			++enumerator->m_position;
		}

		return enumerator;
	}

	Internal::SqlObject^ Enumerator::getItemType() {
		return m_itemType;
	}

	Enumerator::SqlEnumerator^ Enumerator::GetSqlEnumerator() {
		return m_sqlEnumerable->GetEnumerator();
	}

	Enumerator::SqlEnumerable^ Enumerator::GetSqlEnumerable() {
		return m_sqlEnumerable;
	}

	Enumerator^ Enumerator::Copy(bool includeItems) {
		if (!IsNull) {
			Enumerator^ o = gcnew Enumerator;
			o->m_isCompressed = m_isCompressed;
			o->m_itemType = Internal::SqlObject::Create(m_itemType->SqlType);

			if (includeItems) {
				o->m_sqlEnumerable = m_sqlEnumerable;
				o->m_sqlEnumerator = m_sqlEnumerable->GetEnumerator();
				o->m_isSorted = m_isSorted;
				o->m_hasNext = m_hasNext;

				while (o->m_position++ != m_position) {
					o->m_sqlEnumerator->MoveNext();
				}
			} else {
				o->m_sqlEnumerable = gcnew SqlEnumerableImpl;
			}

			return o;
		}

		return Null;
	}

	Enumerator^ Enumerator::create() {
		Enumerator^ o = gcnew Enumerator;
		o->m_isNull = 1;

		return o;
	}

	void Enumerator::readHeader(IO::BinaryReader^ reader) {
		String^ valueType = reader->ReadString();
		m_itemType = !String::IsNullOrEmpty(valueType) ? Internal::SqlObject::Create(valueType) : nullptr;

		m_isCompressed = reader->ReadByte();
		m_isSorted = reader->ReadByte();
		m_hasNext = reader->ReadByte();
		m_position = reader->ReadInt32();
	}

	void Enumerator::readBody(IO::BinaryReader^ reader) {
		int count = reader->ReadInt32();
		if (count) {
			SqlEnumerableImpl^ sqlEnumerableImpl = gcnew SqlEnumerableImpl(count);
			
			if (m_isCompressed) {
				reader = gcnew IO::BinaryReader(gcnew IO::Compression::GZipStream(reader->BaseStream, IO::Compression::CompressionMode::Decompress, true));
			}

			for (int i = 0; i < count; ++i) {
				//if (!IsNested.Value) {
					sqlEnumerableImpl->Add(m_itemType->Read(reader));
				//} else {
				//	List^ list = gcnew List;
				//	list->readHeader(reader);
				//	list->SetCompression(false);
				//	list->readBody(reader);

				//	m_sqlList->Add(list);
				//}
			}

			m_sqlEnumerable = sqlEnumerableImpl;
		} else {
			m_sqlEnumerable = gcnew SqlEnumerableImpl;
		}

		m_sqlEnumerator = m_sqlEnumerable->GetEnumerator();

		if (m_position) {
			for (int i = m_position; i > 0; --i) {
				if (!m_sqlEnumerator->MoveNext()) {
					throw gcnew SmartSqlClrException("Invalid Enumerator. Moving to required position was not possible");
				}
			}
		}
	}

	void Enumerator::MoveNext() {
		if (!IsNull) {
			if (m_sqlEnumerator->MoveNext()) {
				m_hasNext = 1;
				++m_position;
			} else {
				m_hasNext = 0;
			}
		}
	}

	void Enumerator::Reset() {
		if (!IsNull) {
			m_sqlEnumerator->Reset();
			m_hasNext = 0;
			m_position = 0;

			MoveNext();
		}
	}

	Enumerator^ Enumerator::Clone() {
		return Copy(true);
	}

	void Enumerator::SetCompression(SqlBoolean compressed) {
		if (!IsNull) {
			m_isCompressed = compressed.Value ? 1 : 0;
		}
	}

	Object^ Enumerator::CurrentItem::get() {
		return m_sqlEnumerator->Current;
	}

	SqlString Enumerator::ValueType::get() {
		return IsNull ? SqlString::Null : m_itemType->SqlType;
	}

	bool Enumerator::IsNull::get() {
		return m_isNull == 1;
	}

	SqlBoolean Enumerator::IsSorted::get() {
		return IsNull ? SqlBoolean::Null : (m_isSorted == 1);
	}

	SqlBoolean Enumerator::HasNext::get() {
		return IsNull ? SqlBoolean::Null : m_hasNext == 1;
	}

	SqlInt32 Enumerator::Count::get() {
		return IsNull ? SqlInt32::Null : safe_cast<Collections::ICollection^>(m_sqlEnumerable)->Count;
	}

	SqlBoolean Enumerator::IsCompressed::get() {
		return IsNull ? SqlBoolean::Null : (m_isCompressed == 1);
	}

	Enumerator^ Enumerator::Null::get() {
		return NullEnumerator;
	}

	String^ Enumerator::ToString() {
		if (!IsNull) {
			Text::StringBuilder^ builder = gcnew Text::StringBuilder;

			Xml::XmlWriterSettings^ settings = gcnew Xml::XmlWriterSettings;
			settings->OmitXmlDeclaration = true;

			Xml::XmlWriter^ writer = Xml::XmlWriter::Create(builder, settings);

			writer->WriteStartElement(XML_ROOT_ELEMENT);
			if (m_itemType) {
				writer->WriteAttributeString(XML_VALUETYPE_ATTRIBUTE, m_itemType->SqlType);
			}

			writer->WriteAttributeString(XML_ISCOMPRESSED_ATTRIBUTE, IsCompressed.ToString());
			writer->WriteAttributeString(XML_ISSORTED_ATTRIBUTE, IsSorted.ToString());
			
			for each(Object^ value in m_sqlEnumerable) {
				writer->WriteStartElement(XML_ITEM_ELEMENT);

				//if (!IsNested.Value) {
					writer->WriteString(m_itemType->ConvertToString(value));
				//} else {
				//	writer->WriteRaw(value->ToString());
				//}

				writer->WriteEndElement();
			}

			writer->WriteEndElement();
			writer->Flush();

			return builder->ToString();
		}

		return nullptr;
	}

	String^ Enumerator::ToSimpleString() {
		if (!IsNull) {
			Text::StringBuilder^ builder = gcnew Text::StringBuilder("[ ");
			bool start = true;

			for each(Object^ value in m_sqlEnumerable) {
				builder->Append(!start ? ", " : String::Empty);
				//if (!IsNested.Value) {
					builder->Append(m_itemType->ConvertToString(value, false));
				//} else {
				//	builder->Append(safe_cast<List^>(value)->ToSimpleString());
				//}

				start = false;
			}

			builder->Append(" ]");
			return builder->ToString();
		}

		return nullptr;
	}

	Enumerator^ Enumerator::Parse(SqlString sqlValue) {
		String^ value = sqlValue.Value;
		Xml::XmlReader^ reader = Xml::XmlReader::Create(gcnew IO::StringReader(value));

		if (reader->IsStartElement(XML_ROOT_ELEMENT)) {
			String^ valueTypeString = reader->GetAttribute(XML_VALUETYPE_ATTRIBUTE);
			Internal::SqlObject^ valueType = Internal::SqlObject::Create(valueTypeString);
			
			String^ compressedString = reader->GetAttribute(XML_ISCOMPRESSED_ATTRIBUTE);
			bool compressed = compressedString ? Boolean::Parse(compressedString) : false;

			String^ sortedString = reader->GetAttribute(XML_ISSORTED_ATTRIBUTE);
			bool sorted = sortedString ? Boolean::Parse(sortedString) : false;

			SqlEnumerableImpl^ col = gcnew SqlEnumerableImpl;
			if (reader->ReadToFollowing(XML_ITEM_ELEMENT)) {
				do {
					//if (!list->IsNested.Value) {
						col->Add(valueType->Parse(reader->ReadString()));
					//} else {
					//	list->AddNested(List::Parse(reader->ReadInnerXml()));
					//}
				} while (reader->ReadToNextSibling(XML_ITEM_ELEMENT));
			}

			return New(col, valueType, sorted, compressed);
		}

		return Null;
	}

	void Enumerator::Read(IO::BinaryReader^ reader) {
		m_isNull = reader->ReadByte();
		if (!m_isNull) {
			readHeader(reader);
			readBody(reader);
		}
	}

	void Enumerator::Write(IO::BinaryWriter^ writer) {
		writer->Write(m_isNull);
		if (!m_isNull) {
			writer->Write(m_itemType->SqlType);
			writer->Write(m_isCompressed);
			writer->Write(m_isSorted);
			writer->Write(m_hasNext);
			writer->Write(m_position);

			int count = 0;
			try {
				writer->Write(count = Count.Value);
			} catch (Exception^) {
				throw gcnew SmartSqlClrException("An exception occurred while trying to retrieve number of items in the enumerable");
			}

			if (count) {
				IO::Compression::GZipStream^ gzip = nullptr;
				if (m_isCompressed) {
					gzip = gcnew IO::Compression::GZipStream(writer->BaseStream, IO::Compression::CompressionMode::Compress, true);
					writer = gcnew IO::BinaryWriter(gzip);
				}

				for each(Object^ value in m_sqlEnumerable) {
					//if (!IsNested.Value) {
						m_itemType->Write(value, writer);
					//} else {
					//	List^ a = safe_cast<List^>(value);
					//	a->SetCompression(false);
					//	a->Write(writer);
					//}
				}
				if (m_isCompressed) {
					writer->Flush();
					gzip->Close();
				}
			}
		}
	}
}}}
