
#ifndef _SMART_SQLCLR_INTERNAL__SQLOBJECT_H_
#define _SMART_SQLCLR_INTERNAL__SQLOBJECT_H_

#include <Smart/SqlClr/Internal/Interfaces/ISqlObject.hpp>

#using <System.dll>
#using <System.Data.dll>

namespace Smart { namespace SqlClr { namespace Internal {
	using namespace System;
	using namespace System::Data;

	/**
	 * @class SqlObject
	 *
	 * @brief An implementation of the ISqlObject interface
	 *
	 * Allows for the manipulation of an object of a Sql data type
	 * without needing to know the underlying type. Can be useful
	 * when working with sql_variant objects.
	 *
	 * @author Muhmud Ahmad
	 *
	 */
	ref class SqlObject sealed : public Interfaces::ISqlObject {
	private:
		enum class TypeStandardization {
			Cleanup, Float, FloatPrecision, Real, Decimal, DecimalPrecision, DecimalScale, Time, TimePrecision, Character, CharacterLength, Binary, BinaryLength, Synonyms
		};

		typedef Collections::Generic::IDictionary<String^, Type^> TypeDictionary;
		typedef Collections::Generic::Dictionary<String^, Type^> TypeDictionaryImpl;

		typedef Collections::Generic::IDictionary<Type^, String^> MethodDictionary;
		typedef Collections::Generic::Dictionary<Type^, String^> MethodDictionaryImpl;

		typedef Collections::Generic::IDictionary<Type^, int> TypeIndex;
		typedef Collections::Generic::Dictionary<Type^, int> TypeIndexImpl;

		typedef Collections::Generic::IDictionary<Type^, String^> TypeStringIndex;
		typedef Collections::Generic::Dictionary<Type^, String^> TypeStringIndexImpl;

		typedef Collections::Generic::IDictionary<int, Type^> IndexOfTypes;
		typedef Collections::Generic::Dictionary<int, Type^> IndexOfTypesImpl;

		typedef Collections::Generic::KeyValuePair<Text::RegularExpressions::Regex^, String^> RegexReplacement;

		typedef cli::array<RegexReplacement> RegexReplacementArray;
		typedef cli::array<RegexReplacement> RegexReplacementArrayImpl;

		typedef Collections::Generic::IDictionary<TypeStandardization, RegexReplacementArray^> RegexReplacementDictionary;
		typedef Collections::Generic::Dictionary<TypeStandardization, RegexReplacementArray^> RegexReplacementDictionaryImpl;

		static initonly TypeDictionary^ SqlVariantDataTypes = getSqlVariantDataTypes();
		static initonly TypeDictionary^ SqlOtherDataTypes = getSqlOtherDataTypes();

		static initonly TypeIndex^ SqlTypeIndex = getSqlTypeIndex();
		static initonly TypeStringIndex^ SqlTypeStringIndex = getSqlTypeStringIndex();
		static initonly IndexOfTypes^ SqlIndexOfTypes = getSqlIndexOfTypes();

		static initonly RegexReplacementDictionary^ SqlTypeStandardisationRegex = getSqlTypeStandardisationRegex();

		static initonly MethodDictionary^ BinaryReaderMethods = getBinaryReaderMethods();
		static initonly MethodDictionary^ IndependentBinaryReaders = getIndependentBinaryReaders();

		static initonly String^ SqlDateTimeFormat = "yyyy-MM-dd HH:mm:ss.fff";
		static initonly String^ SqlSmallDateTimeFormat = "yyyy-MM-dd HH:mm";
		static initonly String^ DateTimeFormat = "yyyy-MM-dd HH:mm:ss.fffffff";
		static initonly String^ DateTimeOffsetFormat = "yyyy-MM-dd HH:mm:ss.fffffff zzzz";
		static initonly String^ DateFormat = "yyyy-MM-dd";
		static initonly String^ TimeFormat = "HH:mm:ss.fffffff";

		static initonly Text::RegularExpressions::Regex^ SqlVariantRegEx = gcnew Text::RegularExpressions::Regex("^\\[([a-z0-9\\(\\), ]*)\\](.*)", Text::RegularExpressions::RegexOptions::Singleline | Text::RegularExpressions::RegexOptions::Multiline | Text::RegularExpressions::RegexOptions::IgnoreCase);

		Type^ m_sqlDataType;
		String^ m_sqlType;
		bool m_isSqlVariantCompatible;
		bool m_isNumeric;

		int m_precision;
		int m_scale;
		int m_length;

		Reflection::PropertyInfo^ m_valueProperty;
		Reflection::PropertyInfo^ m_isNullProperty;
		Reflection::PropertyInfo^ m_nullProperty;
		Reflection::FieldInfo^ m_nullField;
		
		Reflection::ConstructorInfo^ m_constructorMethod;
		Reflection::MethodInfo^ m_parseMethod;

		Reflection::MethodInfo^ m_binaryReaderMethod;
		Reflection::MethodInfo^ m_binaryWriterMethod;

		SqlObject(Type^ sqlDataType, String^ sqlType, int precision, int scale, int length, bool isSqlVariantCompatible);

		Object^ construct(IO::BinaryReader^ reader);
		Object^ construct(Object^ value);
		Object^ construct(Object^ value, Type^ sqlDataType);
		Object^ construct(Object^ value, Type^ type, Reflection::ConstructorInfo^ constructorMethod);

		static TypeDictionary^ getSqlVariantDataTypes();
		static TypeDictionary^ getSqlOtherDataTypes();

		static TypeIndex^ getSqlTypeIndex();
		static TypeStringIndex^ getSqlTypeStringIndex();
		static IndexOfTypes^ getSqlIndexOfTypes();

		static RegexReplacementDictionary^ getSqlTypeStandardisationRegex();
		static String^ applyRegexReplacements(String^ value, RegexReplacementArray^ replacements);
		static SqlObject^ tryCreate(String^ sqlDataType, String^ sqlType, int precision, int scale, int length);

		static SqlTypes::SqlXml^ toSqlXml(String^ value);

		static MethodDictionary^ getBinaryReaderMethods();
		static MethodDictionary^ getIndependentBinaryReaders();

		static Reflection::PropertyInfo^ getValueProperty(Type^ type);
		static Reflection::PropertyInfo^ getIsNullProperty(Type^ type);
		static Reflection::PropertyInfo^ getNullProperty(Type^ type);
		static Reflection::FieldInfo^ getNullField(Type^ type);

		static Reflection::ConstructorInfo^ getConstructorMethod(Type^ type, Reflection::PropertyInfo^ valueProperty);
		static Reflection::ConstructorInfo^ getConstructorMethod(Type^ type);
		static Reflection::ConstructorInfo^ getNoArgsConstructorMethod(Type^ type);

		static Reflection::MethodInfo^ getParseMethod(Type^ type);

		static Reflection::MethodInfo^ getBinaryReaderMethod(Type^ type);

		static Reflection::MethodInfo^ getBinaryWriterMethod(Type^ type);
		static Reflection::MethodInfo^ getBinaryWriterMethod(Reflection::PropertyInfo^ propertyType);

	public:
		static initonly String^ SqlVariant = "sql_variant";

		/**
		 * Gets the value of the object it's related .NET data type
		 *
		 */
		virtual Object^ GetValue(Object^ instance);

		/**
		 * Checks whether an object value is a null
		 *
		 */
		virtual bool IsNull(Object^ instance);

		/**
		 * Parses a string value to return an object in the Sql data type
		 *
		 * @param value The value to te parsed
		 *
		 */
		virtual Object^ Parse(String^ value);

		/**
		 * Converts an instance of the object into a string with type prefix
		 *
		 */
		virtual String^ ConvertToString(Object^ instance);

		/**
		 * Converts an instance of the object into a string with/without type prefix
		 *
		 */
		virtual String^ ConvertToString(Object^ instance, bool includeTypePrefix);

		/**
		 * Returns whether the value is valid for the data type
		 *
		 */
		virtual bool IsValueValid(Object^ value);

		/**
		 * Reads the .NET value of the object from a BinaryReader
		 *
		 */
		virtual Object^ Read(IO::BinaryReader^ reader);

		/**
		 * Writes the .NET value of the object to a BinaryWriter
		 *
		 */
		virtual void Write(Object^ value, IO::BinaryWriter^ writer);


		/// Returns the Sql data type to which the instance relates
		property Type^ SqlDataType {
			virtual Type^ get();
		}

		/// Returns a string representing the Sql data type
		property String^ SqlType {
			virtual String^ get();
		}

		/// Returns the precision associated with the type
		property int Precision {
			virtual int get();
		}

		/// Returns the scale associated with the type
		property int Scale {
			virtual int get();
		}

		/// Returns the length associated with the type
		property int Length {
			virtual int get();
		}

		/// Returns a appropriately typed null value
		property Object^ Null {
			virtual Object^ get();
		}

		/// Returns whether the type is sql_variant compatible or not
		property bool IsSqlVariantCompatible {
			virtual bool get();
		}

		/// Returns whether the type is numeric of not
		property bool IsNumeric {
			virtual bool get();
		}

		/**
		 * Creates a SqlObject for SqlString
		 *
		 */
		static SqlObject^ Create();

		/**
		 * Creates a SqlObject for the given string representation of the Sql data type
		 *
		 */
		static SqlObject^ Create(String^ sqlType);

		/**
		 * Creates a SqlObject for a sql data type
		 *
		 */
		static SqlObject^ Create(Type^ sqlDataType);
	};
}}}

#endif // _SMART_SQLCLR_INTERNAL__SQLOBJECT_H_
