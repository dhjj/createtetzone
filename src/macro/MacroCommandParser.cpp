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
#include "MacroCommandParser.h"
#include <vector>
// Avoid warnings in headers that are out of our control
#if defined(_MSC_VER)
# pragma warning (push, 1)
#endif
#include "boost/program_options.hpp"
#include <boost/algorithm/string.hpp>
#if defined(_MSC_VER)
# pragma warning(pop)
#endif
#include "Error.h"
#include "ZoneListSerializer.h"

namespace po = boost::program_options;
namespace ba = boost::algorithm;

struct MacroCommandParserImpl
    : boost::noncopyable
{
    MacroCommandParserImpl();

    void throwUsageError(boost::program_options::options_description const& description) const;
    void validateOptions(boost::program_options::options_description const& description);
    std::string zoneCommandString() const;

    std::string const m_sourceZoneCommandString;
    po::variables_map m_variablesMap;
};

MacroCommandParserImpl::MacroCommandParserImpl()
: m_sourceZoneCommandString("source-zones")
{
}

MacroCommandParser::MacroCommandParser()
: m_impl(new MacroCommandParserImpl)
{
}

MacroCommandParser::~MacroCommandParser()
{
}

void MacroCommandParser::parse(std::string const& macroCommand)
{
    typedef std::vector<std::string> TokenList_t;
    TokenList_t arguments;
    ba::split(arguments, macroCommand, ba::is_any_of(" "), ba::token_compress_on);

    po::options_description description("Allowed options");
    description.add_options()
        ("source-zones", po::value<std::string>(), "The list of zones to use as the source for the tetrahedralization");

    try
    {
        po::store(po::command_line_parser(arguments).options(description).run(), m_impl->m_variablesMap);
        po::notify(m_impl->m_variablesMap);
    }
    catch (po::error const&)
    {
        m_impl->throwUsageError(description);
    }

    m_impl->validateOptions(description);
}

ZoneList_t MacroCommandParser::getSourceZones() const
{
    std::string const& commandString = m_impl->m_variablesMap[zoneCommandString()].as<std::string>();
    return ZoneListSerializer().deserialize(commandString);
}

void MacroCommandParserImpl::throwUsageError(po::options_description const& description) const
{
    std::ostringstream usageMessage;
    description.print(usageMessage);
    throw Error(usageMessage.str());
}

void MacroCommandParserImpl::validateOptions(po::options_description const& description)
{
    if (m_variablesMap.count(zoneCommandString()) == 0)
        throwUsageError(description);
}

std::string MacroCommandParserImpl::zoneCommandString() const
{
    return m_sourceZoneCommandString;
}

std::string MacroCommandParser::zoneCommandString() const
{
    return m_impl->zoneCommandString();
}
