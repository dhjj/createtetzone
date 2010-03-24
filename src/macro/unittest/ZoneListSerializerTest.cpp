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
#include "../ZoneListSerializer.h"
#include "Error.h"

//Given:
//When:
//Then:

TEST(ZoneListSerializerTest, whenSerializerIsGivenAStringContainingInvalidCharacter_ItThrowsError)
{
    //Given: an input string with an invalid character
    std::string const input = "a";

    //When: the serializer is asked to deserialize
    //Then: it throws an Error
    ASSERT_THROW(ZoneListSerializer().deserialize(input), Error);
}

TEST(ZoneListSerializerTest, whenSerializerIsGivenAnEmptyString_ItReturnsAnEmptyZoneList)
{
    //Given: an empty input string
    std::string const input;

    //When: the serializer is asked to deserialize
    //Then: it returns and emtpy zone list
    ZoneList_t zoneList = ZoneListSerializer().deserialize(input);
    ASSERT_TRUE(zoneList == ZoneList_t());
}

TEST(ZoneListSerializerTest, whenSerializerIsGivenAStringOfIndividualItems_ItReturnsThoseItemsInAZoneList)
{
    //Given: A String Of Individual Items
    std::string const input = "1,2,10";

    //When: the serializer is asked to deserialize
    ZoneList_t zoneList = ZoneListSerializer().deserialize(input);

    //Then: It Returns Those Items In A ZoneList
    ZoneList_t expectedZoneList;
    expectedZoneList.push_back(1);
    expectedZoneList.push_back(2);
    expectedZoneList.push_back(10);
    ASSERT_TRUE(zoneList == expectedZoneList);
}

TEST(ZoneListSerializerTest, whenSerializerIsGivenAStringContainingARange_ItReturnsAZoneListContainingAllZonesInTheRange)
{
    //Given: A String Containing A Range
    std::string const input = "1-3";

    //When: the serializer is asked to deserialize
    ZoneList_t zoneList = ZoneListSerializer().deserialize(input);

    //Then: It Returns A Zone List Containing All Zones In The Range
    ZoneList_t expectedZoneList;
    expectedZoneList.push_back(1);
    expectedZoneList.push_back(2);
    expectedZoneList.push_back(3);
    ASSERT_TRUE(zoneList == expectedZoneList);
}

TEST(ZoneListSerializerTest, whenSerializerIsGivenAStringContainingAnInvalidRange_ItThrowsError)
{
    //Given: A String Containing An InvalidRange
    std::string const input = "3-1";

    //When: the serializer is asked to deserialize
    //Then: it throws an Error
    ASSERT_THROW(ZoneListSerializer().deserialize(input), Error);
}

TEST(ZoneListSerializerTest, whenSerializerIsGivenAnEmptyZoneList_ItReturnsAnEmptyString)
{
    //Given: An Empty Zone List
    ZoneList_t zoneList;

    //When: the serializer is asked to serialize
    //Then: it returns an empty string
    ASSERT_EQ(std::string(), ZoneListSerializer().serialize(zoneList));
}

TEST(ZoneListSerializerTest, whenSerializerIsGivenAZoneListWithOneZone_ItReturnsAStringContainingTheZoneNumber)
{
    //Given: AZoneListWithOneZone
    ZoneList_t zoneList;
    zoneList.push_back(10);

    //When: the serializer is asked to serialize
    //Then: ItReturnsAStringContainingTheZoneNumber
    ASSERT_EQ(std::string("10"), ZoneListSerializer().serialize(zoneList));
}

TEST(ZoneListSerializerTest, whenSerializerIsGivenAZoneListWithNonSequentialZones_ItReturnsAStringContainingCommaSeparateZoneNumbers)
{
    //Given: AZoneListWithNonSequentialZones
    ZoneList_t zoneList;
    zoneList.push_back(1);
    zoneList.push_back(3);

    //When: the serializer is asked to serialize
    //Then: ItReturnsAStringContainingCommaSeparateZoneNumbers
    ASSERT_EQ(std::string("1,3"), ZoneListSerializer().serialize(zoneList));
}

TEST(ZoneListSerializerTest, whenSerializerIsGivenAZoneListWithSequentialZones_ItReturnsAStringSpecifyingTheZoneRange)
{
    //Given: AZoneListWithSequentialZones
    ZoneList_t zoneList;
    zoneList.push_back(1);
    zoneList.push_back(2);
    zoneList.push_back(3);

    //When: the serializer is asked to serialize
    //Then: ItReturnsAStringContainingCommaSeparateZoneNumbers
    ASSERT_EQ(std::string("1-3"), ZoneListSerializer().serialize(zoneList));
}

TEST(ZoneListSerializerTest, whenSerializerIsGivenAMixOfSequentialAndNonSequentialZones_ItReturnsAStringMixingZoneRangesWithCommaSeparateZoneNumbers)
{
    //Given: AMixOfSequentialAndNonSequentialZones
    ZoneList_t zoneList;
    zoneList.push_back(1);
    zoneList.push_back(2);
    zoneList.push_back(3);
    zoneList.push_back(10);
    zoneList.push_back(12);
    zoneList.push_back(13);
    zoneList.push_back(14);
    zoneList.push_back(16);

    //When: the serializer is asked to serialize
    //Then: ItReturnsAStringMixingZoneRangesWithCommaSeparateZoneNumbers
    ASSERT_EQ(std::string("1-3,10,12-14,16"), ZoneListSerializer().serialize(zoneList));
}
