#include <iostream>

#include <EPP/EppClient.h>
#include <Sydora/Sydora.h>

void info(EPP::Client<Sydora::Sydora>& client)
{
	try 
	{
		auto infoDomain = client->infoDomain("test-example.sk");
		std::cout << infoDomain.name << std::endl;

		for each (auto status in infoDomain.status)
			std::cout << "> Status: " << status << std::endl;

		for each (auto hostname in infoDomain.ns._hosts)
			std::cout << hostname << std::endl;

		std::cout << std::endl;

		// auto infoContact = client->infoContact("EXAMPLE-CONTACT");

		auto infoHost = client->infoHost("ns1.example.sk");

		for each (auto ip in infoHost.ipv4)
			std::cout << "ipv4:" << ip << std::endl;

		for each (auto ip in infoHost.ipv6)
			std::cout << "ipv6:" << ip << std::endl;

	}

	catch (EPP::Exception& e)
	{
		std::cout << e;
	}

}

