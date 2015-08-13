
#include <Smart/SqlClr/Aggregates/SortedDictionaryCreator.hpp>

#include <Smart/SqlClr/Aggregates/Internal/DictionaryCreatorImpl.hpp>
#include <Smart/SqlClr/Types/Dictionary.hpp>

namespace Smart { namespace SqlClr { namespace Aggregates {
	using namespace System;
	using namespace System::Data::SqlTypes;
	using namespace Microsoft::SqlServer::Server;
	using namespace Smart::SqlClr::Aggregates::Internal;

	SortedDictionaryCreator::SortedDictionaryCreator() : m_impl(gcnew DictionaryCreatorImpl(Compressed, Sorted)) { }

	void SortedDictionaryCreator::Init() {
		if (!m_impl) {
			m_impl = gcnew DictionaryCreatorImpl(Compressed, Sorted);
		}

		m_impl->Init();
	}

	void SortedDictionaryCreator::Accumulate(Object^ key, Object^ value) {
		m_impl->Accumulate(key, value);
	}

	void SortedDictionaryCreator::Merge(SortedDictionaryCreator^ value) {
		m_impl->Merge(value->m_impl);
	}

	Types::Dictionary^ SortedDictionaryCreator::Terminate() {
		return m_impl->Terminate();
	}

	void SortedDictionaryCreator::Read(IO::BinaryReader^ reader) {
		m_impl->Read(reader);
	}

	void SortedDictionaryCreator::Write(IO::BinaryWriter^ writer) {
		m_impl->Write(writer);
	}
}}}
