#pragma once

#include <vector>
#include <string>
#include <EPP/Command.h>

#include <Poco/DOM/AutoPtr.h>
#include <Poco/DOM/Document.h>
#include <Poco/DOM/Element.h>

#include <EPP/XML/XmlParserWrapper.h>

namespace Sydora
{
	struct HostCreate
	{
		inline HostCreate() {}

		inline HostCreate(const std::string& name): name(name)
		{}

		inline bool withGlue() const {
			return ipv4.size() > 0 || ipv6.size() > 0;
		}

		std::string name;

		std::vector<std::string> ipv4;
		std::vector<std::string> ipv6;
	};

	struct HostCreated
	{
		std::string name;
		std::string crDate;

		inline HostCreated(XmlParserWrapper& parser)
		{
			name = parser.getInnerTextByPath("host:name");
			crDate = parser.getInnerTextByPath("host:crDate");
		}
	};

	namespace Commands
	{
		class CreateHost : public EPP::Command
		{
		protected:
			HostCreate hc;

		public:
			inline CreateHost()
			{}

			inline CreateHost(const HostCreate& hc)
				: hc(hc)
			{}

			virtual inline AutoPtr<Element> toXML(AutoPtr<Document> pDoc) const
			{
				AutoPtr<Element> create = pDoc->createElement("create");
				AutoPtr<Element> createHost = AddElement(pDoc, create, "host:create");
				createHost->setAttribute("xmlns:host", "urn:ietf:params:xml:ns:host-1.0");

				AddElement(pDoc, createHost, "host:name", hc.name);
				
				for each (auto ip in hc.ipv4)
					AddElement(pDoc, createHost, "host:addr", ip)
						->setAttribute("ip", "v4");

				for each (auto ip in hc.ipv6)
					AddElement(pDoc, createHost, "host:addr", ip)
						->setAttribute("ip", "v6");

				return create;
			}

		};
	}
}