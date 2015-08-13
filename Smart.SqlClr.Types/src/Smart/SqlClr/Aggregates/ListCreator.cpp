
#include <Smart/SqlClr/Aggregates/ListCreator.hpp>

#include <Smart/SqlClr/Aggregates/Internal/ListCreatorImpl.hpp>
#include <Smart/SqlClr/Types/List.hpp>

namespace Smart { namespace SqlClr { namespace Aggregates {
	using namespace System;
	using namespace System::Data::SqlTypes;
	using namespace Microsoft::SqlServer::Server;
	using namespace Smart::SqlClr::Aggregates::Internal;

	ListCreator::ListCreator() : m_impl(gcnew ListCreatorImpl(Compressed, Sorted)) { }

	void ListCreator::Init() {
		if (!m_impl) {
			m_impl = gcnew ListCreatorImpl(Compressed, Sorted);
		}

		m_impl->Init();
	}

	void ListCreator::Accumulate(Object^ value) {
		m_impl->Accumulate(value);
	}

	void ListCreator::Merge(ListCreator^ value) {
		m_impl->Merge(value->m_impl);
	}

	Types::List^ ListCreator::Terminate() {
		return m_impl->Terminate();
	}

	void ListCreator::Read(IO::BinaryReader^ reader) {
		m_impl->Read(reader);
	}

	void ListCreator::Write(IO::BinaryWriter^ writer) {
		m_impl->Write(writer);
	}
}}}
