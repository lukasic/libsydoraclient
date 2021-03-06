#pragma once

#include <iostream>
#include <vector>
#include <EPP/Request.h>
#include <EPP/Command.h>

#include <EPP/XML/XmlParserWrapper.h>
#include <EPP/XML/XmlHelpers.h>

#include <Sydora/Domain/Domain.h>

#include <Poco/DOM/AutoPtr.h>
#include <Poco/DOM/Document.h>
#include <Poco/DOM/Element.h>
using namespace Poco::XML;

namespace Sydora
{
	namespace Commands
	{
		class TransferRequestDomain : public EPP::Command
		{
		protected:
			DomainTransferRequest dtr;

		public:

			inline TransferRequestDomain(const DomainTransferRequest& trreq) : dtr(trreq)
			{}

			virtual AutoPtr<Element> toXML(AutoPtr<Document> pDoc) const;
		};
	}
}

