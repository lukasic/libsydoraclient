#pragma once

#include <iostream>
#include <vector>
#include <EPP/Request.h>
#include <EPP/Command.h>

#include <EPP/XML/XmlParserWrapper.h>
#include <EPP/XML/XmlHelpers.h>

#include <Poco/DOM/AutoPtr.h>
#include <Poco/DOM/Document.h>
#include <Poco/DOM/Element.h>
#include <Poco/UUIDGenerator.h>
#include <Poco/NumberFormatter.h>

using namespace Poco::XML;

namespace Sydora
{

	struct DomainContact : public XmlSerializable
	{
		std::string id;
		std::string type;

		inline DomainContact(const std::string& id, const std::string& type)
			: id(id), type(type)
		{}

		virtual inline AutoPtr<Element> toXML(AutoPtr<Document> pDoc) const
		{
			AutoPtr<Element> e = CreateTextElement(pDoc, "domain:contact", id);
			e->setAttribute("type", type);
			return e;
		}
	};

	struct CreateDomainContact : public DomainContact
	{
		inline CreateDomainContact(const std::string& id, const std::string& type)
			: DomainContact(id, type)
		{}
	};

	struct DomainNSList: public XmlSerializable
	{
		inline DomainNSList() {}

		inline void add(const std::string& name)
		{
			if (_hosts.find(name) == _hosts.end())
				_hosts.insert(name);
		}

		inline bool isSet() const { return _hosts.size() > 0; }

		virtual inline AutoPtr<Element> toXML(AutoPtr<Document> pDoc) const
		{
			AutoPtr<Element> e = pDoc->createElement("domain:ns");

			for each (auto host in _hosts)
				AddElement(pDoc, e, "domain:hostObj", host);

			return e;
		}

		friend DomainNSList& operator<< (DomainNSList& l, const std::string& host)
		{
			l.add(host);
			return l;
		}

		std::set<std::string> _hosts;
	};

	class DomainCreate
	{
	public:

		inline DomainCreate()
		{
			generateAuthInfoPassword();
		}

		inline void addContact(const std::string& id, const std::string& type) {
			contacts.push_back(CreateDomainContact(id, type));
		}

		inline void addNameServer(const std::string& fqdn) {
			ns << fqdn;
		}

		inline void generateAuthInfoPassword() {
			authInfoPw = Poco::UUIDGenerator().create().toString();
		}

		//

		std::string name;
		DomainNSList ns;

		std::vector<CreateDomainContact> contacts;

		int period;
		std::string registrant;
		std::string authInfoPw;

	private:
	};


	class DomainCreated
	{
	public:
		inline DomainCreated()
		{}

		DomainCreated(XmlParserWrapper& parser);

		// private:
		std::string name;
		std::string crDate;
		std::string exDate;
	};

	class DomainRenew
	{
	public:

		inline DomainRenew() {}

		std::string name;
		std::string curExpDate;
		int period; // in years
	};

	class DomainRenewed
	{
	public:
		inline DomainRenewed() {}

		DomainRenewed(XmlParserWrapper& parser);

		std::string name;
		std::string exDate;
	};

	class DomainUpdate: public XmlSerializable
	{
	protected:
		struct _Add : public XmlSerializable
		{
			DomainNSList ns;
			std::list<DomainContact> contacts;

			inline bool isSet() const
			{
				return ns.isSet() || contacts.size() > 0;
			}

			inline AutoPtr<Element> toXML(AutoPtr<Document> pDoc) const
			{
				AutoPtr<Element> e = pDoc->createElement("domain:add");

				if (ns.isSet())
					AddElement(pDoc, e, ns.toXML(pDoc));

				for each (auto contact in contacts)
					AddElement(pDoc, e, contact.toXML(pDoc));

				return e;
			}
		};

		struct _Rem : public XmlSerializable
		{
			DomainNSList ns;
			std::list<DomainContact> contacts;

			inline bool isSet() const
			{
				return ns.isSet() || contacts.size() > 0;
			}

			inline AutoPtr<Element> toXML(AutoPtr<Document> pDoc) const
			{
				AutoPtr<Element> e = pDoc->createElement("domain:rem");

				if (ns.isSet())
					AddElement(pDoc, e, ns.toXML(pDoc));

				for each (auto contact in contacts)
					AddElement(pDoc, e, contact.toXML(pDoc));

				return e;
			}
		};

		struct _Chg : public XmlSerializable
		{
			std::string registrant;
			std::string authInfoPw;

			inline bool isSet() const
			{
				return registrant != "" || authInfoPw != "";
			}
		
			inline AutoPtr<Element> toXML(AutoPtr<Document> pDoc) const
			{
				AutoPtr<Element> e = pDoc->createElement("domain:chg");

				if (registrant != "")
					AddElement(pDoc, e, "domain:registrant", registrant);

				if (authInfoPw != "")
				{
					auto authInfo = AddElement(pDoc, e, "domain:authInfo");
					AddElement(pDoc, authInfo, "domain:pw", authInfoPw);
				}

				return e;
			}
		};


	public:
		inline DomainUpdate(const std::string name): name(name) {}

		virtual inline AutoPtr<Element> toXML(AutoPtr<Document> pDoc) const
		{
			AutoPtr<Element> e = pDoc->createElement("domain:update");
			e->setAttribute("xmlns:domain", "urn:ietf:params:xml:ns:domain-1.0");

			AddElement(pDoc, e, "domain:name", name);

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

	class DomainTransferRequest: public XmlSerializable
	{
	public:
		
		inline DomainTransferRequest()
			: period(1)
		{}

		inline DomainTransferRequest(const std::string& name, const std::string& authInfoPw)
			: name(name), period(1), authInfoPw(authInfoPw)
		{}

		inline DomainTransferRequest(const std::string& name, int period, const std::string& authInfoPw)
			: name(name), period(period), authInfoPw(authInfoPw)
		{}
		
		virtual inline AutoPtr<Element> toXML(AutoPtr<Document> pDoc) const
		{
			AutoPtr<Element> e = pDoc->createElement("domain:transfer");
			e->setAttribute("xmlns:domain", "urn:ietf:params:xml:ns:domain-1.0");

			AddElement(pDoc, e, "domain:name", name);
			AddElement(pDoc, e, "domain:period", period)
				->setAttribute("unit", "y");

			auto authInfo = AddElement(pDoc, e, "domain:authInfo");
			AddElement(pDoc, authInfo, "domain:pw", authInfoPw);

			return e;
		}

		
	// private:
		std::string name;
		int period;
		std::string authInfoPw;

	};


};


class domainStatus
{
public:
	enum domainStatusType {
		clientDeleteProhibited,
		clientHold,
		clientRenewProhibited,
		clientTransferProhibited,
		clientUpdateProhibited,
		inactive,
		ok,
		pendingCreate,
		pendingDelete,
		pendingRenew,
		pendingTransfer,
		pendingUpdate,
		serverDeleteProhibited,
		serverHold,
		serverRenewProhibited,
		serverTransferProhibited,
		serverUpdateProhibited
	};

	domainStatusType value;
	inline domainStatus() {}
	inline domainStatus(domainStatusType v): value(v) {}

	inline domainStatus(const std::string& str)
	{
			 if (str == "clientDeleteProhibited")	value = clientDeleteProhibited;
		else if (str == "clientHold")				value = clientHold;
		else if (str == "clientRenewProhibited")	value = clientRenewProhibited;
		else if (str == "clientTransferProhibited")	value = clientTransferProhibited;
		else if (str == "clientUpdateProhibited")	value = clientUpdateProhibited;
		else if (str == "inactive")					value = inactive;
		else if (str == "ok")						value = ok;
		else if (str == "pendingCreate")			value = pendingCreate;
		else if (str == "pendingDelete")			value = pendingDelete;
		else if (str == "pendingRenew")				value = pendingRenew;
		else if (str == "pendingTransfer")			value = pendingTransfer;
		else if (str == "pendingUpdate")			value = pendingUpdate;
		else if (str == "serverDeleteProhibited")	value = serverDeleteProhibited;
		else if (str == "serverHold")				value = serverHold;
		else if (str == "serverRenewProhibited")	value = serverRenewProhibited;
		else if (str == "serverTransferProhibited")	value = serverTransferProhibited;
		else if (str == "serverUpdateProhibited")	value = serverUpdateProhibited;
	}

	inline friend std::ostream& operator<<(std::ostream& os, const domainStatus & o)
	{
		if (o.value == clientDeleteProhibited)
			return os << "clientDeleteProhibited";
		if (o.value == clientHold)
			return os << "clientHold";
		if (o.value == clientRenewProhibited)
			return os << "clientRenewProhibited";
		if (o.value == clientTransferProhibited)
			return os << "clientTransferProhibited";
		if (o.value == clientUpdateProhibited)
			return os << "clientUpdateProhibited";
		if (o.value == inactive)
			return os << "inactive";
		if (o.value == ok)
			return os << "ok";
		if (o.value == pendingCreate)
			return os << "pendingCreate";
		if (o.value == pendingDelete)
			return os << "pendingDelete";
		if (o.value == pendingRenew)
			return os << "pendingRenew";
		if (o.value == pendingTransfer)
			return os << "pendingTransfer";
		if (o.value == pendingUpdate)
			return os << "pendingUpdate";
		if (o.value == serverDeleteProhibited)
			return os << "serverDeleteProhibited";
		if (o.value == serverHold)
			return os << "serverHold";
		if (o.value == serverRenewProhibited)
			return os << "serverRenewProhibited";
		if (o.value == serverTransferProhibited)
			return os << "serverTransferProhibited";
		if (o.value == serverUpdateProhibited)
			return os << "serverUpdateProhibited";

		return os << "unknown";
	}

	inline friend bool operator== (const domainStatus& ds, const domainStatusType& dst) {
		return ds.value == dst;
	}

	inline friend bool operator== (const domainStatusType& dst, const domainStatus& ds) {
		return ds.value == dst;
	}
};

