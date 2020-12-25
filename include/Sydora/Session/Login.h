#pragma once

#include <EPP/Session/Login.h>

namespace Sydora
{
	namespace Commands
	{
		class Login: public EPP::Commands::Login
		{
		public:
			inline Login(const std::string& clID, const std::string& pw)
				: EPP::Commands::Login(clID, pw)
			{
				svcs.objURI
					<< "urn:ietf:params:xml:ns:domain-1.0"
					<< "urn:ietf:params:xml:ns:contact-1.0"
					<< "urn:ietf:params:xml:ns:host-1.0";

				svcs.svcExtension
					<< "http://www.sk-nic.sk/xml/epp/sk-contact-ident-0.2";

				options["version"] = "1.0";
				options["lang"] = "en";
			}
		};
	}
}

