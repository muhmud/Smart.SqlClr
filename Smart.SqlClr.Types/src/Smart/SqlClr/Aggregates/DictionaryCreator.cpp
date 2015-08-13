
#include <Smart/SqlClr/Aggregates/DictionaryCreator.hpp>

#include <Smart/SqlClr/Aggregates/Internal/DictionaryCreatorImpl.hpp>
#include <Smart/SqlClr/Types/Dictionary.hpp>

namespace Smart { namespace SqlClr { namespace Aggregates {
	using namespace System;
	using namespace System::Data::SqlTypes;
	using namespace Microsoft::SqlServer::Server;
	using namespace Smart::SqlClr::Aggregates::Internal;

	DictionaryCreator::DictionaryCreator() : m_impl(gcnew DictionaryCreatorImpl(Compressed, Sorted)) {	}

	void DictionaryCreator::Init() {
		if (!m_impl) {
			m_impl = gcnew DictionaryCreatorImpl(Compressed, Sorted);
		}

		m_impl->Init();
	}

	void DictionaryCreator::Accumulate(Object^ key, Object^ value) {
		m_impl->Accumulate(key, value);
	}

	void DictionaryCreator::Merge(DictionaryCreator^ value) {
		m_impl->Merge(value->m_impl);
	}

	Types::Dictionary^ DictionaryCreator::Terminate() {
		return m_impl->Terminate();
	}

	void DictionaryCreator::Read(IO::BinaryReader^ reader) {
		m_impl->Read(reader);
	}

	void DictionaryCreator::Write(IO::BinaryWriter^ writer) {
		m_impl->Write(writer);
	}
}}}
