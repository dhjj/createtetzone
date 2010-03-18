#include "ZoneListSerializer.h"
#include <sstream>
// Avoid warnings in headers that are out of our control
#if defined(_MSC_VER)
# pragma warning (push, 1)
#endif
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#if defined(_MSC_VER)
# pragma warning(pop)
#endif
#include "Error.h"

namespace ba = boost::algorithm;

ZoneListSerializer::ZoneRange_t ZoneListSerializer::getZoneRangeFromString(std::string &curString) const
{
    ZoneList_t::value_type beginRange = 0;
    ZoneList_t::value_type endRange = 0;
    std::string::size_type position = curString.find('-');
    try
    {
        if (position != std::string::npos)
        {
            // This string is a range, like "5-9"
            beginRange = boost::lexical_cast<ZoneList_t::value_type>(curString.substr(0, position));
            endRange = boost::lexical_cast<ZoneList_t::value_type>(curString.substr(position + 1));
        }
        else
        {
            beginRange = boost::lexical_cast<ZoneList_t::value_type>(curString);
            endRange = beginRange;
        }
    }
    catch (boost::bad_lexical_cast const&)
    {
        throw Error("Invalid set string specification.");
    }

    if (beginRange < 1 || endRange < beginRange)
        throw Error("Invalid set string specification.");

    return std::make_pair(beginRange, endRange);
}

std::string ZoneListSerializer::serialize(ZoneList_t const& zoneList) const
{
    std::ostringstream outStream;
    ZoneList_t::value_type lastValue = 0;
    bool onARun = false;

    const char separator = ',';
    const char rangeSeparator = '-';

    ZoneList_t::const_iterator it = zoneList.begin();
    while (it != zoneList.end())
    {
        if (it == zoneList.begin())
        {
            /* First time through the loop. */
            outStream << *it;
        }
        else if ((*it - lastValue) == 1)
        {
            onARun = true;
        }
        else
        {
            if (onARun)
            {
                // Finish off the last range
                outStream << rangeSeparator << lastValue;
            }

            outStream << separator << *it;

            onARun = false;
        }
        lastValue = *it;
        ++it;
    }

    if (onARun)
    {
        // Finish off the last range
        outStream << rangeSeparator << lastValue;
    }

    return outStream.str();
}

/**
 * Adapted from code from the strand editor add-on.
 *  A set must have the form: (s[,s][,s]...)
 *  where s can be one of:  n or n-m
 *  container can be any standard container.
 */
ZoneList_t ZoneListSerializer::deserialize(std::string const& zoneListString) const
{
    std::string subString(zoneListString); // Make a copy we can modify
    const static std::string validChars("0123456789,-");

    ba::erase_all(subString, " ");
    ba::trim_if(subString, ba::is_any_of("[]"));

    // Make sure we have no invalid characters
    if (subString.find_first_not_of(validChars) != std::string::npos)
        throw Error("Invalid character found in source zone set string.");

    ZoneList_t result;
    if (subString.empty())
        return result;

    typedef std::vector<std::string> TokenList_t;
    TokenList_t tokens;
    ba::split(tokens, subString, ba::is_any_of(","), ba::token_compress_on);

    for (TokenList_t::size_type curToken = 0; curToken < tokens.size(); curToken++)
    {
        std::string& curString = tokens.at(curToken);
        ZoneRange_t zoneRange = getZoneRangeFromString(curString);

        for (ZoneList_t::value_type curVal = zoneRange.first; curVal <= zoneRange.second; ++curVal)
            result.push_back(curVal);
    }
    return result;
}
