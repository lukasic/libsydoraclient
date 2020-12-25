#pragma once

#include <iostream>
#include <vector>
#include <EPP/Request.h>
#include <EPP/Command.h>

#include <EPP/XML/XmlParserWrapper.h>
#include <EPP/XML/XmlHelpers.h>
using namespace Poco::XML;

class HostUpdate : public XmlSerializable
{
protected:
	struct _Add : public XmlSerializable
	{
		std::list<std::string> ipv4;
		std::list<std::string> ipv6;

		inline bool isSet() const
		{
			return ipv4.size() > 0 || ipv6.size() > 0;
		}

		inline AutoPtr<Element> toXML(AutoPtr<Document> pDoc) const
		{
			AutoPtr<Element> e = pDoc->createElement("host:add");

			for each (auto addr in ipv4)
				AddElement(pDoc, e, "addr", addr)
				->setAttribute("ip", "v4");

			for each (auto addr in ipv6)
				AddElement(pDoc, e, "addr", addr)
				->setAttribute("ip", "v6");

			return e;
		}
	};

	struct _Rem : public XmlSerializable
	{
		std::list<std::string> ipv4;
		std::list<std::string> ipv6;

		inline bool isSet() const
		{
			return ipv4.size() > 0 || ipv6.size() > 0;
		}

		inline AutoPtr<Element> toXML(AutoPtr<Document> pDoc) const
		{
			AutoPtr<Element> e = pDoc->createElement("host:rem");

			for each (auto addr in ipv4)
				AddElement(pDoc, e, "addr", addr)
				->setAttribute("ip", "v4");

			for each (auto addr in ipv6)
				AddElement(pDoc, e, "addr", addr)
				->setAttribute("ip", "v6");

			return e;
		}
	};

	struct _Chg : public XmlSerializable
	{
		std::string name;

		inline bool isSet() const
		{
			return name != "";
		}

		inline AutoPtr<Element> toXML(AutoPtr<Document> pDoc) const
		{
			AutoPtr<Element> e = pDoc->createElement("host:chg");

			if (name != "")
				AddElement(pDoc, e, "host:name", name);

			return e;
		}
	};


public:

	inline HostUpdate() {}

	inline HostUpdate(const std::string name) : name(name) {}

	virtual inline AutoPtr<Element> toXML(AutoPtr<Document> pDoc) const
	{
		AutoPtr<Element> e = pDoc->createElement("host:update");
		e->setAttribute("xmlns:host", "urn:ietf:params:xml:ns:host-1.0");

		AddElement(pDoc, e, "host:name", name);

		if (add.isSet())
			AddElement(pDoc, e, add.toXML(pDoc));

		if (rem.isSet())
			AddElement(pDoc, e, rem.toXML(pDoc));

		if (chg.isSet())
			AddElement(pDoc, e, chg.toXML(pDoc));

		return e;
	}


	std::string name;
	_Add add;
	_Rem rem;
	_Chg chg;
};
