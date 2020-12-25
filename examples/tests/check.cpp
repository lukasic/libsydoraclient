#include <iostream>

#include <EPP/EppClient.h>
#include <Sydora/Sydora.h>

void check(EPP::Client<Sydora::Sydora>& client)
{
	try 
	{
		std::vector<std::string> domains;
		domains.push_back("example.sk");
		domains.push_back("ajkdhsakjhdajkdhjkahsdkjahsd.sk");
		domains.push_back("test.cz");
		
		// auto x = client->checkDomain(domains);
		// for (auto i = x.begin(); i != x.end(); ++i)
		// 	std::cout << i->second << std::endl;

		auto checkContact = client->checkContact("TEST-001");
		std::cout << checkContact << std::endl;

		auto checkHost = client->checkHost("ns1.example.cz");
		std::cout << checkHost << std::endl;

	}
	catch (EPP::Exception& e)
	{
		std::cout << e;
	}
}
