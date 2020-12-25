#pragma once

#include <vector>
#include <string>
#include <EPP/Command.h>

#include <Poco/DOM/AutoPtr.h>
#include <Poco/DOM/Document.h>
#include <Poco/DOM/Element.h>

#include <Poco/NumberFormatter.h>
using Poco::NumberFormatter;

namespace Sydora
{
	namespace Commands
	{
		class PollAck : public EPP::Command
		{
		protected:
			int msgid;

		public:
			inline PollAck(int msgid)
				: msgid(msgid)
			{}

			virtual inline AutoPtr<Element> toXML(AutoPtr<Document> pDoc) const
			{
				AutoPtr<Element> poll = pDoc->createElement("poll");
				poll->setAttribute("op", "ack");
				poll->setAttribute("msgID", NumberFormatter::format(msgid));

				return poll;
			}

		};
	}
}
