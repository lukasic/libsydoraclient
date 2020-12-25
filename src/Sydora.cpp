#include <Sydora/Sydora.h>

#include <map>
#include <string>

#include <EPP/EppClient.h>
#include <EPP/Request.h>
#include <EPP/Command.h>
#include <EPP/Response.h>

#include <Sydora/Session/Login.h>
#include <EPP/Session/Hello.h>
#include <EPP/Session/Greeting.h>
#include <EPP/Session/Logout.h>

#include <Sydora/Contact/Contact.h>
#include <Sydora/Domain/Domain.h>
#include <Sydora/Host/Host.h>

#include <EPP/XML/XmlParserWrapper.h>

#include <Sydora/Domain/CheckDomain.h>
#include <Sydora/Domain/InfoDomain.h>
#include <Sydora/Domain/CreateDomain.h>
#include <Sydora/Domain/RenewDomain.h>
#include <Sydora/Domain/UpdateDomain.h>
#include <Sydora/Domain/TransferRequestDomain.h>

#include <Sydora/Host/InfoHost.h>
#include <Sydora/Host/CheckHost.h>
#include <Sydora/Host/CreateHost.h>
#include <Sydora/Host/UpdateHost.h>

#include <Sydora/Contact/CheckContact.h>
#include <Sydora/Contact/CreateContact.h>
#include <Sydora/Contact/InfoContact.h>

#include <Sydora/Poll/PollReq.h>
#include <Sydora/Poll/PollReqResponse.h>
#include <Sydora/Poll/PollAck.h>

#include <Poco/DOM/AutoPtr.h>
#include <Poco/DOM/Document.h>
#include <Poco/DOM/DOMParser.h>
using namespace Poco::XML;

#include <EPP/CheckObjectResponse.h>

using Sydora::ContactCreated;
using Sydora::ContactInfo;
using Sydora::DomainInfo;
using Sydora::DomainCreated;
using Sydora::DomainRenewed;
using Sydora::HostCreate;
using Sydora::HostCreated;

void Sydora::Sydora::hello()
{
	client.request<
		::EPP::Hello,
		::EPP::Greeting
	>(::EPP::Hello());
}

void Sydora::Sydora::login(const std::string& clID, const std::string& pw)
{
	EPP::Command::Ptr command = new ::Sydora::Commands::Login(clID, pw);

	Request request(command);
	client.request(request);
}

EPP::Command::Ptr Sydora::Sydora::logout()
{
	return new ::EPP::Commands::Logout();
}


EPP::CheckObjectResponse Sydora::Sydora::_checkObject(const std::string& objectType, const std::string& objectField, const std::string& name)
{
	EPP::Command::Ptr command;
	
	if (objectType == "domain")
		command = new ::Sydora::Commands::CheckDomain(name);

	else if (objectType == "contact")
		command = new ::Sydora::Commands::CheckContact(name);
	
	else if (objectType == "host")
		command = new ::Sydora::Commands::CheckHost(name);

	else
		throw EPP::Exception("Unknown object type: " + objectType);

	Request request(command);
	Response response = client.request(request);

	XmlParserWrapper parser(response.xml, "response/resData");
	std::map<std::string, EPP::CheckObjectResponse> data;
	data = EPP::CheckObjectResponse::parseAll(parser, objectType, objectField);
	return data.find(name)->second;
}

std::map<std::string, EPP::CheckObjectResponse> Sydora::Sydora::_checkObjects(
	const std::string& objectType, const std::string& objectField, const std::vector<std::string>& names)
{
	EPP::Command::Ptr command;
	
	if (objectType == "domain")
		command = new ::Sydora::Commands::CheckDomain(names);
	
	else if (objectType == "contact")
		command = new ::Sydora::Commands::CheckContact(names);
	
	else if (objectType == "host")
		command = new ::Sydora::Commands::CheckHost(names);
	
	else
		throw EPP::Exception("Unknown object type: " + objectType);

	Request request(command);
	Response response = client.request(request);

	XmlParserWrapper parser(response.xml, "response/resData");
	auto x = EPP::CheckObjectResponse::parseAll(parser, objectType, objectField);
	return x;
}

EPP::CheckObjectResponse Sydora::Sydora::checkDomain(const std::string& domainName) {
	return _checkObject("domain", "name", domainName);
};

EPP::CheckObjectResponse Sydora::Sydora::checkContact(const std::string& contactID) {
	return _checkObject("contact", "id", contactID);
};

EPP::CheckObjectResponse Sydora::Sydora::checkHost(const std::string& hostName) {
	return _checkObject("host", "name", hostName);
};


std::map<std::string, EPP::CheckObjectResponse> Sydora::Sydora::checkDomains(const std::vector<std::string>& domains) {
	return _checkObjects("domain", "name", domains);
}

std::map<std::string, EPP::CheckObjectResponse> Sydora::Sydora::checkContacts(const std::vector<std::string>& contacts) {
	return _checkObjects("contact", "id", contacts);
}



DomainInfo Sydora::Sydora::infoDomain(const std::string& domainName)
{
	auto command = new ::Sydora::Commands::InfoDomain(domainName);

	Request request(command);
	Response response = client.request(request);
	XmlParserWrapper parser(response.xml, "response/resData/domain:infData");

	return DomainInfo(parser);
};

ContactInfo Sydora::Sydora::infoContact(const std::string& id)
{
	auto command = new ::Sydora::Commands::InfoContact(id);

	Request request(command);
	Response response = client.request(request);
	XmlParserWrapper parser(response.xml, "response/resData/contact:infData");
	XmlParserWrapper extensionsParser(response.xml, "response/extension");

	return ContactInfo(parser, extensionsParser);
};

HostInfo Sydora::Sydora::infoHost(const std::string& hostName)
{
	auto command = new ::Sydora::Commands::InfoHost(hostName);

	Request request(command);
	Response response = client.request(request);
	XmlParserWrapper parser(response.xml, "response/resData/host:infData");

	return HostInfo(parser);
};


DomainCreated Sydora::Sydora::createDomain(const DomainCreate& domain)
{
	auto command = new ::Sydora::Commands::CreateDomain(domain);

	Request request(command);
	Response response = client.request(request);
	XmlParserWrapper parser(response.xml, "response/resData/domain:creData");

	return DomainCreated(parser);
}

ContactCreated Sydora::Sydora::createContact(const ContactCreate& contact)
{
	auto command = new ::Sydora::Commands::CreateContact(contact);

	Request request(command);
	Response response = client.request(request);
	XmlParserWrapper parser(response.xml, "response/resData/contact:creData");

	return ContactCreated(parser);
}

HostCreated Sydora::Sydora::createHost(const HostCreate& hc)
{
	auto command = new ::Sydora::Commands::CreateHost(hc);

	Request request(command);
	Response response = client.request(request);
	XmlParserWrapper parser(response.xml, "response/resData/host:creData");

	return HostCreated(parser);
}


DomainRenewed Sydora::Sydora::renewDomain(const DomainRenew& renew)
{
	auto command = new ::Sydora::Commands::RenewDomain(renew);
	
	Request request(command);
	Response response = client.request(request);
	XmlParserWrapper parser(response.xml, "response/resData/domain:renData");

	return DomainRenewed(parser);
}

void Sydora::Sydora::updateDomain(const DomainUpdate& update)
{
	auto command = new ::Sydora::Commands::UpdateDomain(update);

	Request request(command);
	Response response = client.request(request);
}

void Sydora::Sydora::updateHost(const HostUpdate& update)
{
	auto command = new ::Sydora::Commands::UpdateHost(update);

	Request request(command);
	Response response = client.request(request);
}

void Sydora::Sydora::requestTransferDomain(const DomainTransferRequest& dtr)
{
	auto command = new ::Sydora::Commands::TransferRequestDomain(dtr);

	Request request(command);
	client.request(request);
}

int Sydora::Sydora::getPollMessagesCount()
{
	auto command = new ::Sydora::Commands::PollReq();

	Request request(command);
	Response response = client.request(request);

	if (response.code == 1300)
		return 0;

	if (response.code == 1301)
		return PollReqResponse(response.xml).msgQ.count;

	throw EPP::Exception("Invalid response code from Sydora.");
}

PollData::Ptr Sydora::Sydora::getPollMessage()
{
	auto command = new ::Sydora::Commands::PollReq();

	Request request(command);
	PollReqResponse response = client.request<Request, PollReqResponse>(request);
	
	return response.parseData();
}

void Sydora::Sydora::ackPollMessage(int msgid)
{
	auto command = new ::Sydora::Commands::PollAck(msgid);

	Request request(command);
	client.request(request);
}
