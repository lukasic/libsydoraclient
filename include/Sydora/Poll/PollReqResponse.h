#pragma once

#include <iostream>
#include <vector>

#include <EPP/Response.h>

#include <EPP/Request.h>
#include <EPP/Command.h>
#include <EPP/XML/XmlHelpers.h>
#include <EPP/XML/XmlParserWrapper.h>

#include <Sydora/Poll/PollData.h>

#include <Poco/NumberParser.h>

#include <Poco/DOM/AutoPtr.h>
#include <Poco/DOM/Document.h>
#include <Poco/DOM/Element.h>
using namespace Poco::XML;
using Poco::NumberParser;

class PollReqResponse : public EPP::Response
{
private:

	struct _msgQ
	{
		inline _msgQ(XmlParserWrapper& parser)
		{
			count = NumberParser::parse(
				parser.getAttributeByPath("response/msgQ", "count"));

			id = NumberParser::parse(
				parser.getAttributeByPath("response/msgQ", "id"));

			qDate = parser.getInnerTextByPath("response/msgQ/qDate");
			msg = parser.getInnerTextByPath("response/msgQ/msg");
		}

		int count;
		int id;
		std::string qDate;
		std::string msg;
	};

public:

	inline PollReqResponse(const std::string& xml)
	:
		EPP::Response(xml),
		msgQ(parser)
	{}

	PollData::Ptr parseData() const
	{
		XmlParserWrapper p1 = parser.subparser("response/resData");
		auto elem_list = p1.getChildElements();
		auto elem = elem_list[0];

		std::string content = XmlToString(elem);
		XmlParserWrapper p2 = XmlParserWrapper(content);

		return PollReqResponseDataParser::parse(msgQ.id, p2);
	}

	_msgQ msgQ;

};
