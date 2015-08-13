
#include <Smart/SqlClr/Aggregates/SortedDictionaryCreatorZ.hpp>

#include <Smart/SqlClr/Aggregates/Internal/DictionaryCreatorImpl.hpp>
#include <Smart/SqlClr/Types/Dictionary.hpp>

namespace Smart { namespace SqlClr { namespace Aggregates {
	using namespace System;
	using namespace System::Data::SqlTypes;
	using namespace Microsoft::SqlServer::Server;
	using namespace Smart::SqlClr::Aggregates::Internal;

	SortedDictionaryCreatorZ::SortedDictionaryCreatorZ() : m_impl(gcnew DictionaryCreatorImpl(Compressed, Sorted)) { }

	void SortedDictionaryCreatorZ::Init() {
		if (!m_impl) {
			m_impl = gcnew DictionaryCreatorImpl(Compressed, Sorted);
		}

		m_impl->Init();
	}

	void SortedDictionaryCreatorZ::Accumulate(Object^ key, Object^ value) {
		m_impl->Accumulate(key, value);
	}

	void SortedDictionaryCreatorZ::Merge(SortedDictionaryCreatorZ^ value) {
		m_impl->Merge(value->m_impl);
	}

	Types::Dictionary^ SortedDictionaryCreatorZ::Terminate() {
		return m_impl->Terminate();
	}

	void SortedDictionaryCreatorZ::Read(IO::BinaryReader^ reader) {
		m_impl->Read(reader);
	}

	void SortedDictionaryCreatorZ::Write(IO::BinaryWriter^ writer) {
		m_impl->Write(writer);
	}
}}}
