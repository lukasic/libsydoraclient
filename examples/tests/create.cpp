#include <iostream>

#include <EPP/EppClient.h>
#include <Sydora/Sydora.h>
#include <Sydora/Contact/Contact.h>
#include <Sydora/Domain/Domain.h>

using Sydora::ContactCreate;
using Sydora::DomainInfo;
using Sydora::DomainCreate;
using Sydora::DomainCreated;

void createContact(EPP::Client<Sydora::Sydora>& client)
{
	ContactCreate contact;
	contact.id = "contact-test3";
	contact.postalInfo.type = "int";
	contact.postalInfo.name = "Contact Test";
	contact.postalInfo.addr.street.push_back("Under bridge 28");
	contact.postalInfo.addr.city = "Prague";
	contact.postalInfo.addr.pc = "14700";
	contact.postalInfo.addr.cc = "CZ";
	contact.email = "sknictest@example.com";
	contact.skContactIdent.legalForm = "PERS";
	contact.skContactIdent.value = "1993-01-01";

	client->createContact(contact);
}

void createDomain(EPP::Client<Sydora::Sydora>& client)
{
	DomainCreate domain;
	domain.name = "test-0006.sk";
	domain.registrant = "example-test";
	domain.period = 2;
	domain.addNameServer("ns1.example.com");
	domain.addNameServer("ns2.example.cz");
	domain.addContact("contact-test", "tech");
	domain.addContact("contact-test", "admin");
	
	client->createDomain(domain);
	client->infoDomain("test-0006.sk");
}

void create(EPP::Client<Sydora::Sydora>& client)
{
	try 
	{
		createContact(client);
		createDomain(client);
	}
	
	catch (EPP::Exception& e)
	{
		std::cout << e;
	}

}

