
#ifndef _SMART_SQLCLR__SMARTSQLCLREXCEPTION_HPP_
#define _SMART_SQLCLR__SMARTSQLCLREXCEPTION_HPP_

#using <System.dll>

namespace Smart { namespace SqlClr {
	using namespace System;
	using namespace System::Runtime::Serialization;

	/**
	 * @class SmartSqlClrException
	 *
	 * @brief Used to raise SqlClr exceptions
	 *
	 * Exception class for raising exceptions back to Sql Clients
	 * when raised by the library.
	 *
	 * @author Muhmud Ahmad
	 *
	 */
	public ref class SmartSqlClrException : public ApplicationException {
	public:
		/**
		 * Constructor
		 *
		 */
		SmartSqlClrException();

		/**
		 * Constructor for exception message
		 *
		 */
		SmartSqlClrException(String^ message);

		/**
		 * Constructor for serialization data
		 *
		 */
		SmartSqlClrException(SerializationInfo^ info, StreamingContext context);
	};
}}

#endif // _SMART_SQLCLR__SMARTSQLCLREXCEPTION_HPP_
