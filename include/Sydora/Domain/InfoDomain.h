#pragma once

#include <iostream>
#include <vector>
#include <EPP/Request.h>
#include <EPP/Command.h>

#include <EPP/XML/XmlParserWrapper.h>
#include <EPP/XML/XmlHelpers.h>

#include <Sydora/Domain/Domain.h>

#include <Poco/DOM/AutoPtr.h>
#include <Poco/DOM/Document.h>
#include <Poco/DOM/Element.h>
using namespace Poco::XML;

namespace Sydora
{
	class DomainInfo
	{
	public:

		DomainInfo(XmlParserWrapper& parser);

		std::string name;
		std::string roid;
		std::vector<domainStatus> status;
		
		std::string registrant;

		std::string clID; // registrátor - súčasný
		std::string crID; // registrátor - reg domény

		DomainNSList ns;

		std::string crDate;
		std::string upDate;
		std::string exDate;
		std::string trDate;

		std::string authInfoPw;

		std::vector<DomainContact> contacts;

	private:
	};

	namespace Commands
	{
		class InfoDomain: public EPP::Command
		{
		protected:
			std::vector<std::string> domains;

		public:
			inline InfoDomain()
			{}

			inline InfoDomain(const std::string& domainName)
			{
				domains.push_back(domainName);
			}

			inline InfoDomain(const std::vector<std::string>& domains)
			: domains(domains)
			{}

			virtual AutoPtr<Element> toXML(AutoPtr<Document> pDoc) const;
		};
	}
}

