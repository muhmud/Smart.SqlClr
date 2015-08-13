
#include <Smart/SqlClr/Utils/Binary.hpp>

#include <Smart/SqlClr/SmartSqlClrException.hpp>

namespace Smart { namespace SqlClr { namespace Utils {
	using namespace System;
	using namespace System::Data;

	SqlTypes::SqlBinary Binary::HexToBytes(System::String^ value) {
		String^ val = value->StartsWith("0x") ? value->Substring(2) : value;
		cli::array<Byte>^ output = gcnew cli::array<Byte>((val->Length / 2));
		for (int i = 0; i < val->Length; i += 2) {
			Byte byteHigh = safe_cast<Byte>(val[i]);
			Byte byteLow = safe_cast<Byte>(val[i + 1]);
			output[i / 2] = ((byteHigh - 48 - ((byteHigh >> 4) ^ 0x3)) << 4) | ((byteLow - 48 - ((byteLow >> 4) ^ 0x3)) & 0xF);
		}

		return SqlTypes::SqlBinary(output);
	}

	SqlTypes::SqlString Binary::ToHex(SqlTypes::SqlBinary value) {
		if (!value.IsNull) {
			if (value.Length) {
				cli::array<Byte>^ output = gcnew cli::array<Byte>(value.Length * 2 + 2);
				output[0] = '0';output[1] = 'x';
				for (int i = 0; i < value.Value->Length; ++i) {
					Byte byte = value[i];
					Byte nibble = (byte >> 4);
					Byte nibbleUp = (nibble + 6) >> 4;
					output[i * 2 + 2] = nibble + 48 + (nibbleUp | (nibbleUp << 1) | (nibbleUp << 2));

					nibble = byte & 0xF;
					nibbleUp = (nibble + 6) >> 4;
					output[i * 2 + 1 + 2] = nibble + 48 + (nibbleUp | (nibbleUp << 1) | (nibbleUp << 2));
				}

				return SqlTypes::SqlString(Text::Encoding::ASCII->GetString(output));
			}

			return SqlTypes::SqlString(System::String::Empty);
		}

		return SqlTypes::SqlString::Null;
	}

	SqlTypes::SqlBinary Binary::ToBytes(SqlTypes::SqlString value) {
		if (!value.IsNull) {
			if (!String::IsNullOrEmpty(value.Value)) {
				String^ val = value.Value->StartsWith("0x") ? value.Value->Substring(2) : value.Value;
				if (!Text::RegularExpressions::Regex::IsMatch(val, "^[0-9A-F].*$")) {
					throw gcnew SmartSqlClrException("Invalid String value specified. The value must be a hex string");
				}

				if (val->Length % 2) {
					val = "0" + val;
				}

				return HexToBytes(val);
			}

			return SqlTypes::SqlBinary(gcnew cli::array<Byte> { 0 });
		}

		return SqlTypes::SqlBinary::Null;
	}
}}}
