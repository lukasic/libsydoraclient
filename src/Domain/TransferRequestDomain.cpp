#include <Sydora/Domain/TransferRequestDomain.h>

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

AutoPtr<Element> TransferRequestDomain::toXML(AutoPtr<Document> pDoc) const
{
	AutoPtr<Element> update = pDoc->createElement("transfer");
	update->setAttribute("op", "request");

	AddElement(pDoc, update, dtr.toXML(pDoc));
	return update;
}

