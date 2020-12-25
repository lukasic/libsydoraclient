#pragma once

#include <EPP/Command.h>
#include <Sydora/Domain/Domain.h>

#include <Poco/DOM/AutoPtr.h>
#include <Poco/DOM/Document.h>
#include <Poco/DOM/Element.h>

namespace Sydora
{
	namespace Commands
	{
		class CreateDomain: public EPP::Command
		{
		protected:
			DomainCreate domain;

		public:

			inline CreateDomain(const DomainCreate& domain)
			: domain(domain)
			{}

			virtual AutoPtr<Element> toXML(AutoPtr<Document> pDoc) const;
			
		};
	}
}
