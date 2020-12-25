#pragma once

#include <iostream>
#include <vector>
#include <EPP/Request.h>
#include <EPP/Command.h>

#include <EPP/XML/XmlHelpers.h>
#include <EPP/XML/XmlParserWrapper.h>

#include <Poco/UUIDGenerator.h>
#include <Poco/Format.h>

#include <Poco/DOM/AutoPtr.h>
#include <Poco/DOM/Document.h>
#include <Poco/DOM/Element.h>
using namespace Poco::XML;
using Poco::format;

namespace Sydora
{
	class Address: public XmlSerializable
	{
	public:
		inline Address()
		{}

		Address(const XmlParserWrapper& parser);

		AutoPtr<Element> toXML(AutoPtr<Document> doc) const;

	// private:
		std::vector<std::string> street;
		std::string city;
		std::string sp;
		std::string pc;
		std::string cc;
	};


	class PostalInfo: public XmlSerializable
	{
	public:
		inline PostalInfo()
		{}

		PostalInfo(const XmlParserWrapper& parser);
		AutoPtr<Element> toXML(AutoPtr<Document> doc) const;

	// private:
		std::string type;
		std::string name;
		std::string org;
		Address addr;
	};

	struct DisclosedItem
	{
		enum ITEM {
			/**
			The <contact:disclose> element MUST contain at least one of the
			following child elements:

			<contact:name type="int"/>
			<contact:name type="loc"/>
			<contact:org type="int"/>
			<contact:org type="loc"/>
			<contact:addr type="int"/>
			<contact:addr type="loc"/>
			<contact:voice/>
			<contact:fax/>
			<contact:email/>
			*/

			NameInt,
			NameLoc,
			OrgInt,
			OrgLoc,
			AddrInt,
			AddrLoc,
			Voice,
			Fax,
			Email
		};

		ITEM item;
		std::string name;
		std::string type;

		inline DisclosedItem(const ITEM& i)
		{
			item = i;

			switch (i)
			{
			case NameInt:
				name = "name";
				type = "int";
				break;
			case NameLoc:
				name = "name";
				type = "loc";
				break;
			case OrgInt:
				name = "org";
				type = "int";
				break;
			case OrgLoc:
				name = "org";
				type = "loc";
				break;
			case AddrInt:
				name = "addr";
				type = "int";
				break;
			case AddrLoc:
				name = "addr";
				type = "loc";
				break;
			case Voice:
				name = "voice";
				break;
			case Fax:
				name = "fax";
				break;
			case Email:
				name = "email";
				break;
			default:
				throw EPP::Exception("Invalid argument for disclosed item.");
			}
		}

		inline DisclosedItem(const std::string& name)
		{
			if      (name == "voice") *this = DisclosedItem(Voice);
			else if (name == "fax")   *this = DisclosedItem(Fax);
			else if (name == "email") *this = DisclosedItem(Email);
			
			else 
				throw EPP::Exception("Invalid disclose: " + name);
		}

		inline DisclosedItem(const std::string& name, const std::string& type)
		{
			if (type == "")
				*this = DisclosedItem(name);

			else if (type == "int" && name == "name") *this = DisclosedItem(NameInt);
			else if (type == "int" && name == "org")  *this = DisclosedItem(OrgInt);
			else if (type == "int" && name == "addr") *this = DisclosedItem(AddrInt);

			else if (type == "loc" && name == "name") *this = DisclosedItem(NameLoc);
			else if (type == "loc" && name == "org")  *this = DisclosedItem(OrgLoc);
			else if (type == "loc" && name == "addr") *this = DisclosedItem(AddrLoc);

			else
				throw EPP::Exception("Invalid disclose: " + name + " type=" + type);
		}

		inline std::string toString()
		{
			switch (item)
			{
			case NameInt: return "NameInt";
			case NameLoc: return "NameLoc";
			case OrgInt: return "OrgInt";
			case OrgLoc: return "OrgLoc";
			case AddrInt: return "AddrInt";
			case AddrLoc: return "AddrLoc";
			case Voice: return "Voice";
			case Fax: return "Fax";
			case Email: return "Email";
			default:
				throw EPP::Exception("Cannot convert disclosed item to string.");
			}
		}

		static inline DisclosedItem FromString(const std::string& val)
		{
			
			if (val == "NameInt")  return NameInt;
			if (val == "NameLoc")  return NameLoc;
			if (val == "OrgInt")   return OrgInt;
			if (val == "OrgLoc")   return OrgLoc;
			if (val == "AddrInt")  return AddrInt;
			if (val == "AddrLoc")  return AddrLoc;
			if (val == "Voice")    return Voice;
			if (val == "Fax")      return Fax;
			if (val == "Email")    return Email;

			throw EPP::Exception(format("Cannot parse disclosed item from string '%s'", val));
		}

		static inline DisclosedItem parse(AutoPtr<Element> e)
		{
			std::string name = e->nodeName();

			if (name.find("contact:") == 0)
				name = name.substr(8);

			if (e->hasAttribute("type"))
				return DisclosedItem(name, e->getAttribute("type"));

			return DisclosedItem(name);
		}

		
	};

	inline bool ToBool(std::string val)
	{
		std::transform(val.begin(), val.end(), val.begin(), ::tolower);
		if (val == "0") return false;
		if (val == "false") return false;
		return true;
	}

	inline std::string ToString(bool val)
	{
		return val ? "true" : "false";
	}

	class Disclose : public XmlSerializable
	{
	public:

		inline Disclose(bool flag): flag(flag) {}

		inline Disclose(const XmlParserWrapper& parser)
		{
			flag = ToBool(parser.getAttributeByPath("", "flag"));
			auto elems = parser.getChildElements();

			for (auto i = elems.begin(); i != elems.end(); ++i)
				items.push_back(DisclosedItem::parse(*i));
		}

		virtual inline bool isSet() const { return items.size() > 0; }

		virtual inline AutoPtr<Element> toXML(AutoPtr<Document> doc) const
		{
			AutoPtr<Element> disclose = doc->createElement("contact:disclose");
			disclose->setAttribute("flag", ToString(flag));

			for each (auto i in items)
			{
				auto e = AddElement(doc, disclose, "contact:" + i.name);
				if (i.type != "")
					e->setAttribute("type", i.type);
			}

			return disclose;
		}

	// private:
		bool flag;
		std::vector<DisclosedItem> items;
	};

	class SkContactIdent: public XmlSerializable
	{
	public:
		inline SkContactIdent()
		{}

		SkContactIdent(const XmlParserWrapper& parser);

		virtual AutoPtr<Element> toXML(AutoPtr<Document> doc) const;

		inline SkContactIdent::Ptr clone() const
		{
			return SkContactIdent::Ptr(new SkContactIdent(*this));
		}


	// private:
		std::string legalForm;
		std::string value;
	};


	class ContactCreate
	{
	public:
		inline ContactCreate()
		:
			disclose(true),
			nodisclose(false)
		{
			generateAuthInfoPassword();
		}

		inline void generateAuthInfoPassword() {
			authInfoPw = Poco::UUIDGenerator().create().toString();
		}

		std::string id;
		std::string voice;
		std::string fax;
		std::string email;

		PostalInfo postalInfo;
		SkContactIdent skContactIdent;

		Disclose disclose;
		Disclose nodisclose;

	// protected:
		std::string authInfoPw;
	};


	class ContactCreated
	{
	public:

		ContactCreated(const XmlParserWrapper& parser);

	private:
		std::string id;
		std::string crDate;
	};

	class ContactInfo
	{
	public:

		ContactInfo(const XmlParserWrapper& parser, const XmlParserWrapper& extsParser);

	// private:

		std::string id;
		std::string roid;

		std::string clID; // registrátor - súčasný
		std::string crID; // registrátor - reg domény

		std::string crDate; // dátum registrácie
		std::string upDate; // dátum poslednej aktualizácie
		std::string trDate; // dátum expirácie
	
		std::string voice;
		std::string fax;
		std::string email;

		PostalInfo postalInfo;
		SkContactIdent skContactIdent;

		std::string authInfo;

		Disclose nodisclose;
	};

}