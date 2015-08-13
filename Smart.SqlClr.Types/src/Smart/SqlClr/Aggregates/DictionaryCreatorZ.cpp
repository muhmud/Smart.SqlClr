
#include <Smart/SqlClr/Aggregates/DictionaryCreatorZ.hpp>

#include <Smart/SqlClr/Aggregates/Internal/DictionaryCreatorImpl.hpp>
#include <Smart/SqlClr/Types/Dictionary.hpp>

namespace Smart { namespace SqlClr { namespace Aggregates {
	using namespace System;
	using namespace System::Data::SqlTypes;
	using namespace Microsoft::SqlServer::Server;
	using namespace Smart::SqlClr::Aggregates::Internal;

	DictionaryCreatorZ::DictionaryCreatorZ() : m_impl(gcnew DictionaryCreatorImpl(Compressed, Sorted)) {	}

	void DictionaryCreatorZ::Init() {
		if (!m_impl) {
			m_impl = gcnew DictionaryCreatorImpl(Compressed, Sorted);
		}

		m_impl->Init();
	}

	void DictionaryCreatorZ::Accumulate(Object^ key, Object^ value) {
		m_impl->Accumulate(key, value);
	}

	void DictionaryCreatorZ::Merge(DictionaryCreatorZ^ value) {
		m_impl->Merge(value->m_impl);
	}

	Types::Dictionary^ DictionaryCreatorZ::Terminate() {
		return m_impl->Terminate();
	}

	void DictionaryCreatorZ::Read(IO::BinaryReader^ reader) {
		m_impl->Read(reader);
	}

	void DictionaryCreatorZ::Write(IO::BinaryWriter^ writer) {
		m_impl->Write(writer);
	}
}}}
