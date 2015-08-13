
#include <Smart/SqlClr/Aggregates/Internal/DictionaryCreatorImpl.hpp>

#include <Smart/SqlClr/Types/Dictionary.hpp>

namespace Smart { namespace SqlClr { namespace Aggregates { namespace Internal {
	using namespace System;
	using namespace System::Data::SqlTypes;
	using namespace Microsoft::SqlServer::Server;

	DictionaryCreatorImpl::DictionaryCreatorImpl(bool compressed, bool sorted) : m_compressed(compressed), m_sorted(sorted) { }
	
	void DictionaryCreatorImpl::Init() {
		m_result = nullptr;
	}

	void DictionaryCreatorImpl::Accumulate(Object^ key, Object^ value) {
		if (key) {
			if (!m_result) {
				Type^ valueType = nullptr;
				if (value) {
					valueType = value->GetType();
				}

				if (m_sorted) {
					m_result = Types::Dictionary::NewSortedOfType(key->GetType(), valueType);
				} else {
					m_result = Types::Dictionary::NewOfType(key->GetType(), valueType);
				}

				m_result->SetCompression(m_compressed);
			}

			m_result->AddEntry(safe_cast<IComparable^>(key), value);
		}
	}

	void DictionaryCreatorImpl::Merge(DictionaryCreatorImpl^ value) {
		if (m_result && value->m_result) {
			m_result->AddFromDictionary(value->m_result);
		} else if (!m_result && value->m_result) {
			m_result = value->m_result;
		}
	}

	Types::Dictionary^ DictionaryCreatorImpl::Terminate() {
		return m_result ? m_result : (m_sorted ? Types::Dictionary::NewSortedOf("string", "sql_variant") : Types::Dictionary::New());
	}

	void DictionaryCreatorImpl::Read(IO::BinaryReader^ reader) {
		if (!reader->ReadByte()) {
			m_result = gcnew Types::Dictionary;
			m_result->Read(reader);
		} else {
			m_result = nullptr;
		}
	}

	void DictionaryCreatorImpl::Write(IO::BinaryWriter^ writer) {
		if (m_result) {
			writer->Write(Convert::ToByte(0));
			m_result->Write(writer);
		} else {
			writer->Write(Convert::ToByte(1));
		}
	}
}}}}
