
#ifndef _SMART_SQLCLR_INTERNAL_INTERFACES__ISQLOBJECT_HPP_
#define _SMART_SQLCLR_INTERNAL_INTERFACES__ISQLOBJECT_HPP_

#using <System.dll>

namespace Smart { namespace SqlClr { namespace Internal { namespace Interfaces {
	using namespace System;

	/**
	 * @interface ISqlObject
	 *
	 * @brief Represents an Object with a Sql data type
	 *
	 * This interface can be useful when working with an object
	 * passed to a SqlClr method as a sql_variant.
	 *
	 * @author Muhmud Ahmad
	 *
	 */
	interface class ISqlObject {
	public:
		/**
		 * Gets the value of the object it's related .NET data type
		 *
		 */
		Object^ GetValue(Object^ instance);

		/**
		 * Checks whether an object value is a null
		 *
		 */
		bool IsNull(Object^ instance);

		/**
		 * Parses a string value to return an object in the Sql data type
		 *
		 * @param value The value to te parsed
		 *
		 */
		Object^ Parse(String^ value);

		/**
		 * Converts an instance of the object into a string with type prefix
		 *
		 */
		String^ ConvertToString(Object^ instance);

		/**
		 * Converts an instance of the object into a string with/without type prefix
		 *
		 */
		String^ ConvertToString(Object^ instance, bool includeTypePrefix);

		/**
		 * Returns whether the value is valid for the data type
		 *
		 */
		bool IsValueValid(Object^ value);

		/**
		 * Reads the .NET value of the object from a BinaryReader
		 *
		 */
		System::Object^ Read(IO::BinaryReader^ reader);

		/**
		 * Writes the .NET value of the object to a BinaryWriter
		 *
		 */
		void Write(Object^ value, IO::BinaryWriter^ writer);


		/// Returns the Sql data type to which the instance relates
		property Type^ SqlDataType {
			Type^ get();
		}

		/// Returns a string representing the Sql data type
		property String^ SqlType {
			String^ get();
		}

		/// Returns the precision associated with the type
		property int Precision {
			int get();
		}

		/// Returns the scale associated with the type
		property int Scale {
			int get();
		}

		/// Returns the length associated with the type
		property int Length {
			int get();
		}

		/// Returns a appropriately typed null value
		property Object^ Null {
			Object^ get();
		}

		/// Returns whether the type is sql_variant compatible or not
		property bool IsSqlVariantCompatible {
			bool get();
		}

		/// Returns whether the type is numeric of not
		property bool IsNumeric {
			bool get();
		}
	};
}}}}

#endif // _SMART_SQLCLR_INTERNAL_INTERFACES__ISQLOBJECT_HPP_
