#include "GMockInclude.h"
#include <set>
#include <string>
#include "ADDGLBL.h"
#include "../MacroCommandParser.h"
#include "Error.h"

//Given:
//When:
//Then:

TEST(MacroCommandParserTest, parserGivesCorrectSourceZonesCommandString)
{
    //Given: a MacroCommandParser
    MacroCommandParser parser;

    //When: asked for the source zones command string
    //Then: it returns "source-zones"
    std::string expectedCommand = "source-zones";
    ASSERT_EQ(expectedCommand, parser.zoneCommandString());
}

TEST(MacroCommandParserTest, parserThrowsErrorWhenGivenInvalidInputString)
{
    //Given: A macro command containing an invalid input
    std::string const macroCommand = "!@#$)!#@$";

    //When: The parser is asked to parse
    //Then: It throws an exception of type Error
    MacroCommandParser parser;
    ASSERT_THROW(parser.parse(macroCommand), Error);
}

TEST(MacroCommandParserTest, parserThrowsErrorWhenGivenEmptyInputString)
{
    //Given: A macro command containing an empty input
    std::string const macroCommand;

    //When: The parser is asked to parse
    //Then: It throws an exception of type Error
    MacroCommandParser parser;
    ASSERT_THROW(parser.parse(macroCommand), Error);
}

TEST(MacroCommandParserTest, parserCanParseAValidSourceZoneList)
{
    //Given: A macro command containing a valid source zone list
    std::string const macroCommand = "--source-zones=1-3";

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

//Given:
//When:
//Then:
