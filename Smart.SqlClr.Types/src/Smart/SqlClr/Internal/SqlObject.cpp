
#include <Smart/SqlClr/Internal/SqlObject.hpp>

#include <Smart/SqlClr/Types/Dictionary.hpp>
#include <Smart/SqlClr/Types/Enumerator.hpp>
#include <Smart/SqlClr/Types/List.hpp>

#using <Smart.SqlClr.dll> as_friend
#using <System.Xml.dll>
#using <Microsoft.SqlServer.Types.dll>

namespace Smart { namespace SqlClr { namespace Internal {
	using namespace System;
	using namespace System::Data;
	using namespace Microsoft::SqlServer::Types;

	SqlObject::SqlObject(Type^ sqlDataType, String^ sqlType, int precision, int scale, int length, bool isSqlVariantCompatible) : m_sqlDataType(sqlDataType), m_sqlType(sqlType), m_precision(precision), m_scale(scale), m_length(length), m_isSqlVariantCompatible(isSqlVariantCompatible) {
		m_valueProperty = getValueProperty(m_sqlDataType);
		m_isNullProperty = getIsNullProperty(m_sqlDataType);
		m_nullProperty = getNullProperty(m_sqlDataType);
		m_nullField = getNullField(m_sqlDataType);

		m_constructorMethod = getConstructorMethod(m_sqlDataType, m_valueProperty);
		m_parseMethod = getParseMethod(m_sqlDataType);

		m_binaryReaderMethod = getBinaryReaderMethod(m_sqlDataType);
		m_binaryWriterMethod = getBinaryWriterMethod(m_valueProperty);

		m_isNumeric = isSqlVariantCompatible && (sqlDataType == SqlTypes::SqlByte::typeid || 
			sqlDataType == SqlTypes::SqlDecimal::typeid || 
			sqlDataType == SqlTypes::SqlDouble::typeid || 
			sqlDataType == SqlTypes::SqlInt16::typeid || 
			sqlDataType == SqlTypes::SqlInt32::typeid || 
			sqlDataType == SqlTypes::SqlInt64::typeid || 
			sqlDataType == SqlTypes::SqlMoney::typeid || 
			sqlDataType == SqlTypes::SqlSingle::typeid);
	}

	Object^ SqlObject::construct(IO::BinaryReader^ reader) {
		Object^ val = nullptr;
		if (m_sqlDataType == SqlHierarchyId::typeid) {
			val = gcnew SqlHierarchyId;
		} else if (m_sqlDataType == SqlGeography::typeid) {
			val = gcnew SqlGeography;
		} else if (m_sqlDataType == SqlGeometry::typeid) {
			val = gcnew SqlGeometry;
		} else {
			val = getNoArgsConstructorMethod(m_sqlDataType)->Invoke(nullptr);
		}

		val->GetType()->GetMethod("Read")->Invoke(val, gcnew cli::array<Object^> { reader });
		return val;
	}
	
	Object^ SqlObject::construct(Object^ value) {
		return construct(value, m_sqlDataType, m_constructorMethod);
	}

	Object^ SqlObject::construct(Object^ value, Type^ sqlDataType) {
		return construct(value, sqlDataType, getConstructorMethod(sqlDataType));
	}

	Object^ SqlObject::construct(Object^ value, Type^ type, Reflection::ConstructorInfo^ constructorMethod) {
		if (value) {
			if (type == System::Data::SqlTypes::SqlString::typeid) {
				return value->ToString();
			}

			if (constructorMethod) {
				return constructorMethod->Invoke(gcnew cli::array<Object^> { value });
			}

			if (type == SqlTypes::SqlXml::typeid) {
				return toSqlXml(value->ToString());
			} 
			
			if (type == DateTime::typeid || type == DateTimeOffset::typeid) {
				return value;
			}

			return value->ToString();
		}

		return Null;
	}

	SqlObject::TypeDictionary^ SqlObject::getSqlVariantDataTypes() {
		TypeDictionary^ types = gcnew TypeDictionaryImpl;

		types->Add("bigint", SqlTypes::SqlInt64::typeid);
		types->Add("binary", SqlTypes::SqlBinary::typeid);
		types->Add("bit", SqlTypes::SqlBoolean::typeid);
		types->Add("char", SqlTypes::SqlString::typeid);
		types->Add("date", DateTime::typeid);
		types->Add("datetime", SqlTypes::SqlDateTime::typeid);
		types->Add("datetime2", DateTime::typeid);
		types->Add("time", DateTime::typeid);
		types->Add("datetimeoffset", DateTimeOffset::typeid);
		types->Add("decimal", SqlTypes::SqlDecimal::typeid);
		types->Add("float", SqlTypes::SqlDouble::typeid);
		types->Add("int", SqlTypes::SqlInt32::typeid);
		types->Add("money", SqlTypes::SqlMoney::typeid);
		types->Add("nchar", SqlTypes::SqlString::typeid);
		types->Add("nvarchar", SqlTypes::SqlString::typeid);
		types->Add("string", SqlTypes::SqlString::typeid);
		types->Add("real", SqlTypes::SqlSingle::typeid);
		types->Add("smalldatetime", SqlTypes::SqlDateTime::typeid);
		types->Add("smallint", SqlTypes::SqlInt16::typeid);
		types->Add("smallmoney", SqlTypes::SqlMoney::typeid);
		types->Add("tinyint", SqlTypes::SqlByte::typeid);
		types->Add("uniqueidentifier", SqlTypes::SqlGuid::typeid);
		types->Add("varbinary", SqlTypes::SqlBinary::typeid);
		types->Add("varchar", SqlTypes::SqlString::typeid);

		return types;
	}

	SqlObject::TypeDictionary^ SqlObject::getSqlOtherDataTypes() {
		TypeDictionary^ types = gcnew TypeDictionaryImpl;

		types->Add("varchar(max)", SqlTypes::SqlString::typeid);
		types->Add("nvarchar(max)", SqlTypes::SqlString::typeid);
		types->Add("varbinary(max)", SqlTypes::SqlBinary::typeid);
		types->Add("xml", SqlTypes::SqlXml::typeid);
		types->Add("hierarchyid", Microsoft::SqlServer::Types::SqlHierarchyId::typeid);
		types->Add("geography", Microsoft::SqlServer::Types::SqlGeography::typeid);
		types->Add("geometry", Microsoft::SqlServer::Types::SqlGeometry::typeid);
		types->Add("dictionary", Types::Dictionary::typeid);
		types->Add("list", Types::List::typeid);
		types->Add("enumerator", Types::Enumerator::typeid);
		types->Add("udt", SqlTypes::SqlBinary::typeid);
		types->Add("sql_variant", Object::typeid);

		return types;
	}

	SqlObject::TypeIndex^ SqlObject::getSqlTypeIndex() {
		TypeIndex^ types = gcnew TypeIndexImpl;

		types->Add(SqlTypes::SqlInt64::typeid, 0);
		types->Add(SqlTypes::SqlBinary::typeid, 1);
		types->Add(SqlTypes::SqlBoolean::typeid, 2);
		types->Add(SqlTypes::SqlString::typeid, 3);
		types->Add(DateTime::typeid, 4);
		types->Add(SqlTypes::SqlDateTime::typeid, 5);
		types->Add(DateTimeOffset::typeid, 6);
		types->Add(SqlTypes::SqlDecimal::typeid, 7);
		types->Add(SqlTypes::SqlDouble::typeid, 8);
		types->Add(SqlTypes::SqlInt32::typeid, 9);
		types->Add(SqlTypes::SqlMoney::typeid, 10);
		types->Add(SqlTypes::SqlSingle::typeid, 11);
		types->Add(SqlTypes::SqlInt16::typeid, 12);
		types->Add(SqlTypes::SqlByte::typeid, 13);
		types->Add(SqlTypes::SqlGuid::typeid, 14);
		types->Add(SqlTypes::SqlXml::typeid, 15);
		types->Add(Microsoft::SqlServer::Types::SqlHierarchyId::typeid, 16);
		types->Add(Microsoft::SqlServer::Types::SqlGeography::typeid, 17);
		types->Add(Microsoft::SqlServer::Types::SqlGeometry::typeid, 18);
		types->Add(Types::Dictionary::typeid, 19);
		types->Add(Types::List::typeid, 20);
		types->Add(Types::Enumerator::typeid, 21);

		return types;
	}

	SqlObject::TypeStringIndex^ SqlObject::getSqlTypeStringIndex() {
		TypeStringIndex^ types = gcnew TypeStringIndexImpl;

		types->Add(SqlTypes::SqlInt64::typeid, "bigint");
		types->Add(SqlTypes::SqlBinary::typeid, "binary");
		types->Add(SqlTypes::SqlBoolean::typeid, "bit");
		types->Add(SqlTypes::SqlString::typeid, "string");
		types->Add(DateTime::typeid, "datetime2");
		types->Add(SqlTypes::SqlDateTime::typeid, "datetime");
		types->Add(DateTimeOffset::typeid, "datetimeoffset");
		types->Add(SqlTypes::SqlDecimal::typeid, "decimal");
		types->Add(SqlTypes::SqlDouble::typeid, "float");
		types->Add(SqlTypes::SqlInt32::typeid, "int");
		types->Add(SqlTypes::SqlMoney::typeid, "money");
		types->Add(SqlTypes::SqlSingle::typeid, "real");
		types->Add(SqlTypes::SqlInt16::typeid, "smallint");
		types->Add(SqlTypes::SqlByte::typeid, "tinyint");
		types->Add(SqlTypes::SqlGuid::typeid, "uniqueidentifier");
		types->Add(SqlTypes::SqlXml::typeid, "xml");
		types->Add(Microsoft::SqlServer::Types::SqlHierarchyId::typeid, "hierarchyid");
		types->Add(Microsoft::SqlServer::Types::SqlGeography::typeid, "geography");
		types->Add(Microsoft::SqlServer::Types::SqlGeometry::typeid, "geometry");
		types->Add(Types::Dictionary::typeid, "dictionary");
		types->Add(Types::List::typeid, "list");
		types->Add(Types::Enumerator::typeid, "enumerator");

		return types;
	}

	SqlObject::IndexOfTypes^ SqlObject::getSqlIndexOfTypes() {
		IndexOfTypes^ types = gcnew IndexOfTypesImpl;

		types->Add(0, SqlTypes::SqlInt64::typeid);
		types->Add(1, SqlTypes::SqlBinary::typeid);
		types->Add(2, SqlTypes::SqlBoolean::typeid);
		types->Add(3, SqlTypes::SqlString::typeid);
		types->Add(4, DateTime::typeid);
		types->Add(5, SqlTypes::SqlDateTime::typeid);
		types->Add(6, DateTimeOffset::typeid);
		types->Add(7, SqlTypes::SqlDecimal::typeid);
		types->Add(8, SqlTypes::SqlDouble::typeid);
		types->Add(9, SqlTypes::SqlInt32::typeid);
		types->Add(10, SqlTypes::SqlMoney::typeid);
		types->Add(11, SqlTypes::SqlSingle::typeid);
		types->Add(12, SqlTypes::SqlInt16::typeid);
		types->Add(13, SqlTypes::SqlByte::typeid);
		types->Add(14, SqlTypes::SqlGuid::typeid);
		types->Add(15, SqlTypes::SqlXml::typeid);
		types->Add(16, Microsoft::SqlServer::Types::SqlHierarchyId::typeid);
		types->Add(17, Microsoft::SqlServer::Types::SqlGeography::typeid);
		types->Add(18, Microsoft::SqlServer::Types::SqlGeometry::typeid);
		types->Add(19, Types::Dictionary::typeid);
		types->Add(20, Types::List::typeid);
		types->Add(21, Types::Enumerator::typeid);

		return types;
	}

	SqlObject::RegexReplacementDictionary^ SqlObject::getSqlTypeStandardisationRegex() {
		RegexReplacementDictionary^ dictionary = gcnew RegexReplacementDictionaryImpl;

		dictionary->Add(TypeStandardization::Cleanup, gcnew RegexReplacementArrayImpl {
															RegexReplacement(gcnew Text::RegularExpressions::Regex("[ \t]*\\([ \t]*"), "("),
															RegexReplacement(gcnew Text::RegularExpressions::Regex("[ \t]*\\)"), ")"),
															RegexReplacement(gcnew Text::RegularExpressions::Regex("[ \t]*,[ \t]*"), ", "),
															RegexReplacement(gcnew Text::RegularExpressions::Regex("[ \t]+"), " ")
														});

		dictionary->Add(TypeStandardization::Float, gcnew RegexReplacementArrayImpl{ 
															RegexReplacement(gcnew Text::RegularExpressions::Regex("^(float)\\(([0-9]*)\\)$"), "float")
														});

		dictionary->Add(TypeStandardization::FloatPrecision, gcnew RegexReplacementArrayImpl{ 
															RegexReplacement(gcnew Text::RegularExpressions::Regex("^(float)\\(([0-9]*)\\)$"), "$2")
														});

		dictionary->Add(TypeStandardization::Real, gcnew RegexReplacementArrayImpl{ 
															RegexReplacement(gcnew Text::RegularExpressions::Regex("^(float)\\(([0-9]*)\\)$"), "real")
														});

		dictionary->Add(TypeStandardization::Decimal, gcnew RegexReplacementArrayImpl{ 
															RegexReplacement(gcnew Text::RegularExpressions::Regex("^(decimal|numeric|dec)\\(([0-9]*)\\)$"), "decimal"),
															RegexReplacement(gcnew Text::RegularExpressions::Regex("^(decimal|numeric|dec)\\(([0-9]*), ([0-9]*)\\)$"), "decimal")
														});

		dictionary->Add(TypeStandardization::DecimalPrecision, gcnew RegexReplacementArrayImpl{ 
															RegexReplacement(gcnew Text::RegularExpressions::Regex("^(decimal|numeric|dec)\\(([0-9]*), ([0-9]*)\\)$"), "$2"),
															RegexReplacement(gcnew Text::RegularExpressions::Regex("^(decimal|numeric|dec)\\(([0-9]*)\\)$"), "$2")
														});

		dictionary->Add(TypeStandardization::DecimalScale, gcnew RegexReplacementArrayImpl{ 
															RegexReplacement(gcnew Text::RegularExpressions::Regex("^(decimal|numeric|dec)\\(([0-9]*), ([0-9]*)\\)$"), "$3")
														});

		dictionary->Add(TypeStandardization::Time, gcnew RegexReplacementArrayImpl{ 
															RegexReplacement(gcnew Text::RegularExpressions::Regex("^(time|datetime2|datetimeoffset)\\(([0-9]*)\\)$"), "$1")
														});

		dictionary->Add(TypeStandardization::TimePrecision, gcnew RegexReplacementArrayImpl{ 
															RegexReplacement(gcnew Text::RegularExpressions::Regex("^(time|datetime2|datetimeoffset)\\(([0-9]*)\\)$"), "$2")
														});

		dictionary->Add(TypeStandardization::Character, gcnew RegexReplacementArrayImpl{ 
															RegexReplacement(gcnew Text::RegularExpressions::Regex("^(varchar|character varying|char varying)\\(([0-9]*)\\)$"), "varchar"),
															RegexReplacement(gcnew Text::RegularExpressions::Regex("^(char|character)\\(([0-9]*)\\)$"), "char"),
															RegexReplacement(gcnew Text::RegularExpressions::Regex("^(nvarchar|national character varying|national char varying)\\(([0-9]*)\\)$"), "nvarchar"),
															RegexReplacement(gcnew Text::RegularExpressions::Regex("^(nchar|national character|national char)\\(([0-9]*)\\)$"), "nchar")
														});

		dictionary->Add(TypeStandardization::CharacterLength, gcnew RegexReplacementArrayImpl{ 
															RegexReplacement(gcnew Text::RegularExpressions::Regex("^(varchar|character varying|char varying|nvarchar|national character varying|national char varying|char|character|nchar|national character|national char)\\(([0-9]*)\\)$"), "$2")
														});

		dictionary->Add(TypeStandardization::Binary, gcnew RegexReplacementArrayImpl{ 
															RegexReplacement(gcnew Text::RegularExpressions::Regex("^(binary)\\(([0-9]*)\\)$"), "$1"),
															RegexReplacement(gcnew Text::RegularExpressions::Regex("^(binary varying|varbinary)\\(([0-9]*)\\)$"), "varbinary")
														});

		dictionary->Add(TypeStandardization::BinaryLength, gcnew RegexReplacementArrayImpl{ 
															RegexReplacement(gcnew Text::RegularExpressions::Regex("^(binary|binary varying|varbinary)\\(([0-9]*)\\)$"), "$2")
														});

		dictionary->Add(TypeStandardization::Synonyms, gcnew RegexReplacementArrayImpl {
															RegexReplacement(gcnew Text::RegularExpressions::Regex("^binary varying$"), "varbinary"),
															RegexReplacement(gcnew Text::RegularExpressions::Regex("^character varying$"), "varchar"),
															RegexReplacement(gcnew Text::RegularExpressions::Regex("^char varying$"), "varchar"),
															RegexReplacement(gcnew Text::RegularExpressions::Regex("^character$"), "char"),
															RegexReplacement(gcnew Text::RegularExpressions::Regex("^national character varying$"), "nvarchar"),
															RegexReplacement(gcnew Text::RegularExpressions::Regex("^national char varying$"), "nvarchar"),
															RegexReplacement(gcnew Text::RegularExpressions::Regex("^national character$"), "nchar"),
															RegexReplacement(gcnew Text::RegularExpressions::Regex("^national char$"), "nchar"),
															RegexReplacement(gcnew Text::RegularExpressions::Regex("^dec$"), "decimal"),
															RegexReplacement(gcnew Text::RegularExpressions::Regex("^numeric$"), "decimal"),
															RegexReplacement(gcnew Text::RegularExpressions::Regex("^double precision$"), "float"),
															RegexReplacement(gcnew Text::RegularExpressions::Regex("^integer$"), "int")
														});

		return dictionary;
	}

	SqlObject::MethodDictionary^ SqlObject::getBinaryReaderMethods() {
		MethodDictionary^ dictionary = gcnew MethodDictionaryImpl;

		dictionary->Add(SqlTypes::SqlInt64::typeid, "ReadInt64");
		dictionary->Add(SqlTypes::SqlBinary::typeid, "ReadBytes");
		dictionary->Add(SqlTypes::SqlBoolean::typeid, "ReadBoolean");
		dictionary->Add(SqlTypes::SqlString::typeid, "ReadString");
		dictionary->Add(DateTime::typeid, "ReadInt64");
		dictionary->Add(SqlTypes::SqlDateTime::typeid, "ReadInt64");
		dictionary->Add(DateTimeOffset::typeid, "ReadInt64");
		dictionary->Add(SqlTypes::SqlDecimal::typeid, "ReadDecimal");
		dictionary->Add(SqlTypes::SqlDouble::typeid, "ReadDouble");
		dictionary->Add(SqlTypes::SqlInt32::typeid, "ReadInt32");
		dictionary->Add(SqlTypes::SqlMoney::typeid, "ReadDecimal");
		dictionary->Add(SqlTypes::SqlSingle::typeid, "ReadSingle");
		dictionary->Add(SqlTypes::SqlInt16::typeid, "ReadInt16");
		dictionary->Add(SqlTypes::SqlByte::typeid, "ReadByte");
		dictionary->Add(SqlTypes::SqlGuid::typeid, "ReadBytes");
		dictionary->Add(SqlTypes::SqlXml::typeid, "ReadString");
		
		return dictionary;
	}

	SqlObject::MethodDictionary^ SqlObject::getIndependentBinaryReaders() {
		MethodDictionary^ dictionary = gcnew MethodDictionaryImpl;

		dictionary->Add(Microsoft::SqlServer::Types::SqlHierarchyId::typeid, "Read");
		dictionary->Add(Microsoft::SqlServer::Types::SqlGeography::typeid, "Read");
		dictionary->Add(Microsoft::SqlServer::Types::SqlGeometry::typeid, "Read");
		dictionary->Add(Types::Dictionary::typeid, "Read");
		dictionary->Add(Types::Enumerator::typeid, "Read");
		dictionary->Add(Types::List::typeid, "Read");
		dictionary->Add(Object::typeid, String::Empty);

		return dictionary;
	}

	Reflection::PropertyInfo^ SqlObject::getValueProperty(Type^ type) {
		return type->GetProperty("Value");
	}

	Reflection::PropertyInfo^ SqlObject::getIsNullProperty(Type^ type) {
		return type->GetProperty("IsNull");
	}

	Reflection::PropertyInfo^ SqlObject::getNullProperty(Type^ type) {
		return type->GetProperty("Null");
	}

	Reflection::FieldInfo^ SqlObject::getNullField(Type^ type) {
		return type->GetField("Null");
	}

	Reflection::ConstructorInfo^ SqlObject::getConstructorMethod(Type^ type, Reflection::PropertyInfo^ valueProperty) {
		if (valueProperty) {
			return type->GetConstructor(gcnew cli::array<Type^> { valueProperty->PropertyType });
		}

		return nullptr;
	}

	Reflection::ConstructorInfo^ SqlObject::getConstructorMethod(Type^ type) {
		return getConstructorMethod(type, getValueProperty(type));
	}

	Reflection::ConstructorInfo^ SqlObject::getNoArgsConstructorMethod(Type^ type) {
		return type->GetConstructor(Type::EmptyTypes);
	}

	Reflection::MethodInfo^ SqlObject::getParseMethod(Type^ type) {
		if (type == SqlTypes::SqlBinary::typeid) {
			return Utils::Binary::typeid->GetMethod("HexToBytes", Reflection::BindingFlags::Static | Reflection::BindingFlags::NonPublic);
		} 
		
		if (type == SqlTypes::SqlXml::typeid) {
			return SqlObject::typeid->GetMethod("toSqlXml", Reflection::BindingFlags::Static | Reflection::BindingFlags::NonPublic);
		} 
		
		return type->GetMethod("Parse", gcnew cli::array<Type^> { String::typeid });
	}

	Reflection::MethodInfo^ SqlObject::getBinaryReaderMethod(Type^ type) {
		return BinaryReaderMethods->ContainsKey(type) ? IO::BinaryReader::typeid->GetMethod(BinaryReaderMethods[type]) : nullptr;
	}

	Reflection::MethodInfo^ SqlObject::getBinaryWriterMethod(Type^ type) {
		return BinaryReaderMethods->ContainsKey(type) ? getBinaryWriterMethod(getValueProperty(type)) : nullptr;
	}

	Reflection::MethodInfo^ SqlObject::getBinaryWriterMethod(Reflection::PropertyInfo^ propertyType) {
		if (propertyType) {
			return IO::BinaryWriter::typeid->GetMethod("Write", gcnew cli::array<Type^> { propertyType->PropertyType });
		}

		return nullptr;
	}

	String^ SqlObject::applyRegexReplacements(String^ value, RegexReplacementArray^ replacements) {
		for each (RegexReplacement replacement in replacements) {
			value = replacement.Key->Replace(value, replacement.Value);
		}

		return value;
	}

	SqlObject^ SqlObject::tryCreate(String^ sqlDataType, String^ sqlType, int precision, int scale, int length) {
		if (SqlVariantDataTypes->ContainsKey(sqlDataType)) {
			Type^ type = SqlVariantDataTypes[sqlDataType];
			return gcnew SqlObject(type, sqlType, precision, scale, (sqlType == "varchar" || 
																		sqlType == "nvarchar" || 
																		sqlType == "char" || 
																		sqlType == "nchar" || 
																		sqlType == "binary" || 
																		sqlType == "varbinary") && length == -1 ? (sqlType == "nvarchar" || sqlType == "nchar" ? 4000 : 8000) : length, true);
		}

		if (SqlOtherDataTypes->ContainsKey(sqlDataType)) {
			Type^ type = SqlOtherDataTypes[sqlDataType];
			return gcnew SqlObject(type, sqlType, precision, scale, length, sqlDataType == SqlVariant);
		}

		return nullptr;
	}

	SqlTypes::SqlXml^ SqlObject::toSqlXml(String^ value) {
		return gcnew SqlTypes::SqlXml(Xml::XmlReader::Create(gcnew IO::StringReader(value)));
	}

	Object^ SqlObject::GetValue(Object^ instance) {
		if (instance->GetType() == System::String::typeid) {
			return instance;
		}
		Reflection::PropertyInfo^ valueProperty = nullptr;
		if (m_sqlDataType != Object::typeid) {
			valueProperty = m_valueProperty;
		} else if (instance) {
			valueProperty = getValueProperty(instance->GetType());
		}

		return !IsNull(instance) ? (valueProperty ? valueProperty->GetValue(instance, nullptr) : instance) : nullptr;
	}

	bool SqlObject::IsNull(Object^ instance) {
		if (instance->GetType() == System::String::typeid) {
			return instance == nullptr;
		}
		Reflection::PropertyInfo^ isNullProperty = nullptr;
		if (m_sqlDataType != Object::typeid) {
			isNullProperty = m_isNullProperty;
		} else if (instance) {
			isNullProperty = getIsNullProperty(instance->GetType());
		}

		return !instance || instance == DBNull::Value || (isNullProperty && System::Convert::ToBoolean(isNullProperty->GetValue(instance, nullptr)));
	}

	Object^ SqlObject::Parse(String^ value) {
		Object^ parsedValue = nullptr;
		if (m_sqlDataType != Object::typeid) {
			parsedValue = m_parseMethod && value ? m_parseMethod->Invoke(nullptr, gcnew cli::array<Object^> { value }) : (value ? SqlTypes::SqlString(value) : Null);
		} else if (value) {
			if (!SqlVariantRegEx->IsMatch(value)) {
				throw gcnew SmartSqlClrException("Invalid sql_variant string value");
			}

			parsedValue = Create(SqlVariantRegEx->Replace(value, "$1"))->Parse(SqlVariantRegEx->Replace(value, "$2"));
		}

		if (!IsValueValid(parsedValue)) {
			throw gcnew SmartSqlClrException("Invalid string value, value is not valid for the specified data type");
		}

		return parsedValue;
	}

	String^ SqlObject::ConvertToString(Object^ instance) {
		return ConvertToString(instance, true);
	}

	String^ SqlObject::ConvertToString(Object^ instance, bool includeTypePrefix) {
		Type^ type = nullptr;
		String^ typeString = nullptr;
		if (m_sqlDataType != Object::typeid) {
			type = m_sqlDataType;
			typeString = m_sqlType;
		} else {
			type = instance->GetType();
			if (type != System::String::typeid) {
				typeString = SqlTypeStringIndex[type];
			}
		}

		Object^ value = GetValue(instance);
		String^ stringValue = nullptr;
		if (value) {
			if (type != SqlTypes::SqlBinary::typeid && type != DateTime::typeid && type != DateTimeOffset::typeid && type != SqlTypes::SqlDateTime::typeid) {
				stringValue = Convert::ToString(value);
			} else if (type == SqlTypes::SqlDateTime::typeid) {
				if (typeString == ("smalldatetime")) {
					stringValue = safe_cast<SqlTypes::SqlDateTime^>(instance)->Value.ToString(SqlSmallDateTimeFormat);
				} else {
					stringValue = safe_cast<SqlTypes::SqlDateTime^>(instance)->Value.ToString(SqlDateTimeFormat);
				}
			} else if (type == DateTime::typeid) {
				if (typeString == "date") {
					stringValue = safe_cast<DateTime^>(instance)->ToString(DateFormat);
				} else if (typeString && typeString->StartsWith("time")) {
					stringValue = safe_cast<DateTime^>(instance)->ToString(TimeFormat);
				} else {
					stringValue = safe_cast<DateTime^>(instance)->ToString(DateTimeFormat);
				}
			} else if (type == DateTimeOffset::typeid) {
				stringValue = safe_cast<DateTimeOffset^>(instance)->ToString(DateTimeOffsetFormat);
			} else {
				stringValue = Utils::Binary::ToHex(safe_cast<cli::array<Byte>^>(value)).Value;
			}

			if (m_sqlDataType == Object::typeid && includeTypePrefix) {
				stringValue = String::Format("[{0}]{1}", typeString, stringValue);
			}
		}

		return stringValue;
	}

	bool SqlObject::IsValueValid(Object^ value) {
		if (value && value != DBNull::Value) {
			if (m_sqlDataType != Object::typeid) {
				if (m_sqlDataType == value->GetType()) {
					if (!IsNull(value)) {
						if (m_sqlDataType == SqlTypes::SqlString::typeid) {
							return m_length == -1 || safe_cast<SqlTypes::SqlString>(value).Value->Length <= m_length;
						}

						if (m_sqlDataType == SqlTypes::SqlBinary::typeid) {
							return m_length == -1 || safe_cast<SqlTypes::SqlBinary>(value).Length <= m_length;
						}

						if (m_sqlDataType == SqlTypes::SqlDecimal::typeid) {
							SqlTypes::SqlDecimal decimal = safe_cast<SqlTypes::SqlDecimal>(value);
							return m_precision == -1 || ((decimal.Precision - decimal.Scale) <= m_precision);
						}
					}
				} else {
					return false;
				}
			} else {
				if (value->GetType() != SqlTypes::SqlBinary::typeid && value->GetType() != SqlTypes::SqlString::typeid) {
					return SqlVariantDataTypes->Values->Contains(value->GetType());
				}

				if (!IsNull(value)) {
					return (value->GetType() == SqlTypes::SqlBinary::typeid && safe_cast<SqlTypes::SqlBinary>(value).Value->Length <= 8000) || 
							(value->GetType() == SqlTypes::SqlString::typeid && safe_cast<SqlTypes::SqlString>(value).Value->Length <= 8000);
				}
			}
		}

		return true;
	}

	System::Object^ SqlObject::Read(IO::BinaryReader^ reader) {
		Type^ type = nullptr;
		Reflection::MethodInfo^ binaryReaderMethod = nullptr;
		if (m_sqlDataType != Object::typeid) {
			type = m_sqlDataType;
			binaryReaderMethod = m_binaryReaderMethod;
		} else {
			int typeIndex = reader->ReadInt16();
			if (typeIndex != -1) {
				type = SqlIndexOfTypes[typeIndex];
				binaryReaderMethod = getBinaryReaderMethod(type);
			}
		}

		if (!reader->ReadBoolean()) {
			if (type != SqlTypes::SqlBinary::typeid && type != DateTime::typeid && type != DateTimeOffset::typeid && type != SqlTypes::SqlDateTime::typeid && type != SqlTypes::SqlGuid::typeid && type != SqlTypes::SqlXml::typeid) {
				if (binaryReaderMethod) {
					return construct(binaryReaderMethod->Invoke(reader, nullptr), type);
				}

				return construct(reader, type);
			}

			if (type == SqlTypes::SqlBinary::typeid) {
				return construct(reader->ReadBytes(reader->ReadInt32()), type);
			}

			if (type == SqlTypes::SqlDateTime::typeid || type == DateTime::typeid) {
				return construct(DateTime(reader->ReadInt64()), type);
			}

			if (type == DateTimeOffset::typeid) {
				return construct(DateTimeOffset(reader->ReadInt64(), TimeSpan(reader->ReadInt64())), type);
			}

			if (type == SqlTypes::SqlGuid::typeid) {
				return construct(Guid(reader->ReadBytes(16)), type);
			}

			return construct(reader->ReadString(), type);
		} 

		return Null;
	}

	void SqlObject::Write(Object^ value, IO::BinaryWriter^ writer) {
		Type^ type = nullptr;
		Reflection::MethodInfo^ binaryWriterMethod = nullptr;
		if (m_sqlDataType != Object::typeid) {
			type = m_sqlDataType;
			binaryWriterMethod = m_binaryWriterMethod;
		} else {
			if (value && value != DBNull::Value) {
				type = value->GetType();
				binaryWriterMethod = getBinaryWriterMethod(type);

				if (type == System::String::typeid) {
					writer->Write(safe_cast<Int16>(SqlTypeIndex[System::Data::SqlTypes::SqlString::typeid]));
				} else {
					writer->Write(safe_cast<Int16>(SqlTypeIndex[type]));
				}
			} else {
				writer->Write(safe_cast<Int16>(-1));
			}
		}

		if (!IsNull(value)) {
			writer->Write(false);

			Object^ nativeValue = GetValue(value);
			if (type != SqlTypes::SqlBinary::typeid && type != SqlTypes::SqlDateTime::typeid && type != DateTime::typeid && type != DateTimeOffset::typeid && type != SqlTypes::SqlGuid::typeid && type != SqlTypes::SqlXml::typeid) {
				if (binaryWriterMethod) {
					binaryWriterMethod->Invoke(writer, gcnew cli::array<Object^> { nativeValue });
				} else {
					if (type == System::String::typeid) {
						writer->Write(value->ToString());
					} else {
						value->GetType()->GetMethod("Write")->Invoke(value, gcnew cli::array<Object^> { writer });
					}
				}
			} else if (type == SqlTypes::SqlBinary::typeid) {
				cli::array<Byte>^ bytes = safe_cast<cli::array<Byte>^>(nativeValue);

				writer->Write(bytes->Length);
				writer->Write(bytes);
			} else if (type == DateTime::typeid || type == SqlTypes::SqlDateTime::typeid) {
				writer->Write(safe_cast<DateTime>(nativeValue).Ticks);
			} else if (type == DateTimeOffset::typeid) {
				DateTimeOffset val = safe_cast<DateTimeOffset>(nativeValue);

				writer->Write(val.Offset.Ticks);
				writer->Write(val.Ticks);
			} else if (type == SqlTypes::SqlGuid::typeid) {
				writer->Write(safe_cast<Guid>(nativeValue).ToByteArray());
			} else {
				writer->Write(safe_cast<String^>(nativeValue));
			}
		} else {
			writer->Write(true);
		}
	}

	Type^ SqlObject::SqlDataType::get() {
		return m_sqlDataType;
	}

	String^ SqlObject::SqlType::get() {
		return m_sqlType;
	}

	int SqlObject::Precision::get() {
		return m_precision;
	}

	int SqlObject::Scale::get() {
		return m_scale;
	}

	int SqlObject::Length::get() {
		return m_length;
	}

	Object^ SqlObject::Null::get() {
		if (m_sqlDataType != Object::typeid) {
			return m_nullField ? m_nullField->GetValue(nullptr) : (m_nullProperty ? m_nullProperty->GetValue(nullptr, nullptr) : DBNull::Value);
		}

		return DBNull::Value;
	}

	bool SqlObject::IsSqlVariantCompatible::get() {
		return m_isSqlVariantCompatible;
	}

	bool SqlObject::IsNumeric::get() {
		return m_isNumeric;
	}

	SqlObject^ SqlObject::Create() {
		return gcnew SqlObject(SqlTypes::SqlString::typeid, "varchar", -1, -1, 8000, true);
	}

	SqlObject^ SqlObject::Create(String^ sqlType) {
		if (!sqlType) {
			throw gcnew ArgumentNullException("sqlType");
		}

		SqlObject^ sqlObject = nullptr;
		String^ standardisedSqlType = applyRegexReplacements(sqlType->Trim()->ToLower(), SqlTypeStandardisationRegex[TypeStandardization::Cleanup]);
		standardisedSqlType = applyRegexReplacements(standardisedSqlType, SqlTypeStandardisationRegex[TypeStandardization::Synonyms]);
		if (!(sqlObject = tryCreate(standardisedSqlType, standardisedSqlType, -1, -1, -1))) {
			if (standardisedSqlType->StartsWith("float(")) {
				int precision = -1;
				try {
					precision = Convert::ToInt32(applyRegexReplacements(standardisedSqlType, SqlTypeStandardisationRegex[TypeStandardization::FloatPrecision]));
				} catch (Exception^) { }

				if (precision >= 1 && precision <= 53) {
					if (precision <= 24) {
						standardisedSqlType = applyRegexReplacements(standardisedSqlType, SqlTypeStandardisationRegex[TypeStandardization::Real]);
					} else {
						standardisedSqlType = applyRegexReplacements(standardisedSqlType, SqlTypeStandardisationRegex[TypeStandardization::Float]);
					}

					sqlObject = tryCreate(standardisedSqlType, standardisedSqlType, precision, -1, -1);
				}
			} else if (standardisedSqlType->StartsWith("dec(") || standardisedSqlType->StartsWith("decimal(") || standardisedSqlType->StartsWith("numeric(")) {
				int precision = -1;
				int scale = 0;
				bool hasScale = false;
				try {
					precision = Convert::ToInt32(applyRegexReplacements(standardisedSqlType, SqlTypeStandardisationRegex[TypeStandardization::DecimalPrecision]));
					scale = Convert::ToInt32(applyRegexReplacements(standardisedSqlType, SqlTypeStandardisationRegex[TypeStandardization::DecimalScale]));

					hasScale = true;
				} catch (Exception^) { }

				if (precision >= 0 && precision <= 38 && scale >= 0 && ((hasScale && scale <= precision) || !hasScale)) {
					standardisedSqlType = applyRegexReplacements(standardisedSqlType, SqlTypeStandardisationRegex[TypeStandardization::Decimal]);
					sqlObject = tryCreate(standardisedSqlType, hasScale ? String::Format("{0}({1}, {2})", standardisedSqlType, precision, scale) : String::Format("{0}({1})", standardisedSqlType, precision), precision, scale, -1);
				}
			} else if (standardisedSqlType->StartsWith("time(") || standardisedSqlType->StartsWith("datetime2(") || standardisedSqlType->StartsWith("datetimeoffset(")) {
				int precision = -1;
				try {
					precision = Convert::ToInt32(applyRegexReplacements(standardisedSqlType, SqlTypeStandardisationRegex[TypeStandardization::TimePrecision]));
				} catch (Exception^) { }

				if (precision >= 0 && precision <= 7) {
					standardisedSqlType = applyRegexReplacements(standardisedSqlType, SqlTypeStandardisationRegex[TypeStandardization::Time]);
					sqlObject = tryCreate(standardisedSqlType, String::Format("{0}({1})", standardisedSqlType, precision), precision, -1, -1);
				}
			} else if (standardisedSqlType->StartsWith("varchar(") || standardisedSqlType->StartsWith("nvarchar(") || standardisedSqlType->StartsWith("char(") || standardisedSqlType->StartsWith("nchar(") || standardisedSqlType->StartsWith("character(") || standardisedSqlType->StartsWith("character varying(") || standardisedSqlType->StartsWith("char varying(") || standardisedSqlType->StartsWith("national character varying(") || standardisedSqlType->StartsWith("national char varying(") || standardisedSqlType->StartsWith("national character(") || standardisedSqlType->StartsWith("national char(")) {
				int length = -1;
				try {
					length = Convert::ToInt32(applyRegexReplacements(standardisedSqlType, SqlTypeStandardisationRegex[TypeStandardization::CharacterLength]));
				} catch (Exception^) { }

				if (length >= 1 && length <= (standardisedSqlType->StartsWith("varchar") || standardisedSqlType->StartsWith("char") ? 8000 : 4000)) {
					standardisedSqlType = applyRegexReplacements(standardisedSqlType, SqlTypeStandardisationRegex[TypeStandardization::Character]);
					sqlObject = tryCreate(standardisedSqlType, String::Format("{0}({1})", standardisedSqlType, length), -1, -1, length);
				}
			} else if (standardisedSqlType->StartsWith("varbinary(") || standardisedSqlType->StartsWith("binary(") || standardisedSqlType->StartsWith("binary varying(")) {
				int length = -1;
				try {
					length = Convert::ToInt32(applyRegexReplacements(standardisedSqlType, SqlTypeStandardisationRegex[TypeStandardization::BinaryLength]));
				} catch (Exception^) { }

				if (length >= 1 && length <= 8000) {
					standardisedSqlType = applyRegexReplacements(standardisedSqlType, SqlTypeStandardisationRegex[TypeStandardization::Binary]);
					sqlObject = tryCreate(standardisedSqlType, String::Format("{0}({1})", standardisedSqlType, length), -1, -1, length);
				}
			}
		}
		
		if (!sqlObject) {
			throw gcnew SmartSqlClrException(String::Format("Invalid type '{0}'", sqlType));
		}

		return sqlObject;
	}

	SqlObject^ SqlObject::Create(Type^ sqlDataType) {
		if (!SqlTypeStringIndex->ContainsKey(sqlDataType)) {
			throw gcnew SmartSqlClrException(String::Format("Invalid type '{0}'", sqlDataType->Name));
		}

		return Create(SqlTypeStringIndex[sqlDataType]);
	}
}}}
