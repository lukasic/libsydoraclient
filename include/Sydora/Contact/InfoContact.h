#pragma once

#include <vector>

#include <EPP/Command.h>
#include <EPP/XML/XmlHelpers.h>

#include <Poco/DOM/AutoPtr.h>
#include <Poco/DOM/Document.h>
#include <Poco/DOM/Element.h>

using namespace Poco::XML;

namespace Sydora
{
	namespace Commands
	{
		class InfoContact: public EPP::Command
		{
		protected:
			std::vector<std::string> contacts;

		public:
			inline InfoContact()
			{}

			inline InfoContact(const std::string& contactID)
			{
				contacts.push_back(contactID);
			}

			inline InfoContact(const std::vector<std::string>& contacts)
			: contacts(contacts)
			{}

			virtual inline AutoPtr<Element> toXML(AutoPtr<Document> pDoc) const
			{
				AutoPtr<Element> info = pDoc->createElement("info");
				AutoPtr<Element> contactInfo = pDoc->createElement("contact:info");
				contactInfo->setAttribute(
					"xmlns:contact", "urn:ietf:params:xml:ns:contact-1.0");

				for (int i = 0; i < contacts.size(); i++)
					contactInfo->appendChild(CreateTextElement(pDoc,
						"contact:id", contacts[i]));

				info->appendChild(contactInfo);
				return info;
			}
		};
	}
}