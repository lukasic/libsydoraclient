#pragma once

#include <iostream>
#include <vector>
#include <set>

#include <EPP/Request.h>
#include <EPP/Command.h>
#include <EPP/XML/XmlHelpers.h>

#include <Poco/DOM/AutoPtr.h>
#include <Poco/DOM/Document.h>
#include <Poco/DOM/Element.h>
using namespace Poco::XML;

class HostInfo
{
public:

	inline HostInfo(XmlParserWrapper& parser)
	{
		name = parser.getInnerTextByPath("host:name");
		roid = parser.getInnerTextByPath("host:roid");
		
		clID = parser.getInnerTextByPath("host:clID");
		crID = parser.getInnerTextByPath("host:crID");

		crDate = parser.getInnerTextByPath("host:crDate");
		upDate = parser.getInnerTextByPath("host:upDate", "");
		trDate = parser.getInnerTextByPath("host:trDate", "");

		auto eaddrs = parser.getElementsByPath("host:addr");
		for each (auto ea in eaddrs)
		{
			if (ea->getAttribute("ip") == "v4")
				ipv4.insert(ea->innerText());

			else if (ea->getAttribute("ip") == "v6")
				ipv6.insert(ea->innerText());

			else
				throw EPP::Exception("Sydora::InfoHost parsing error.");
		}
	}

	std::string name;
	std::string roid;

	std::string clID; // registrátor - súčasný
	std::string crID; // registrátor - reg domény

	std::string crDate; // dátum registrácie
	std::string upDate; // dátum poslednej aktualizácie
	std::string trDate; // dátum expirácie

	std::set<std::string> ipv4;
	std::set<std::string> ipv6;

private:
};

namespace Sydora
{
	namespace Commands
	{
		class InfoHost: public EPP::Command
		{
		protected:
			std::vector<std::string> hosts;

		public:
			inline InfoHost()
			{}

			inline InfoHost(const std::string& hostName)
			{
				hosts.push_back(hostName);
			}

			inline InfoHost(const std::vector<std::string>& hosts)
			: hosts(hosts)
			{}

			virtual inline AutoPtr<Element> toXML(AutoPtr<Document> pDoc) const
			{
				AutoPtr<Element> info = pDoc->createElement("info");
				AutoPtr<Element> hostInfo = pDoc->createElement("host:info");
				hostInfo->setAttribute(
					"xmlns:host", "urn:ietf:params:xml:ns:host-1.0");

				for (int i = 0; i < hosts.size(); i++)
					hostInfo->appendChild(CreateTextElement(pDoc,
						"host:name", hosts[i]));

				info->appendChild(hostInfo);
				return info;
			}
		};
	}
}