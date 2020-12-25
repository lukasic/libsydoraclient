#pragma once

#include <iostream>
#include <vector>
#include <map>

#include <EPP/Request.h>
#include <EPP/Command.h>
#include <EPP/XML/XmlHelpers.h>
#include <EPP/XML/XmlParserWrapper.h>

#include <Poco/StringTokenizer.h>
using Poco::StringTokenizer;

#include <Poco/DOM/AutoPtr.h>
#include <Poco/DOM/Document.h>
#include <Poco/DOM/Element.h>
using namespace Poco::XML;

class PollData
{
public:
	typedef Poco::SharedPtr<PollData> Ptr;

	inline PollData(int msgID)
		: msgID(msgID)
	{}

	virtual const std::string getClassName() const = 0;
	virtual PollData::Ptr clone() const = 0;

	const int msgID;
};


class PollDataParser
{
public:
	typedef SharedPtr<PollDataParser> Ptr;

	virtual bool parsable(XmlParserWrapper& parser) = 0;
	virtual PollData::Ptr parse(int msgID, XmlParserWrapper& parser) = 0;
};

class PollReqResponseDataParser
{
public:
	static std::list<PollDataParser::Ptr>* parsers;

	static inline void registerParser(PollDataParser::Ptr p)
	{
		if (parsers == nullptr)
			parsers = new std::list<PollDataParser::Ptr>();

		parsers->push_back(p);
	}

	static inline PollData::Ptr parse(int msgID, XmlParserWrapper& xmlParser)
	{
		if (parsers == nullptr)
			throw EPP::Exception("No parser registered.");

		for each (auto dataParser in *parsers)
			if (dataParser->parsable(xmlParser))
				return dataParser->parse(msgID, xmlParser);

		std::cout << "Parsers count: " << parsers->size() << std::endl;

		std::string nodeName = xmlParser.getRootElement()->nodeName();
		throw EPP::Exception("Cannot parse poll data. Data parser not found");
	}

};

template<class T>
class PollDataParser_Registrator
{
public:
	static bool reg;

	static inline bool init()
	{
		PollReqResponseDataParser::registerParser(new T());
		return true;
	}
};

template<class T>
bool PollDataParser_Registrator<T>::reg = PollDataParser_Registrator<T>::init();

#define REGISTER_POLL_DATA_PARSER(cls) bool PollDataParser_Registrator<cls>::reg = PollDataParser_Registrator<cls>::init();
