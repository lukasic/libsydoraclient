#include <Sydora/Contact/CreateContact.h>

#include <vector>
#include <list>
#include <string>

#include <EPP/Command.h>
#include <Sydora/Contact/Contact.h>

#include <Poco/DOM/AutoPtr.h>
#include <Poco/DOM/Document.h>
#include <Poco/DOM/Element.h>

using Sydora::Commands::CreateContact;

std::list<XmlSerializable::Ptr> CreateContact::getExtensions() const
{
	std::list<XmlSerializable::Ptr> v;
	v.push_back(contact.skContactIdent.clone());
	return v;
}

AutoPtr<Element> CreateContact::toXML(AutoPtr<Document> doc) const
{
	AutoPtr<Element> create = doc->createElement("create");
	AutoPtr<Element> e = AddElement(doc, create, "contact:create");
	e->setAttribute("xmlns:contact", "urn:ietf:params:xml:ns:contact-1.0");

	AddElement(doc, e, "contact:id", contact.id);
	AddElement(doc, e, contact.postalInfo.toXML(doc));
	
	if (contact.voice != "")
		AddElement(doc, e, "contact:voice", contact.voice);

	if (contact.fax != "")
		AddElement(doc, e, "contact:fax", contact.fax);
	
	AddElement(doc, e, "contact:email", contact.email);

	auto authInfo = AddElement(doc, e, "contact:authInfo");
	AddElement(doc, authInfo, "contact:pw", contact.authInfoPw);

	if (contact.disclose.isSet())
		AddElement(doc, e, contact.disclose.toXML(doc));

	if (contact.nodisclose.isSet())
		AddElement(doc, e, contact.nodisclose.toXML(doc));

	return create;
}

