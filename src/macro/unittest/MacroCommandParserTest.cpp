#include "GMockInclude.h"
#include <set>
#include <string>
#include "ADDGLBL.h"
#include "../MacroCommandParser.h"

TEST(MacroCommandParserTest, parserCanParseAValidSourceZoneList)
{
    //Given: A macro command containing a valid source zone list
    std::string const macroCommand = "--source-zones=[1-3]";

    //When: The parser is asked to parse and return the source zones
    MacroCommandParser parser;
    parser.parse(macroCommand);

    //Then: It responds with a ZoneList_t containing zones 1, 2, and 3
    ZoneList_t expectedSourceZones;
    expectedSourceZones.push_back(1);
    expectedSourceZones.push_back(2);
    expectedSourceZones.push_back(3);
    ASSERT_TRUE(expectedSourceZones == parser.getSourceZones());
}