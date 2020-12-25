#include <Sydora/Domain/CreateDomain.h>

#include <vector>
#include <string>
#include <EPP/XML/XmlParserWrapper.h>
#include <EPP/Command.h>

#include <Poco/DOM/AutoPtr.h>
#include <Poco/DOM/Document.h>
#include <Poco/DOM/Element.h>
#include <Poco/UUIDGenerator.h>
#include <Poco/NumberFormatter.h>


AutoPtr<Element> Sydora::Commands::CreateDomain::toXML(AutoPtr<Document> pDoc) const
{
	AutoPtr<Element> create = pDoc->createElement("create");
	AutoPtr<Element> createDomain = AddElement(pDoc, create, "domain:create");
	createDomain->setAttribute("xmlns:domain", "urn:ietf:params:xml:ns:domain-1.0");

	AddElement(pDoc, createDomain, "domain:name", domain.name);
	AddElement(pDoc, createDomain, "domain:period", domain.period)
		->setAttribute("unit", "y");

	if (domain.ns.isSet())
		AddElement(pDoc, createDomain, domain.ns.toXML(pDoc));

	AddElement(pDoc, createDomain, "domain:registrant", domain.registrant);

	for (unsigned int i = 0; i < domain.contacts.size(); i++)
		AddElement(pDoc, createDomain, domain.contacts[i].toXML(pDoc));

	auto authInfo = AddElement(pDoc, createDomain, "domain:authInfo");
	AddElement(pDoc, authInfo, "domain:pw", domain.authInfoPw);

	return create;
}
