// #include <afx.h>

#include <iostream>

#include <EPP/EppClient.h>
#include <EPP/ConnHandlers.h>

#include <Sydora/Sydora.h>

#include "tests/check.cpp"
#include "tests/info.cpp"
#include "tests/create.cpp"
// #include "tests/poll.cpp"

void poll(EPP::Client<Sydora::Sydora>& client);

#include <EPP/XML/XmlParserWrapper.h>

#include <string>
#include <sstream>

#include <Poco/Net/Context.h>

#include <Poco/DOM/Document.h>
#include <Poco/DOM/Element.h>
#include <Poco/DOM/Text.h>
#include <Poco/DOM/Node.h>
#include <Poco/DOM/AutoPtr.h>
#include <Poco/DOM/DOMWriter.h>
#include <Poco/XML/XMLWriter.h>
#include <Poco/SharedPtr.h>

#ifdef _WIN32
#pragma comment (lib, "EppClient.lib")
#pragma comment (lib, "SydoraClient.lib")
#endif

int main(int argc, const char ** argv)
{
	Poco::Net::initializeSSL();

	try
	{
		auto socket = new EPP::ConnHandlers::LoggedStringSocket("epp.test.sk-nic.sk", 700);
		EPP::Client<Sydora::Sydora> client(socket);
		client->login(SKNIC_USERNAME, SKNIC_PASSWORD);
		
		// check(client);
		info(client);
		// create(client);
		// poll(client);

	}
	catch (EPP::Exception& e)
	{
		std::cout << e;
	}

	Poco::Net::uninitializeSSL();
}
