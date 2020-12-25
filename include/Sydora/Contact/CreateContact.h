#pragma once

#include <list>

#include <EPP/Command.h>
#include <Sydora/Contact/Contact.h>

#include <Poco/DOM/AutoPtr.h>
#include <Poco/DOM/Document.h>
#include <Poco/DOM/Element.h>

namespace Sydora
{
	namespace Commands
	{
		class CreateContact: public EPP::Command
		{
		protected:
			ContactCreate contact;

		public:
			inline CreateContact(const ContactCreate& contact)
			: contact(contact)
			{}

			virtual std::list<XmlSerializable::Ptr> getExtensions() const;
			
			virtual AutoPtr<Element> toXML(AutoPtr<Document> doc) const;

		};
	}
}