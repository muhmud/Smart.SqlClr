
#include <Smart/SqlClr/Types/Variant.hpp>

#include <Smart/SqlClr/Internal/SqlObject.hpp>
#include <Smart/SqlClr/Types/Dictionary.hpp>
#include <Smart/SqlClr/Types/Enumerator.hpp>
#include <Smart/SqlClr/Types/List.hpp>

#using <System.Xml.dll>

namespace Smart { namespace SqlClr { namespace Types {
	using namespace System;
	using namespace System::Data::SqlTypes;
	using namespace Microsoft::SqlServer::Server;
	using namespace Microsoft::SqlServer::Types;

	Variant^ Variant::create() {
		Variant^ variant = gcnew Variant;
		variant->m_value = nullptr;

		return variant;
	}

	Variant^ Variant::CreateFromBigInt(SqlInt64 value) {
		Variant^ variant = gcnew Variant;
		variant->m_value = value;

		return variant;
	}

	Variant^ Variant::CreateFromBinary(SqlBinary value) {
		Variant^ variant = gcnew Variant;
		variant->m_value = value;

		return variant;
	}

	Variant^ Variant::CreateFromBit(SqlBoolean value) {
		Variant^ variant = gcnew Variant;
		variant->m_value = value;

		return variant;
	}

	Variant^ Variant::CreateFromVarchar(SqlString value) {
		Variant^ variant = gcnew Variant;
		variant->m_value = value;

		return variant;
	}

	Variant^ Variant::CreateFromNVarchar(SqlString value) {
		Variant^ variant = gcnew Variant;
		variant->m_value = value;

		return variant;
	}

	Variant^ Variant::CreateFromDate(DateTime value) {
		Variant^ variant = gcnew Variant;
		variant->m_value = value;

		return variant;
	}

	Variant^ Variant::CreateFromTime(DateTime value) {
		Variant^ variant = gcnew Variant;
		variant->m_value = value;

		return variant;
	}

	Variant^ Variant::CreateFromDateTime2(DateTime value) {
		Variant^ variant = gcnew Variant;
		variant->m_value = value;

		return variant;
	}

	Variant^ Variant::CreateFromDateTime(SqlDateTime value) {
		Variant^ variant = gcnew Variant;
		variant->m_value = value;

		return variant;
	}

	Variant^ Variant::CreateFromDateTimeOffset(DateTimeOffset value) {
		Variant^ variant = gcnew Variant;
		variant->m_value = value;

		return variant;
	}

	Variant^ Variant::CreateFromDecimal(SqlDecimal value) {
		Variant^ variant = gcnew Variant;
		variant->m_value = value;

		return variant;
	}

	Variant^ Variant::CreateFromFloat(SqlDouble value) {
		Variant^ variant = gcnew Variant;
		variant->m_value = value;

		return variant;
	}

	Variant^ Variant::CreateFromInt(SqlInt32 value) {
		Variant^ variant = gcnew Variant;
		variant->m_value = value;

		return variant;
	}

	Variant^ Variant::CreateFromMoney(SqlMoney value) {
		Variant^ variant = gcnew Variant;
		variant->m_value = value;

		return variant;
	}

	Variant^ Variant::CreateFromReal(SqlSingle value) {
		Variant^ variant = gcnew Variant;
		variant->m_value = value;

		return variant;
	}

	Variant^ Variant::CreateFromSmallInt(SqlInt16 value) {
		Variant^ variant = gcnew Variant;
		variant->m_value = value;

		return variant;
	}

	Variant^ Variant::CreateFromTinyInt(SqlByte value) {
		Variant^ variant = gcnew Variant;
		variant->m_value = value;

		return variant;
	}

	Variant^ Variant::CreateFromUniqueIdentifier(SqlGuid value) {
		Variant^ variant = gcnew Variant;
		variant->m_value = value;

		return variant;
	}

	Variant^ Variant::CreateFromXml(SqlXml^ value) {
		Variant^ variant = gcnew Variant;
		variant->m_value = value;

		return variant;
	}

	Variant^ Variant::CreateFromHierarchyID(SqlHierarchyId value) {
		Variant^ variant = gcnew Variant;
		variant->m_value = value;

		return variant;
	}

	Variant^ Variant::CreateFromGeography(SqlGeography^ value) {
		Variant^ variant = gcnew Variant;
		variant->m_value = value;

		return variant;
	}

	Variant^ Variant::CreateFromGeometry(SqlGeometry^ value) {
		Variant^ variant = gcnew Variant;
		variant->m_value = value;

		return variant;
	}

	Variant^ Variant::CreateFromDictionary(Dictionary^ value) {
		Variant^ variant = gcnew Variant;
		variant->m_value = value;

		return variant;
	}

	Variant^ Variant::CreateFromEnumerator(Enumerator^ value) {
		Variant^ variant = gcnew Variant;
		variant->m_value = value;

		return variant;
	}

	Variant^ Variant::CreateFromList(List^ value) {
		Variant^ variant = gcnew Variant;
		variant->m_value = value;

		return variant;
	}

	Variant^ Variant::CreateFrom(Object^ value) {
		Variant^ variant = gcnew Variant;
		variant->m_value = value;

		return variant;
	}

	SqlString Variant::Type::get() {
		return IsNull ? SqlString::Null : m_value->GetType()->Name;
	}

	SqlInt64 Variant::AsBigInt::get() {
		return safe_cast<SqlInt64>(m_value);
	}

	SqlBinary Variant::AsBinary::get() {
		return safe_cast<SqlBinary>(m_value);
	}

	SqlBoolean Variant::AsBoolean::get() {
		return safe_cast<SqlBoolean>(m_value);
	}

	SqlString Variant::AsNVarchar::get() {
		return safe_cast<SqlString>(m_value);
	}
	
	DateTime Variant::AsDateTime2::get() {
		return safe_cast<DateTime>(m_value);
	}

	SqlDateTime Variant::AsDateTime::get() {
		return safe_cast<SqlDateTime>(m_value);
	}
	
	DateTimeOffset Variant::AsDateTimeOffset::get() {
		return safe_cast<DateTimeOffset>(m_value);
	}

	SqlDecimal Variant::AsDecimal::get() {
		return safe_cast<SqlDecimal>(m_value);
	}
	
	SqlDouble Variant::AsFloat::get() {
		return safe_cast<SqlDouble>(m_value);
	}

	SqlInt32 Variant::AsInt::get() {
		return *safe_cast<SqlInt32^>(m_value);
	}
	
	SqlMoney Variant::AsMoney::get() {
		return safe_cast<SqlMoney>(m_value);
	}

	SqlSingle Variant::AsReal::get() {
		return safe_cast<SqlSingle>(m_value);
	}

	SqlInt16 Variant::AsSmallInt::get() {
		return safe_cast<SqlInt16>(m_value);
	}

	SqlByte Variant::AsTinyInt::get() {
		return safe_cast<SqlByte>(m_value);
	}

	SqlGuid Variant::AsUniqueIdentifier::get() {
		return safe_cast<SqlGuid>(m_value);
	}

	SqlXml^ Variant::AsXml::get() {
		return safe_cast<SqlXml^>(m_value);
	}

	SqlHierarchyId Variant::AsHierarchyID::get() {
		return safe_cast<SqlHierarchyId>(m_value);
	}

	SqlGeography^ Variant::AsGeography::get() {
		return safe_cast<SqlGeography^>(m_value);
	}

	SqlGeometry^ Variant::AsGeometry::get() {
		return safe_cast<SqlGeometry^>(m_value);
	}

	Dictionary^ Variant::AsDictionary::get() {
		return safe_cast<Dictionary^>(m_value);
	}

	Enumerator^ Variant::AsEnumerator::get() {
		return safe_cast<Enumerator^>(m_value);
	}

	List^ Variant::AsList::get() {
		return safe_cast<List^>(m_value);
	}

	Object^ Variant::AsSqlVariant::get() {
		return m_value;
	}

	bool Variant::IsNull::get() {
		return m_value == nullptr;
	}

	Variant^ Variant::Null::get() {
		return NullVariant;
	}

	String^ Variant::ToString() {
		if (!IsNull) {
			Text::StringBuilder^ builder = gcnew Text::StringBuilder;

			Xml::XmlWriterSettings^ settings = gcnew Xml::XmlWriterSettings;
			settings->OmitXmlDeclaration = true;

			Xml::XmlWriter^ writer = Xml::XmlWriter::Create(builder, settings);

			writer->WriteStartElement(XML_ROOT_ELEMENT);

			Internal::SqlObject^ type = Internal::SqlObject::Create(m_value->GetType());
			writer->WriteAttributeString(XML_TYPE_ATTRIBUTE, type->SqlType);
			writer->WriteStartElement(XML_VALUE_ELEMENT);
			if (m_value->GetType() != Dictionary::typeid && m_value->GetType() != Enumerator::typeid && m_value->GetType() != List::typeid) {
				writer->WriteString(type->ConvertToString(m_value));
			} else {
				writer->WriteRaw(m_value->ToString());
			}
			writer->WriteEndElement();

			writer->WriteEndElement();
			writer->Flush();

			return builder->ToString();
		} 

		return nullptr;
	}

	Variant^ Variant::Parse(SqlString sqlValue) {
		String^ value = sqlValue.Value;
		Xml::XmlReader^ reader = Xml::XmlReader::Create(gcnew IO::StringReader(value));

		if (reader->IsStartElement(XML_ROOT_ELEMENT)) {
			String^ typeString = reader->GetAttribute(XML_TYPE_ATTRIBUTE);
			Internal::SqlObject^ type = Internal::SqlObject::Create(typeString);

			Variant^ variant = gcnew Variant;
			if (reader->ReadToFollowing(XML_VALUE_ELEMENT)) {
				variant->m_value = type->Parse(reader->ReadString());
			}

			return variant;
		}
		
		return Null;
	}

	void Variant::Read(IO::BinaryReader^ reader) {
		if (!reader->ReadByte()) {
			Internal::SqlObject^ sqlObject = Internal::SqlObject::Create(reader->ReadString());
			m_value = sqlObject->Read(reader);
		} else {
			m_value = nullptr;
		}
	}

	void Variant::Write(IO::BinaryWriter^ writer) {
		if (!IsNull) {
			writer->Write(safe_cast<Byte>(0));

			Internal::SqlObject^ sqlObject = Internal::SqlObject::Create(m_value->GetType());
			writer->Write(sqlObject->SqlType);
			sqlObject->Write(m_value, writer);
		} else {
			writer->Write(safe_cast<Byte>(1));
		}
	}
}}}
