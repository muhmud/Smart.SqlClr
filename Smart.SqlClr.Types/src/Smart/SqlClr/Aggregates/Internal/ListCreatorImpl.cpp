
#include <Smart/SqlClr/Aggregates/Internal/ListCreatorImpl.hpp>

#include <Smart/SqlClr/Types/List.hpp>

namespace Smart { namespace SqlClr { namespace Aggregates { namespace Internal {
	using namespace System;
	using namespace System::Data::SqlTypes;
	using namespace Microsoft::SqlServer::Server;

	ListCreatorImpl::ListCreatorImpl(bool compressed, bool sorted) : m_compressed(compressed), m_sorted(sorted) { }
	
	void ListCreatorImpl::Init() {
		m_result = nullptr;
	}

	void ListCreatorImpl::Accumulate(Object^ value) {
		if (value) {
			if (!m_result) {
				m_result = Types::List::NewOfType(value->GetType());
				m_result->SetCompression(m_compressed);
			}

			if (value) {
				m_result->AddItem(value);
			}
		}
	}

	void ListCreatorImpl::Merge(ListCreatorImpl^ value) {
		if (m_result && value->m_result) {
			m_result->AddFromList(value->m_result);
		} else if (!m_result && value->m_result) {
			m_result = value->m_result;
		}
	}

	Types::List^ ListCreatorImpl::Terminate() {
		if (m_result) {
			if (m_sorted) {
				m_result->Sort();
				return m_result;
			}

			return m_result;
		}

		return Types::List::New();
	}

	void ListCreatorImpl::Read(IO::BinaryReader^ reader) {
		if (!reader->ReadByte()) {
			m_result = gcnew Types::List;
			m_result->Read(reader);
		} else {
			m_result = nullptr;
		}
	}

	void ListCreatorImpl::Write(IO::BinaryWriter^ writer) {
		if (m_result) {
			writer->Write(Convert::ToByte(0));
			m_result->Write(writer);
		} else {
			writer->Write(Convert::ToByte(1));
		}
	}
}}}}
