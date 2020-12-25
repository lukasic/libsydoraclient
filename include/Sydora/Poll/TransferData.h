#pragma once

#include <iostream>
#include <vector>

#include <EPP/Request.h>
#include <EPP/Command.h>
#include <EPP/XML/XmlHelpers.h>
#include <EPP/XML/XmlParserWrapper.h>

#include "PollData.h"


class TransferData : public PollData
{
public:
	typedef SharedPtr<TransferData> Ptr;

	virtual const std::string getClassName() const
	{
		return "TransferData";
	}

	virtual PollData::Ptr clone() const
	{
		return PollData::Ptr(new TransferData(*this));
	}

	inline TransferData(int msgID, XmlParserWrapper& parser)
	: PollData(msgID)
	{
		std::string nodeName = parser.getElementByPath("")->nodeName();
		type = StringTokenizer(nodeName, ":")[0];

		name = parser.getInnerTextByPath(type + ":name");
		trStatus = parser.getInnerTextByPath(type + ":trStatus");
		reID = parser.getInnerTextByPath(type + ":reID");
		reDate = parser.getInnerTextByPath(type + ":reDate");
		acID = parser.getInnerTextByPath(type + ":acID");
		acDate = parser.getInnerTextByPath(type + ":acDate");
		exDate = parser.getInnerTextByPath(type + ":exDate");
	}

	std::string type;

	std::string name;
	std::string trStatus;
	std::string reID;
	std::string reDate;
	std::string acID;
	std::string acDate;
	std::string exDate;

private:
};


class TransferDataParser: public PollDataParser
{
public:

	virtual bool parsable(XmlParserWrapper& parser)
	{
		std::string nodeName = parser.getRootElement()->nodeName();
		
		if (nodeName == "domain:trnData")
			return true;

		return false;
	}

	virtual PollData::Ptr parse(int msgID, XmlParserWrapper& parser)
	{
		return TransferData(msgID, parser).clone();
	}
};

REGISTER_POLL_DATA_PARSER(TransferDataParser)
