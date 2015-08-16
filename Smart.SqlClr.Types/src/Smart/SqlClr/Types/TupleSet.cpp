
#include <Smart/SqlClr/Types/TupleSet.hpp>

#include <Smart/SqlClr/Internal/SqlObject.hpp>
#include <Smart/SqlClr/Types/Enumerator.hpp>
#include <Smart/SqlClr/Types/List.hpp>

#using <Smart.SqlClr.dll> as_friend
#using <System.Xml.dll>

namespace Smart { namespace SqlClr { namespace Types {
	TupleSet^ TupleSet::Copy(bool includeTuples) {
		TupleSet^ ts = gcnew TupleSet;
		ts->m_isNull = m_isNull;
		ts->m_indexes = m_indexes;
		ts->m_isCompressed = m_isCompressed;
		ts->m_items = m_items;
		ts->m_types = m_types;
		ts->m_uniqueIndexes = ts->m_uniqueIndexes;

		ts->m_tuples = gcnew SqlTuplesImpl;
		if (includeTuples) {
			for each(Tuple^ tuple in m_tuples) {
				ts->m_tuples->Add(tuple->Copy());
			}
		}

		return ts;
	}

	TupleSet^ TupleSet::create() {
		TupleSet^ tupleSet = gcnew TupleSet;
		tupleSet->m_isNull = 1;

		return tupleSet;
	}

	void TupleSet::checkIndex(SqlInt32 position) {
		if (position.IsNull) {
			throw gcnew SmartSqlClrException("Invalid index. Value cannot be null");
		}

		if (position.Value < 0 || position.Value >= m_tuples->Count) {
			throw gcnew SmartSqlClrException(String::Format("Invalid index '{0}'. Index is outside the bounds of the collection.", position.Value));
		}
	}

	void TupleSet::checkKeyValue(SqlString name, Object^ value) {
		if (name.IsNull) {
			throw gcnew SmartSqlClrException("Invalid unique index name. Name cannot be null");
		}

		if (!value) {
			throw gcnew SmartSqlClrException("Invalid value. Value cannot be null");
		}

		if (!m_uniqueIndexes->ContainsKey(name.Value)) {
			throw gcnew SmartSqlClrException("Invalid unique index. The specified column does not have an unique index");
		}

		checkValue(name.Value, value);
	}

	void TupleSet::checkIndexValue(SqlString name, Object^ value) {
		if (name.IsNull) {
			throw gcnew SmartSqlClrException("Invalid index name. Name cannot be null");
		}

		if (!value) {
			throw gcnew SmartSqlClrException("Invalid value. Value cannot be null");
		}

		if (!m_indexes->ContainsKey(name.Value)) {
			throw gcnew SmartSqlClrException("Invalid index. The specified column does not have an index");
		}

		checkValue(name.Value, value);
	}

	void TupleSet::checkValue(String^ name, Object^ value) {
		int column = m_items[name];
		Internal::SqlObject^ columnType = m_types[column];
		if (!columnType->IsValueValid(value)) {
			throw gcnew SmartSqlClrException(String::Format("Invalid value. Value does not have the right type for column '{0}'. Expected '{1}'", name, columnType->SqlType));
		}
	}

	void TupleSet::checkTuple(Tuple^ tuple) {
		if (!tuple || tuple->IsNull) {
			throw gcnew SmartSqlClrException("Tuple to operate on cannot be null");
		}

		if (m_items->Count != tuple->GetItems()->Count) {
			throw gcnew SmartSqlClrException("Tuple has incompatible columns");
		}

		Collections::Generic::IEnumerator<Collections::Generic::KeyValuePair<String^, int>>^ tupleItems = tuple->GetItems()->GetEnumerator();
		tupleItems->MoveNext();
		for each(Tuple::SqlTupleItemEntry itemEntry in m_items) {
			if (itemEntry.Key != tupleItems->Current.Key || itemEntry.Value != tupleItems->Current.Value) {
				throw gcnew SmartSqlClrException("Tuple has incompatible columns");
			}
		}

		if (m_types->Count != tuple->GetTypes()->Count) {
			throw gcnew SmartSqlClrException("Tuple has incompatible types");
		}

		for (int i = 0; i < m_types->Count; ++i) {
			if (m_types[i]->SqlType != tuple->GetTypes()[i]->SqlType) {
				throw gcnew SmartSqlClrException("Tuple has incompatible types");
			}
		}

		// Check for unique key violations
		if (m_uniqueIndexes->Count) {
			for each (SqlTupleUniqueIndexesEntry entry in m_uniqueIndexes) {
				Object^ val = tuple->GetByName(entry.Key);
				if (val) {
					if (entry.Value->ContainsKey(val)) {
						throw gcnew SmartSqlClrException(String::Format("The Column value for '{0}' has violated a unique index in the set. The related value is '{1}'", entry.Key, val));
					}
				}
			}
		}
	}

	void TupleSet::readHeader(IO::BinaryReader^ reader) {
		m_items = gcnew Tuple::SqlTupleItemsImpl;
		m_types = gcnew Tuple::SqlTupleTypesImpl;

		int itemCount = reader->ReadInt32();
		for (int i = 0; i < itemCount; ++i) {
			m_items->Add(reader->ReadString(), i);
			m_types->Add(Internal::SqlObject::Create(reader->ReadString()));
		}
	}

	void TupleSet::readBody(IO::BinaryReader^ reader) {
		int count = reader->ReadInt32();
		if (count) {
			m_tuples = gcnew SqlTuplesImpl(count);
			for (int i = 0; i < count; ++i) {
				m_tuples->Add(Tuple::Create(m_names, m_items, m_types, reader));
			}
		} else {
			m_tuples = gcnew SqlTuplesImpl;
		}
	}
	
	void TupleSet::removeTuples(cli::array<int>^ positions) {
		for each(int position in positions) {
			removeTuple(position);
		}
	}

	void TupleSet::removeTuple(int position) {
		// Get the tuple
		Tuple^ tuple = m_tuples[position];

		// Remove the tuple
		m_tuples->RemoveAt(position);

		// Update unique indexes
		if (m_uniqueIndexes->Count) {
			for each (SqlTupleUniqueIndexesEntry entry in m_uniqueIndexes) {
				Object^ val = tuple->GetByName(entry.Key);
				if (val) {
					entry.Value->Remove(val);
				}
			}
		}

		// Update indexes
		if (m_indexes->Count) {
			for each (SqlTupleIndexesEntry entry in m_indexes) {
				Object^ val = tuple->GetByName(entry.Key);
				if (val) {
					SqlTupleIndexEntries^ entries = entry.Value[val];
					entries->Remove(position);
				}
			}
		}
	}

	Tuple^ TupleSet::Get(SqlInt32 position) {
		if (!IsNull) {
			checkIndex(position);

			return m_tuples[position.Value];
		}

		return Tuple::Null;
	}

	Tuple^ TupleSet::GetByKey(SqlString keyName, Object^ keyValue) {
		if (!IsNull) {
			checkKeyValue(keyName, keyValue);

			return m_tuples[m_uniqueIndexes[keyName.Value][keyValue]];
		}

		return Tuple::Null;
	}

	[SqlMethod(IsDeterministic=true, OnNullCall=false, IsPrecise=true, DataAccess=DataAccessKind::None)]
	TupleSet^ TupleSet::GetByIndex(SqlString indexName, Object^ indexValue) {
		if (!IsNull) {
			checkIndexValue(indexName, indexValue);

			TupleSet^ ts = Copy(false);
			for each(int pos in m_indexes[indexName.Value][indexValue]) {
				ts->AddTuple(m_tuples[pos]->Copy());
			}

			return ts;
		}

		return Null;
	}

	void TupleSet::AddTuple(Tuple^ tuple) {
		if (!IsNull) {
			checkTuple(tuple);

			m_tuples->Add(tuple);

			// Update unique indexes
			if (m_uniqueIndexes->Count) {
				for each (SqlTupleUniqueIndexesEntry entry in m_uniqueIndexes) {
					Object^ val = tuple->GetByName(entry.Key);
					if (val) {
						entry.Value->Add(val, m_tuples->Count - 1);
					}
				}
			}

			// Update indexes
			if (m_indexes->Count) {
				for each (SqlTupleIndexesEntry entry in m_indexes) {
					Object^ val = tuple->GetByName(entry.Key);
					if (val) {
						if (!entry.Value->ContainsKey(val)) {	
							entry.Value->Add(val, gcnew SqlTupleIndexEntriesImpl);
						}

						entry.Value[val]->Add(m_tuples->Count - 1);
					}
				}
			}
		}
	}

	void TupleSet::Remove(SqlInt32 position) {
		if (!IsNull) {
			checkIndex(position);

			removeTuple(position.Value);
		}
	}

	void TupleSet::RemoveByKey(SqlString keyName, Object^ keyValue) {
		if (!IsNull) {
			checkKeyValue(keyName, keyValue);
			
			SqlTupleUniqueIndex^ ux = m_uniqueIndexes[keyName.Value];
			if (ux->ContainsKey(keyValue)) {
				removeTuple(ux[keyValue]);
			}
		}
	}

	void TupleSet::RemoveByIndex(SqlString indexName, Object^ indexValue) {
		if (!IsNull) {
			checkIndexValue(indexName, indexValue);

			SqlTupleIndex^ ix = m_indexes[indexName.Value];
			if (ix->ContainsKey(indexValue)) {
				removeTuples(safe_cast<SqlTupleIndexEntriesImpl^>(ix[indexValue])->ToArray());
			}
		}
	}

	void TupleSet::SetCompression(SqlBoolean compressed) {
		if (!IsNull) {
			if (!compressed.IsNull) {
				m_isCompressed = compressed.Value ? 1 : 0;
			}
		}
	}

	void TupleSet::SetColumnarCompression(SqlBoolean compressed) {
		if (!IsNull) {
			if (!compressed.IsNull) {
				if (compressed.Value) {
					m_isCompressed = 1;
					m_isColumnarCompressed = 1;
				} else {
					m_isColumnarCompressed = 0;
				}
			}
		}
	}

	SqlBoolean TupleSet::IsCompressed::get() {
		return !IsNull ? m_isCompressed == 1 : SqlBoolean::Null;
	}

	SqlBoolean TupleSet::IsColumnarCompressed::get() {
		return !IsNull ? m_isColumnarCompressed == 1 : SqlBoolean::Null;
	}

	SqlInt32 TupleSet::Count::get() {
		return !IsNull ? m_tuples->Count : SqlInt32::Null;
	}

	Enumerator^ TupleSet::Tuples::get() {
		return !IsNull ? Enumerator::New(m_tuples, nullptr, 0, m_isCompressed == 1) : Enumerator::Null;
	}

	List^ TupleSet::Columns::get() {
		List^ columns = List::NewOfType(SqlString::typeid);
		for each(String^ column in m_items->Keys) {
			columns->getSqlList()->Add(SqlString(column));
		}

		return columns;
	}

	List^ TupleSet::Types::get() {
		List^ types = List::NewOfType(SqlString::typeid);
		for each(Internal::SqlObject^ type in m_types) {
			types->getSqlList()->Add(type->SqlType);
		}

		return types;
	}

	bool TupleSet::IsNull::get() {
		return m_isNull == 1;
	}

	TupleSet^ TupleSet::Null::get() {
		return NullTupleSet;
	}

	String^ TupleSet::ToString() {
		if (!IsNull) {
			Text::StringBuilder^ builder = gcnew Text::StringBuilder;

			Xml::XmlWriterSettings^ settings = gcnew Xml::XmlWriterSettings;
			settings->OmitXmlDeclaration = true;

			Xml::XmlWriter^ writer = Xml::XmlWriter::Create(builder, settings);
			writer->WriteStartElement(XML_ROOT_ELEMENT);
			writer->WriteAttributeString(XML_ISCOMPRESSED_ATTRIBUTE, IsCompressed.ToString());
			
			writer->WriteStartElement(XML_COLUMNS_ELEMENT);
			for each(Tuple::SqlTupleItemEntry itemEntry in m_items) {
				String^ name = itemEntry.Key;
				int position = itemEntry.Value;

				writer->WriteStartElement(XML_COLUMN_ELEMENT);
				writer->WriteAttributeString(XML_COLUMN_NAME_ATTRIBUTE, name);
				writer->WriteAttributeString(XML_COLUMN_TYPE_ATTRIBUTE, m_types[position]->SqlType);
				if (m_uniqueIndexes->ContainsKey(name)) {
					writer->WriteAttributeString(XML_COLUMN_KEY_ATTRIBUTE, "true");
				} else if (m_indexes->ContainsKey(name)) {
					writer->WriteAttributeString(XML_COLUMN_INDEX_ATTRIBUTE, "true");
				}

				writer->WriteEndElement();
			}

			writer->WriteEndElement();

			for each(Tuple^ value in m_tuples) {
				value->WriteXml(writer, true);
			}

			writer->WriteEndElement();
			writer->Flush();

			return builder->ToString();
		}

		return nullptr;
	}

	TupleSet^ TupleSet::Parse(SqlString sqlValue) {
		String^ value = sqlValue.Value;
		Xml::XmlReader^ reader = Xml::XmlReader::Create(gcnew IO::StringReader(value));

		reader->Read();
		reader->ReadStartElement(XML_ROOT_ELEMENT);

		TupleSet^ tupleSet = gcnew TupleSet;
		tupleSet->m_items = gcnew Tuple::SqlTupleItemsImpl;
		tupleSet->m_types = gcnew Tuple::SqlTupleTypesImpl;
		tupleSet->m_tuples = gcnew SqlTuplesImpl;
		tupleSet->m_uniqueIndexes = gcnew SqlTupleUniqueIndexesImpl;
		tupleSet->m_indexes = gcnew SqlTupleIndexesImpl;

		String^ compressedString = reader->GetAttribute(XML_ISCOMPRESSED_ATTRIBUTE);
		bool compressed = compressedString ? Boolean::Parse(compressedString) : false;
		tupleSet->m_isCompressed = compressed ? 1 : 0;

		reader->ReadStartElement(XML_COLUMNS_ELEMENT);
		int counter = 0;
		if (reader->ReadToFollowing(XML_COLUMN_ELEMENT)) {
			do {
				String^ name = reader->GetAttribute(XML_COLUMN_NAME_ATTRIBUTE);
				String^ type = reader->GetAttribute(XML_COLUMN_TYPE_ATTRIBUTE);

				String^ isKeyString = reader->GetAttribute(XML_COLUMN_KEY_ATTRIBUTE);
				bool isKey = isKeyString ? Boolean::Parse(isKeyString) : false;

				String^ isIndexString = reader->GetAttribute(XML_COLUMN_INDEX_ATTRIBUTE);
				bool isIndex = isIndexString ? Boolean::Parse(isIndexString) : false;

				tupleSet->m_items->Add(name, counter++);
				tupleSet->m_types->Add(Internal::SqlObject::Create(type));
				if (isKey) {
					tupleSet->m_uniqueIndexes->Add(name, gcnew SqlTupleUniqueIndexImpl);
				} else if(isIndex) {
					tupleSet->m_indexes->Add(name, gcnew SqlTupleIndexImpl);
				}
			} while (reader->ReadToNextSibling(XML_COLUMN_ELEMENT));
		} else {
			throw gcnew SmartSqlClrException("Invalid Xml. Column metadata not found");
		}

		// reader->ReadEndElement(); ? 


		while(reader->IsStartElement("Tuple")) {
			tupleSet->m_tuples->Add(Tuple::Create(tupleSet->m_names, tupleSet->m_items, tupleSet->m_types, reader));
		}

		return tupleSet;
	}

	TupleSet^ TupleSet::ParseTuples(SqlString sqlValue) {
		String^ value = sqlValue.Value;
		Xml::XmlReader^ reader = Xml::XmlReader::Create(gcnew IO::StringReader(value));

		reader->Read();
		reader->ReadStartElement(XML_TUPLES_ROOT_ELEMENT);

		TupleSet^ tupleSet = gcnew TupleSet;
		tupleSet->m_items = gcnew Tuple::SqlTupleItemsImpl;
		tupleSet->m_types = gcnew Tuple::SqlTupleTypesImpl;
		tupleSet->m_tuples = gcnew SqlTuplesImpl;
		tupleSet->m_uniqueIndexes = gcnew SqlTupleUniqueIndexesImpl;
		tupleSet->m_indexes = gcnew SqlTupleIndexesImpl;

		while(reader->IsStartElement("Tuple")) {
			Tuple^ tuple = Tuple::Create();
			tuple->ReadXml(reader, true);
			tupleSet->m_tuples->Add(tuple);
		}

		if (tupleSet->m_tuples->Count) {
			Tuple^ tuple = tupleSet->m_tuples[0];
			tupleSet->m_items = tuple->GetItems();
			tupleSet->m_types = tuple->GetTypes();

			return tupleSet;
		}

		return Null;
	}

	void TupleSet::Read(IO::BinaryReader^ reader) {
		m_isNull = reader->ReadByte();
		if (!m_isNull) {
			m_isCompressed = reader->ReadByte();
			if (m_isCompressed) {
				m_isColumnarCompressed = reader->ReadByte();
				reader = gcnew IO::BinaryReader(gcnew IO::Compression::GZipStream(reader->BaseStream, IO::Compression::CompressionMode::Decompress, true));
			}

			m_items = gcnew Tuple::SqlTupleItemsImpl;
			m_types = gcnew Tuple::SqlTupleTypesImpl;
			m_tuples = gcnew SqlTuplesImpl;
			m_uniqueIndexes = gcnew SqlTupleUniqueIndexesImpl;
			m_indexes = gcnew SqlTupleIndexesImpl;

			int count = reader->ReadInt32();
			for (int i = 0; i < count; ++i) {
				String^ name = reader->ReadString();
				m_items->Add(name, i);
				m_types->Add(Internal::SqlObject::Create(reader->ReadString()));
				Byte attr = reader->ReadByte();
				if (attr == 1) {
					m_uniqueIndexes->Add(name, gcnew SqlTupleUniqueIndexImpl);
				} else if (attr == 2) {
					m_indexes->Add(name, gcnew SqlTupleIndexImpl);
				}
			}

			count = reader->ReadInt32();
			if (!m_isColumnarCompressed) {
				for (int i = 0; i < count; ++i) {
					m_tuples->Add(Tuple::Create(m_names, m_items, m_types, reader));
				}
			} else {
				for (int i = 0; i < m_items->Count; ++i) {
					for (int j = 0; j < count; ++j) {
						if (i != 0) {
							m_tuples[j]->readBody(reader, i);
						} else {
							Tuple^ tuple = Tuple::Create(m_names, m_items, m_types);
							tuple->readBody(reader, i);

							m_tuples->Add(tuple);
						}
					}
				}
			}
		}
	}

	void TupleSet::Write(IO::BinaryWriter^ writer) {
		writer->Write(m_isNull);
		if (!m_isNull) {
			writer->Write(m_isCompressed);
			if (m_isCompressed) {
				writer->Write(m_isColumnarCompressed);
			}

			IO::Compression::GZipStream^ gzip = nullptr;
			if (m_isCompressed) {
				gzip = gcnew IO::Compression::GZipStream(writer->BaseStream, IO::Compression::CompressionMode::Compress, true);
				writer = gcnew IO::BinaryWriter(gzip);
			}

			writer->Write(m_items->Count);
			for each (Tuple::SqlTupleItemEntry entry in m_items) {
				writer->Write(entry.Key);
				writer->Write(m_types[entry.Value]->SqlType);
				if (m_uniqueIndexes->ContainsKey(entry.Key)) {
					writer->Write(safe_cast<Byte>(1));
				} else if (m_indexes->ContainsKey(entry.Key)) {
					writer->Write(safe_cast<Byte>(2));
				} else {
					writer->Write(safe_cast<Byte>(0));
				}
			}

			writer->Write(m_tuples->Count);

			if (!m_isColumnarCompressed) {
				for each(Tuple^ tuple in m_tuples) {
					tuple->writeBody(writer, SqlInt32::Null);
				}
			} else {
				for (int i = 0; i < m_items->Count; ++i) {
					for each(Tuple^ tuple in m_tuples) {
						tuple->writeBody(writer, i);
					}
				}
			}

			if (m_isCompressed) {
				writer->Flush();
				gzip->Close();
			}
		}
	}
}}}
