#include <Sydora/Domain/Domain.h>

using namespace Sydora;

DomainCreated::DomainCreated(XmlParserWrapper& parser)
{
	name = parser.getInnerTextByPath("domain:name");
	crDate = parser.getInnerTextByPath("domain:crDate");
	exDate = parser.getInnerTextByPath("domain:exDate");
}

DomainRenewed::DomainRenewed(XmlParserWrapper& parser)
{
	name = parser.getInnerTextByPath("domain:name");
	exDate = parser.getInnerTextByPath("domain:exDate");
}

