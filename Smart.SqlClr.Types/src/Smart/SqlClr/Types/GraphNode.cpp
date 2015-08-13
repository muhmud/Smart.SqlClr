
#include <Smart/SqlClr/Types/GraphNode.hpp>

#include <Smart/SqlClr/Types/Variant.hpp>

#using <System.Xml.dll>

namespace Smart { namespace SqlClr { namespace Types {
	GraphNode^ GraphNode::create() {
		GraphNode^ node = gcnew GraphNode;
		node->m_isNull = 1;

		return node;
	}

	GraphNode^ GraphNode::New(SqlHierarchyId nodeID, Variant^ value) {
		GraphNode^ node = gcnew GraphNode;
		node->m_graphNode = SqlGraphNode(nodeID, value);

		return node;
	}

	SqlHierarchyId GraphNode::NodeID::get() {
		return !IsNull ? m_graphNode.Key : SqlHierarchyId::Null;
	}

	Variant^ GraphNode::Value::get() {
		return !IsNull ? m_graphNode.Value : Variant::Null;
	}

	bool GraphNode::IsNull::get() {
		return m_isNull == 1;
	}

	GraphNode^ GraphNode::Null::get() {
		return NullGraphNode;
	}

	String^ GraphNode::ToString() {
		if (!IsNull) {
			Text::StringBuilder^ builder = gcnew Text::StringBuilder;

			Xml::XmlWriterSettings^ settings = gcnew Xml::XmlWriterSettings;
			settings->OmitXmlDeclaration = true;

			Xml::XmlWriter^ writer = Xml::XmlWriter::Create(builder, settings);

			writer->WriteStartElement(XML_ROOT_ELEMENT);

			writer->WriteStartElement(XML_KEY_ELEMENT);
			writer->WriteString(m_graphNode.Key.ToString());
			writer->WriteEndElement();

			writer->WriteStartElement(XML_VALUE_ELEMENT);
			writer->WriteRaw(m_graphNode.Value->ToString());
			writer->WriteEndElement();

			writer->WriteEndElement();
			writer->Flush();

			return builder->ToString();
		}

		return nullptr;
	}

	GraphNode^ GraphNode::Parse(SqlString sqlValue) {
		String^ value = sqlValue.Value;
		Xml::XmlReader^ reader = Xml::XmlReader::Create(gcnew IO::StringReader(value));

		GraphNode^ graphNode = gcnew GraphNode;
		if (reader->IsStartElement(XML_ROOT_ELEMENT)) {
			if (reader->ReadToFollowing(XML_KEY_ELEMENT)) {
				SqlHierarchyId nodeID = SqlHierarchyId::Parse(reader->ReadString());

				if (reader->ReadToFollowing(XML_VALUE_ELEMENT)) {
					Variant^ value = Variant::Parse(reader->ReadInnerXml());

					graphNode->m_graphNode = SqlGraphNode(nodeID, value);
					return graphNode;
				}
			}
		}

		return Null;
	}

	void GraphNode::Read(IO::BinaryReader^ reader) {
		m_isNull = reader->ReadByte();
		if (!m_isNull) {
			SqlHierarchyId nodeID;
			nodeID.Read(reader);

			Variant^ value = gcnew Variant;
			value->Read(reader);

			m_graphNode = SqlGraphNode(nodeID, value);
		}
	}

	void GraphNode::Write(IO::BinaryWriter^ writer) {
		writer->Write(m_isNull);
		if (!m_isNull) {
			m_graphNode.Key.Write(writer);
			m_graphNode.Value->Write(writer);
		}
	}
}}}
