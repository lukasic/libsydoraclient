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
		class CheckContact: public EPP::Command
		{
		protected:
			std::vector<std::string> contacts;

		public:
			inline CheckContact()
			{}
			
			inline CheckContact(const std::string& contactName)
			{
				contacts.push_back(contactName);
			}

			inline CheckContact(const std::vector<std::string>& contacts)
			: contacts(contacts)
			{}

			virtual AutoPtr<Element> toXML(AutoPtr<Document> pDoc) const;

		};
	}
}