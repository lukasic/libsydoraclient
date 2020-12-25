#include <Sydora/Domain/InfoDomain.h>

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

Sydora::DomainInfo::DomainInfo(XmlParserWrapper& parser)
{
	name = parser.getInnerTextByPath("domain:name");
	roid = parser.getInnerTextByPath("domain:roid");

	status = parser.getAttributesByPath<domainStatus>("domain:status", "s");

	clID = parser.getInnerTextByPath("domain:clID");
	crID = parser.getInnerTextByPath("domain:crID");
	crDate = parser.getInnerTextByPath("domain:crDate");
	upDate = parser.getInnerTextByPath("domain:upDate", "");
	exDate = parser.getInnerTextByPath("domain:exDate", "");
	trDate = parser.getInnerTextByPath("domain:trDate", "");

	registrant = parser.getInnerTextByPath("domain:registrant");

	auto nslist = parser.getInnerTextsByPath("domain:ns/domain:hostObj");
	for each (auto obj in nslist)
		ns.add(obj);

	auto eContacts = parser.getElementsByPath("domain:contact");
	for each (auto e in eContacts)
		contacts.push_back(DomainContact(e->innerText(), e->getAttribute("type")));

	authInfoPw = parser.getInnerTextByPath("domain:authInfo/domain:pw", "");
}


AutoPtr<Element> Sydora::Commands::InfoDomain::toXML(AutoPtr<Document> pDoc) const
{
	AutoPtr<Element> info = pDoc->createElement("info");
	AutoPtr<Element> domainInfo = pDoc->createElement("domain:info");
	domainInfo->setAttribute(
		"xmlns:domain", "urn:ietf:params:xml:ns:domain-1.0");

	for (int i = 0; i < domains.size(); i++)
		domainInfo->appendChild(CreateTextElement(pDoc,
			"domain:name", domains[i]));

	info->appendChild(domainInfo);
	return info;
}

