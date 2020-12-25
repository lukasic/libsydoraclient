#pragma once

#include <iostream>
#include <vector>

#include <EPP/Request.h>
#include <EPP/Command.h>
#include <EPP/XML/XmlHelpers.h>

#include <Sydora/Host/Host.h>

#include <Poco/DOM/AutoPtr.h>
#include <Poco/DOM/Document.h>
#include <Poco/DOM/Element.h>
using namespace Poco::XML;


namespace Sydora
{
	namespace Commands
	{
		class UpdateHost : public EPP::Command
		{
		protected:
			HostUpdate hu;

		public:
			inline UpdateHost()
			{}

			inline UpdateHost(const HostUpdate& hu)
				: hu(hu)
			{}

			virtual inline AutoPtr<Element> toXML(AutoPtr<Document> pDoc) const
			{
				AutoPtr<Element> info = pDoc->createElement("update");
				AutoPtr<Element> update = hu.toXML(pDoc);
				info->appendChild(update);
				return info;
			}
		};
	}
}