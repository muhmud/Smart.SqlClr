
#include <Smart/SqlClr/Types/Tuple.hpp>

#include <Smart/SqlClr/Internal/SqlObject.hpp>

#using <Smart.SqlClr.dll> as_friend

namespace Smart { namespace SqlClr { namespace Types {
	using namespace System;
	using namespace System::Data::SqlTypes;
	using namespace Microsoft::SqlServer::Server;

	Tuple^ Tuple::Create(SqlTupleNames^ names, SqlTupleItems^ items, SqlTupleTypes^ types, IO::BinaryReader^ reader) {
		Tuple^ tuple = Create(names, items, types);
		tuple->readBody(reader, SqlInt32::Null);

		return tuple;
	}
	
	Tuple^ Tuple::Create(SqlTupleNames^ names, SqlTupleItems^ items, SqlTupleTypes^ types, Xml::XmlReader^ reader) {
		Tuple^ tuple = Create(names, items, types);
		tuple->ReadXml(reader, false);

		return tuple;
	}

	Tuple^ Tuple::Create(SqlTupleNames^ names, SqlTupleItems^ items, SqlTupleTypes^ types) {
		Tuple^ tuple = gcnew Tuple;
		tuple->m_names = names;
		tuple->m_items = items;
		tuple->m_types = types;

		return tuple;
	}

	Tuple^ Tuple::Create() {
		Tuple^ tuple = gcnew Tuple;
		tuple->m_names = gcnew SqlTupleNamesImpl;
		tuple->m_items = gcnew SqlTupleItemsImpl;
		tuple->m_types = gcnew SqlTupleTypesImpl;
		tuple->m_values = gcnew SqlTupleValuesImpl;

		return tuple;
	}

	Tuple^ Tuple::Copy() {
		Tuple^ tuple = gcnew Tuple;
		tuple->m_isNull = m_isNull;
		if (!m_isNull) {
			tuple->m_names = m_names;
			tuple->m_items = m_items;
			tuple->m_types = m_types;
			
			for each(Object^ value in m_values) {
				tuple->m_values->Add(value);
			}
		}

		return tuple;
	}

	void Tuple::ReadXml(Xml::XmlReader^ reader, bool includeMetadata) {
		reader->ReadStartElement(XML_ROOT_ELEMENT);
		reader->MoveToContent();
		int counter = 0;
		do {
			Internal::SqlObject^ type = nullptr;
			if (!includeMetadata) {
				type = m_types[counter++];
			} else {
				m_names->Add(reader->Name);
				m_items->Add(reader->Name, counter++);

				String^ typeString = reader->GetAttribute(XML_TYPE_ATTRIBUTE);
				type = Internal::SqlObject::Create(typeString ? typeString : "string");
				m_types->Add(type);
			}

			String^ isNullString = reader->GetAttribute(XML_ISNULL_ATTRIBUTE);
			bool isNullValue = isNullString ? Convert::ToBoolean(isNullString) : false;

			if (!isNullValue) {
				m_values->Add(type->Parse(reader->ReadString()));
			} else {
				setNull(counter, isNullValue);
			}

			reader->ReadEndElement();
		} while (reader->IsStartElement());

		reader->ReadEndElement();
	}

	void Tuple::WriteXml(Xml::XmlWriter^ writer, bool includeMetadata) {
		writer->WriteStartElement(XML_ROOT_ELEMENT);

		for (int i = 0; i < m_names->Count; ++i) {
			String^ item = m_names[i];
			writer->WriteStartElement(item);

			Internal::SqlObject^ type = m_types[i];
			if (includeMetadata) {
				writer->WriteAttributeString(XML_TYPE_ATTRIBUTE, type->SqlType);
			}

			if (isNull(i)) {
				writer->WriteAttributeString(XML_ISNULL_ATTRIBUTE, "true");
			} else {
				writer->WriteString(type->ConvertToString(m_values[i]));
			}
			
			writer->WriteEndElement();
		}

		writer->WriteEndElement();
	}

	Tuple::SqlTupleItems^ Tuple::GetItems() {
		return m_items;
	}

	Tuple::SqlTupleTypes^ Tuple::GetTypes() {
		return m_types;
	}

	Tuple^ Tuple::create() {
		Tuple^ tuple = gcnew Tuple;
		tuple->m_isNull = 1;

		return tuple;
	}

	void Tuple::checkIndex(SqlInt32 index) {
		if (index.IsNull) {
			throw gcnew SmartSqlClrException(String::Format("Invalid index. Index cannot be null"));
		}

		if (index.Value < 0 || index.Value > m_values->Count) {
			throw gcnew SmartSqlClrException(String::Format("Invalid index '{0}'", index.Value));
		}
	}

	void Tuple::checkName(SqlString name) {
		if (name.IsNull) {
			throw gcnew SmartSqlClrException(String::Format("Invalid name. Name cannot be null"));
		}

		if (!m_items->ContainsKey(name.Value)) {
			throw gcnew SmartSqlClrException(String::Format("Invalid item name '{0}'", name.Value));
		}
	}

	void Tuple::checkType(int index, Object^ value) {
		if (!m_types[index]->IsValueValid(value)) {
			throw gcnew SmartSqlClrException(String::Format("Invalid Type. The type of item '{0}' is '{1}'", index, m_types[index]->SqlType));
		}
	}

	bool Tuple::isNull(int index) {
		return (m_nulls & (safe_cast<Int64>(1) << index)) != 0;
	}

	void Tuple::setNull(int index, bool yes) {
		m_nulls = yes ? m_nulls | (safe_cast<Int64>(1) << index) : m_nulls ^ (safe_cast<Int64>(1) << index);
	}

	void Tuple::readHeader(IO::BinaryReader^ reader) {
		m_items = gcnew SqlTupleItemsImpl;
		m_types = gcnew SqlTupleTypesImpl;

		int itemCount = reader->ReadInt32();
		if (itemCount) {
			for (int i = 0; i < itemCount; ++i) {
				String^ name = reader->ReadString();
				m_names->Add(name);
				m_items->Add(name, i);
				m_types->Add(Internal::SqlObject::Create(reader->ReadString()));
			}
		}
	}

	void Tuple::readBody(IO::BinaryReader^ reader, SqlInt32 column) {
		if (column.IsNull || column.Value == 0) {
			m_values = gcnew SqlTupleValuesImpl;
			m_nulls = 0;

			if (reader->ReadByte()) {
				m_nulls = reader->ReadInt64();
			}
		}

		if (column.IsNull) {
			for (int i = 0; i < m_items->Count; ++i) {
				m_values->Add(m_types[i]->Read(reader));
			}
		} else {
			m_values->Add(m_types[column.Value]->Read(reader));
		}
	}

	void Tuple::writeHeader(IO::BinaryWriter^ writer) {
		writer->Write(m_items->Count);

		for (int i = 0; i < m_names->Count; ++i) {
			String^ item = m_names[i];
			writer->Write(item);
			writer->Write(m_types[i]->SqlType);
		}
	}

	void Tuple::writeBody(IO::BinaryWriter^ writer, SqlInt32 column) {
		if (column.IsNull || column.Value == 0) {
			if (!m_nulls) {
				writer->Write(safe_cast<Byte>(0));
			} else {
				writer->Write(safe_cast<Byte>(1));
				writer->Write(m_nulls);
			}
		}

		if (column.IsNull) {
			for (int i = 0; i < m_items->Count; ++i) {
				m_types[i]->Write(m_values[i], writer);
			}
		} else {
			m_types[column.Value]->Write(m_values[column.Value], writer);
		}
	}

	Object^ Tuple::Get(SqlInt32 index) {
		if (!IsNull) {
			checkIndex(index);

			// Check for a null
			if (!isNull(index.Value)) {
				return m_values[index.Value];
			}
		}

		return nullptr;
	}

	Object^ Tuple::GetByName(SqlString name) {
		if (!IsNull) {
			checkName(name);

			int index = m_items[name.Value];
			if (!isNull(index)) {
				return m_values[index];
			}
		}

		return nullptr;
	}

	void Tuple::SetByID(SqlInt32 index, Object^ value) {
		if (!IsNull) {
			checkIndex(index);
			checkType(index.Value, value);

			if (value) {
				setNull(index.Value, false);
				m_values[index.Value] = value;
			} else {
				setNull(index.Value, true);
			}
		}
	}

	void Tuple::SetByName(SqlString name, Object^ value) {
		if (!IsNull) {
			checkName(name);

			int index = m_items[name.Value];
			checkType(index, value);

			if (value) {
				setNull(index, false);
				m_values[index] = value;
			} else {
				setNull(index, true);
			}
		}
	}

	bool Tuple::IsNull::get() {
		return m_isNull == 1;
	}

	Tuple^ Tuple::Null::get() {
		return NullTuple;
	}

	String^ Tuple::ToString() {
		if (!IsNull) {
			Text::StringBuilder^ builder = gcnew Text::StringBuilder;

			Xml::XmlWriterSettings^ settings = gcnew Xml::XmlWriterSettings;
			settings->OmitXmlDeclaration = true;

			Xml::XmlWriter^ writer = Xml::XmlWriter::Create(builder, settings);
			WriteXml(writer, true);
			writer->Flush();

			return builder->ToString();
		}

		return nullptr;
	}

	String^ Tuple::ToSimpleString() {
		if (!IsNull) {
			Text::StringBuilder^ builder = gcnew Text::StringBuilder("( ");
			bool start = true;

			for (int i = 0; i < m_names->Count; ++i) {
				String^ name = m_names[i];
				builder->Append(!start ? ", " : String::Empty);
				if (!isNull(i)) {
					builder->Append(m_types[i]->ConvertToString(m_values[i]));
				}
				start = false;
			}

			builder->Append(" )");
			return builder->ToString();
		}

		return nullptr;
	}

	Tuple^ Tuple::Parse(SqlString sqlValue) {
		String^ value = sqlValue.Value;
		Xml::XmlReader^ reader = Xml::XmlReader::Create(gcnew IO::StringReader(value));

		reader->Read();
		Tuple^ tuple = Create();
		tuple->ReadXml(reader, true);

		return tuple;
	}

	void Tuple::Read(IO::BinaryReader^ reader) {
		m_isNull = reader->ReadByte();
		if (!m_isNull) {
			readHeader(reader);
			readBody(reader, SqlInt32::Null);
		}
	}

	void Tuple::Write(IO::BinaryWriter^ writer) {
		writer->Write(m_isNull);
		if (!m_isNull) {
			writeHeader(writer);
			writeBody(writer, SqlInt32::Null);
		}
	}
}}}
