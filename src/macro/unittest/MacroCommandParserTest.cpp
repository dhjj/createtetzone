/*
 * Copyright (c) Tecplot, Inc.
 *
 * All rights reserved.  Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   - Redistributions of source code must retain the above copyright notice, this list of
 *     conditions and the following disclaimer.
 *   - Redistributions in binary form must reproduce the above copyright notice, this list
 *     of conditions and the following disclaimer in the documentation and/or other
 *     materials provided with the distribution.
 *   - Neither the name of the Tecplot, Inc., nor the names of its contributors may be
 *     used to endorse or promote products derived from this software without specific
 *     prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
 * THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
 * THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
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
