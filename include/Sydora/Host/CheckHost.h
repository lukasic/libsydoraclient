#pragma once

#include <vector>
#include <string>
#include <EPP/Command.h>

#include <Poco/DOM/AutoPtr.h>
#include <Poco/DOM/Document.h>
#include <Poco/DOM/Element.h>

namespace Sydora
{
	namespace Commands
	{
		class CheckHost: public EPP::Command
		{
		protected:
			std::vector<std::string> hosts;

		public:
			inline CheckHost()
			{}
			
			inline CheckHost(const std::string& hostName)
			{
				hosts.push_back(hostName);
			}

			inline CheckHost(const std::vector<std::string>& hosts)
			: hosts(hosts)
			{}

			virtual inline AutoPtr<Element> toXML(AutoPtr<Document> pDoc) const
			{
				AutoPtr<Element> check = pDoc->createElement("check");
				AutoPtr<Element> checkHost = pDoc->createElement("host:check");
				checkHost->setAttribute("xmlns:host", "urn:ietf:params:xml:ns:host-1.0");

				for (int i = 0; i < hosts.size(); i++)
					checkHost->appendChild(CreateTextElement(pDoc,
						"host:name", hosts[i]));

				check->appendChild(checkHost);
				return check;
			}

		};
	}
}