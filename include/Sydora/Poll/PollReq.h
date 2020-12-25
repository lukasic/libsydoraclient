#pragma once

#include <vector>
#include <string>
#include <EPP/Command.h>

#include <Poco/DOM/AutoPtr.h>
#include <Poco/DOM/Document.h>
#include <Poco/DOM/Element.h>

namespace Sydora
{
	namespace Commands
	{
		class PollReq : public EPP::Command
		{
		protected:

		public:
			inline PollReq()
			{}

			virtual inline AutoPtr<Element> toXML(AutoPtr<Document> pDoc) const
			{
				AutoPtr<Element> poll = pDoc->createElement("poll");
				poll->setAttribute("op", "req");

				return poll;
			}

		};
	}
}
