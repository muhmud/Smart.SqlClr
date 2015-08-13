
#ifndef _SMART_SQLCLR_UTILS__BINARY_HPP_
#define _SMART_SQLCLR_UTILS__BINARY_HPP_

#using <System.dll>
#using <System.Data.dll>

namespace Smart { namespace SqlClr { namespace Utils {
	using namespace System;
	using namespace System::Data;

	/**
	 * @class Binary
	 *
	 * @brief Utilities for working with binary data
	 *
	 * Various utilities for working with binary data
	 * within SQL Server.
	 *
	 * @author Muhmud Ahmad
	 *
	 */
	public ref class Binary {
	internal:
		static SqlTypes::SqlBinary HexToBytes(System::String^ value);

	public:
		/**
		 * Converts a binary value to a hex string
		 *
		 */
		static SqlTypes::SqlString ToHex(SqlTypes::SqlBinary value);

		/**
		 * Converts a hex string to binary
		 *
		 */
		static SqlTypes::SqlBinary ToBytes(SqlTypes::SqlString value);
	};
}}}

#endif // _SMART_SQLCLR_UTILS__BINARY_HPP_
