
#include <Smart/SqlClr/SmartSqlClrException.hpp>

namespace Smart { namespace SqlClr {
	using namespace System;
	using namespace System::Runtime::Serialization;

	SmartSqlClrException::SmartSqlClrException() : ApplicationException("An unknown error has occurred") { }

	SmartSqlClrException::SmartSqlClrException(String^ message) : ApplicationException(message) { }

	SmartSqlClrException::SmartSqlClrException(SerializationInfo^ info, StreamingContext context) : ApplicationException(info, context) { }
}}
