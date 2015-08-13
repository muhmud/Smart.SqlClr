
#include <Smart/SqlClr/Aggregates/ListCreatorZ.hpp>

#include <Smart/SqlClr/Aggregates/Internal/ListCreatorImpl.hpp>
#include <Smart/SqlClr/Types/List.hpp>

namespace Smart { namespace SqlClr { namespace Aggregates {
	using namespace System;
	using namespace System::Data::SqlTypes;
	using namespace Microsoft::SqlServer::Server;
	using namespace Smart::SqlClr::Aggregates::Internal;

	ListCreatorZ::ListCreatorZ() : m_impl(gcnew ListCreatorImpl(Compressed, Sorted)) { }

	void ListCreatorZ::Init() {
		if (!m_impl) {
			m_impl = gcnew ListCreatorImpl(Compressed, Sorted);
		}

		m_impl->Init();
	}

	void ListCreatorZ::Accumulate(Object^ value) {
		m_impl->Accumulate(value);
	}

	void ListCreatorZ::Merge(ListCreatorZ^ value) {
		m_impl->Merge(value->m_impl);
	}

	Types::List^ ListCreatorZ::Terminate() {
		return m_impl->Terminate();
	}

	void ListCreatorZ::Read(IO::BinaryReader^ reader) {
		m_impl->Read(reader);
	}

	void ListCreatorZ::Write(IO::BinaryWriter^ writer) {
		m_impl->Write(writer);
	}
}}}
