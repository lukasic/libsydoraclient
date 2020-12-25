#include <Sydora/Domain/CheckDomain.h>

#include <EPP/Command.h>

#include <Poco/DOM/AutoPtr.h>
#include <Poco/DOM/Document.h>
#include <Poco/DOM/Element.h>

AutoPtr<Element> Sydora::Commands::CheckDomain::toXML(AutoPtr<Document> pDoc) const
{
	AutoPtr<Element> check = pDoc->createElement("check");
	AutoPtr<Element> checkDomain = pDoc->createElement("domain:check");
	checkDomain->setAttribute("xmlns:domain", "urn:ietf:params:xml:ns:domain-1.0");

	for (int i = 0; i < domains.size(); i++)
		checkDomain->appendChild(CreateTextElement(pDoc,
			"domain:name", domains[i]));

	check->appendChild(checkDomain);
	return check;
}
