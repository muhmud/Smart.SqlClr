
#include <Smart/SqlClr/Aggregates/SortedListCreator.hpp>

#include <Smart/SqlClr/Aggregates/Internal/ListCreatorImpl.hpp>
#include <Smart/SqlClr/Types/List.hpp>

namespace Smart { namespace SqlClr { namespace Aggregates {
	using namespace System;
	using namespace System::Data::SqlTypes;
	using namespace Microsoft::SqlServer::Server;
	using namespace Smart::SqlClr::Aggregates::Internal;

	SortedListCreator::SortedListCreator() : m_impl(gcnew ListCreatorImpl(Compressed, Sorted)) { }

	void SortedListCreator::Init() {
		if (!m_impl) {
			m_impl = gcnew ListCreatorImpl(Compressed, Sorted);
		}

		m_impl->Init();
	}

	void SortedListCreator::Accumulate(Object^ value) {
		m_impl->Accumulate(value);
	}

	void SortedListCreator::Merge(SortedListCreator^ value) {
		m_impl->Merge(value->m_impl);
	}

	Types::List^ SortedListCreator::Terminate() {
		return m_impl->Terminate();
	}

	void SortedListCreator::Read(IO::BinaryReader^ reader) {
		m_impl->Read(reader);
	}

	void SortedListCreator::Write(IO::BinaryWriter^ writer) {
		m_impl->Write(writer);
	}
}}}
