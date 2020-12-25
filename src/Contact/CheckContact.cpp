#include <Sydora/Contact/CheckContact.h>

#include <Poco/DOM/AutoPtr.h>
#include <Poco/DOM/Document.h>
#include <Poco/DOM/Element.h>

using Sydora::Commands::CheckContact;


AutoPtr<Element> CheckContact::toXML(AutoPtr<Document> pDoc) const
{
	AutoPtr<Element> check = pDoc->createElement("check");
	AutoPtr<Element> checkContact = pDoc->createElement("contact:check");
	checkContact->setAttribute("xmlns:contact", "urn:ietf:params:xml:ns:contact-1.0");

	for (int i = 0; i < contacts.size(); i++)
		checkContact->appendChild(CreateTextElement(pDoc,
			"contact:id", contacts[i]));

	check->appendChild(checkContact);
	return check;
}

