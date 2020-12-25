#pragma once

// #include "afx.h"

#include <map>
#include <string>

#include <EPP/EppClient.h>
#include <EPP/Request.h>
#include <EPP/Command.h>
#include <EPP/Response.h>

#include <Sydora/Session/Login.h>
#include <EPP/Session/Hello.h>
#include <EPP/Session/Logout.h>

#include <Sydora/Contact/Contact.h>
#include <Sydora/Domain/Domain.h>
#include <Sydora/Host/Host.h>

#include <EPP/XML/XmlParserWrapper.h>

#include <Sydora/Domain/CheckDomain.h>
#include <Sydora/Domain/InfoDomain.h>
#include <Sydora/Domain/CreateDomain.h>
#include <Sydora/Host/InfoHost.h>
#include <Sydora/Host/CheckHost.h>
#include <Sydora/Host/CreateHost.h>
#include <Sydora/Contact/CheckContact.h>
#include <Sydora/Contact/CreateContact.h>
#include <Sydora/Contact/InfoContact.h>

#include <Sydora/Poll/PollReqResponse.h>

#include <Poco/UnicodeConverter.h>
using Poco::UnicodeConverter;

#include <Poco/DOM/AutoPtr.h>
#include <Poco/DOM/Document.h>
#include <Poco/DOM/DOMParser.h>
using namespace Poco::XML;

#include <EPP/CheckObjectResponse.h>


namespace Sydora
{
	class Sydora
	{
	private:
		EPP::Client<Sydora>& client;

	public:
		class Request: public EPP::Request
		{
		public:
			inline Request(EPP::Command::Ptr cmd)
			: EPP::Request(cmd)
			{}

		protected:
			inline Request()
			: EPP::Request()
			{}
		};

		class Response: public EPP::Response
		{
		public:
			inline Response(const std::string& xml)
			: EPP::Response(xml)
			{}

		private:
		};

		inline Sydora(EPP::Client<Sydora>& client)
		: client(client)
		{}

		virtual inline ~Sydora() {}

		virtual void hello();
		virtual void login(const std::string& clID, const std::string& pw);
		virtual EPP::Command::Ptr logout();

		virtual EPP::CheckObjectResponse
			_checkObject(const std::string& objectType, const std::string& objectField, const std::string& name);

		virtual std::map<std::string, EPP::CheckObjectResponse> _checkObjects(
			const std::string& objectType, const std::string& objectField, const std::vector<std::string>& names);

		virtual EPP::CheckObjectResponse checkDomain  (const std::string& domainName);
		virtual EPP::CheckObjectResponse checkContact (const std::string& contactID);
		virtual EPP::CheckObjectResponse checkHost    (const std::string& hostName);

		virtual std::map<std::string, EPP::CheckObjectResponse>
			checkDomains(const std::vector<std::string>& domains);
		
		virtual std::map<std::string, EPP::CheckObjectResponse>
			checkContacts(const std::vector<std::string>& contacts);

		/**
		 * INFO COMMANDS
		 *
		 */

		virtual DomainInfo infoDomain(const std::string& domainName);
		virtual ContactInfo infoContact(const std::string& id);
		virtual HostInfo infoHost(const std::string& hostName);

		/**
		 * CREATE COMMANDS
		 *
		 */ 

		virtual DomainCreated createDomain(const DomainCreate& domain);
		virtual ContactCreated createContact(const ContactCreate& contact);
		virtual HostCreated createHost(const HostCreate& hc);

		/**
		 * RENEW
		 */
		
		virtual DomainRenewed renewDomain(const DomainRenew& renew);

		/**
		 * UPDATE
		 */
		
		virtual void updateDomain(const DomainUpdate& update);
		virtual void updateHost(const HostUpdate& update);

		/**
		 * TRANSFER
		 */
		virtual void requestTransferDomain(const DomainTransferRequest& dtr);

		/**
		 * POLL
		 */
		virtual int getPollMessagesCount();
		virtual PollData::Ptr getPollMessage();

		virtual void ackPollMessage(int msgid);

	};
}
