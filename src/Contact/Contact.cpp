#include <Sydora/Contact/Contact.h>

#include <iostream>
#include <vector>
#include <EPP/Request.h>
#include <EPP/Command.h>

#include <EPP/XML/XmlHelpers.h>
#include <EPP/XML/XmlParserWrapper.h>

#include <Poco/UUIDGenerator.h>
#include <Poco/NumberFormatter.h>

#include <Poco/DOM/AutoPtr.h>
#include <Poco/DOM/Document.h>
#include <Poco/DOM/Element.h>

using namespace Poco::XML;
using namespace Sydora;

Address::Address(const XmlParserWrapper& parser)
{
	street = parser.getInnerTextsByPath("contact:street");
	city = parser.getInnerTextByPath("contact:city");
	sp = parser.getInnerTextByPath("contact:sp");
	pc = parser.getInnerTextByPath("contact:pc");
	cc = parser.getInnerTextByPath("contact:cc");
}

AutoPtr<Element> Address::toXML(AutoPtr<Document> doc) const
{
	AutoPtr<Element> addr;
	addr = doc->createElement("contact:addr");

	for (unsigned i = 0; i < street.size(); i++)
		AddElement(doc, addr, "contact:street", street[i]);

	AddElement(doc, addr, "contact:city", city);
	AddElement(doc, addr, "contact:sp", sp);
	AddElement(doc, addr, "contact:pc", pc);
	AddElement(doc, addr, "contact:cc", cc);

	return addr;
}


PostalInfo::PostalInfo(const XmlParserWrapper& parser)
{
	type = parser.getAttributeByPath("", "type");
	name = parser.getInnerTextByPath("contact:name");
	org = parser.getInnerTextByPath("contact:org");

	addr = parser.parse<Address>("contact:addr");
}

AutoPtr<Element> PostalInfo::toXML(AutoPtr<Document> doc) const
{
	AutoPtr<Element> postalInfo = doc->createElement("contact:postalInfo");
	postalInfo->setAttribute("type", type);

	AddElement(doc, postalInfo, "contact:name", name);
	AddElement(doc, postalInfo, "contact:org", org);

	postalInfo->appendChild(addr.toXML(doc));

	return postalInfo;
}


SkContactIdent::SkContactIdent(const XmlParserWrapper& parser)
{
	legalForm = parser.getInnerTextByPath("legalForm");
	value = parser.getInnerTextByPath("identValue", "");
}

AutoPtr<Element> SkContactIdent::toXML(AutoPtr<Document> doc) const
{
	AutoPtr<Element> skContactIdent = doc->createElement("skContactIdent:create");
	skContactIdent->setAttribute("xmlns:skContactIdent", "http://www.sk-nic.sk/xml/epp/sk-contact-ident-0.2");

	AddElement(doc, skContactIdent, "skContactIdent:legalForm", legalForm);

	if (value == "")
		return skContactIdent;

	auto identValue = AddElement(doc, skContactIdent, "skContactIdent:identValue");

	if (legalForm == "CORP")
		AddElement(doc, identValue, "skContactIdent:corpIdent", value);
	else
		AddElement(doc, identValue, "skContactIdent:persIdent", value);

	return skContactIdent;
}


ContactCreated::ContactCreated(const XmlParserWrapper& parser)
{
	crDate = parser.getInnerTextByPath("contact:crDate");
	id = parser.getInnerTextByPath("contact:id");
}


ContactInfo::ContactInfo(const XmlParserWrapper& parser, const XmlParserWrapper& extsParser)
	: nodisclose(false)
{
	id = parser.getInnerTextByPath("contact:id");
	roid = parser.getInnerTextByPath("contact:roid");
	clID = parser.getInnerTextByPath("contact:clID");
	crID = parser.getInnerTextByPath("contact:crID");
	crDate = parser.getInnerTextByPath("contact:crDate");
	upDate = parser.getInnerTextByPath("contact:upDate", "");
	trDate = parser.getInnerTextByPath("contact:trDate", "");
	authInfo = parser.getInnerTextByPath("contact:authInfo", "");
	email = parser.getInnerTextByPath("contact:email", "");
	postalInfo = parser.parse<PostalInfo>("contact:postalInfo");
	voice = parser.getInnerTextByPath("contact:voice", "");
	fax = parser.getInnerTextByPath("contact:fax", "");
	
	auto discloses = parser.getElementsByPath("contact:disclose");
	if (discloses.size() == 1)
		nodisclose = parser.parse<Disclose>("contact:disclose");

	if (extsParser.getElementByPath("infData").isNull() == false)
		skContactIdent = extsParser.parse<SkContactIdent>("infData");
}
