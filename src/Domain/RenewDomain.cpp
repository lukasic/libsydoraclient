#include <Sydora/Domain/RenewDomain.h>

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
using namespace Sydora::Commands;

AutoPtr<Element> RenewDomain::toXML(AutoPtr<Document> pDoc) const
{
	AutoPtr<Element> info = pDoc->createElement("renew");
	AutoPtr<Element> domainRenew = pDoc->createElement("domain:renew");
	info->appendChild(domainRenew);

	domainRenew->setAttribute(
		"xmlns:domain", "urn:ietf:params:xml:ns:domain-1.0");

	AddElement(pDoc, domainRenew, "domain:name", dr.name);
	AddElement(pDoc, domainRenew, "domain:curExpDate", dr.curExpDate);
	AddElement(pDoc, domainRenew, "domain:period", dr.period)
		->setAttribute("unit", "y");

	return info;
}

