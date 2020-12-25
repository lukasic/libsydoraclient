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
		class CheckDomain: public EPP::Command
		{
		protected:
			std::vector<std::string> domains;

		public:
			inline CheckDomain()
			{}
			
			inline CheckDomain(const std::string& domainName)
			{
				domains.push_back(domainName);
			}

			inline CheckDomain(const std::vector<std::string>& domains)
			: domains(domains)
			{}

			virtual AutoPtr<Element> toXML(AutoPtr<Document> pDoc) const;

		};
	}
}