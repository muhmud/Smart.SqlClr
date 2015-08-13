
#include <Smart/SqlClr/Aggregates/SortedListCreatorZ.hpp>

#include <Smart/SqlClr/Aggregates/Internal/ListCreatorImpl.hpp>
#include <Smart/SqlClr/Types/List.hpp>

namespace Smart { namespace SqlClr { namespace Aggregates {
	using namespace System;
	using namespace System::Data::SqlTypes;
	using namespace Microsoft::SqlServer::Server;
	using namespace Smart::SqlClr::Aggregates::Internal;

	SortedListCreatorZ::SortedListCreatorZ() : m_impl(gcnew ListCreatorImpl(Compressed, Sorted)) { }

	void SortedListCreatorZ::Init() {
		if (!m_impl) {
			m_impl = gcnew ListCreatorImpl(Compressed, Sorted);
		}

		m_impl->Init();
	}

	void SortedListCreatorZ::Accumulate(Object^ value) {
		m_impl->Accumulate(value);
	}

	void SortedListCreatorZ::Merge(SortedListCreatorZ^ value) {
		m_impl->Merge(value->m_impl);
	}

	Types::List^ SortedListCreatorZ::Terminate() {
		return m_impl->Terminate();
	}

	void SortedListCreatorZ::Read(IO::BinaryReader^ reader) {
		m_impl->Read(reader);
	}

	void SortedListCreatorZ::Write(IO::BinaryWriter^ writer) {
		m_impl->Write(writer);
	}
}}}
