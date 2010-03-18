#include "MacroCommandParser.h"
#include <vector>
// Avoid warnings in headers that are out of our control
#if defined(_MSC_VER)
# pragma warning (push, 1)
#endif
#include "boost/program_options.hpp"
#if defined(_MSC_VER)
# pragma warning(pop)
#endif

namespace po = boost::program_options;

void MacroCommandParser::parse(std::string const& macroCommand)
{
    std::vector<std::string> arguments = po::split_winmain(macroCommand);

    po::options_description description("Allowed options");
    description.add_options()
        ("source-zones", po::value<int>(), "set compression level");

    po::variables_map variablesMap;
    po::store(po::command_line_parser(arguments).options(description).run(), variablesMap);
    po::notify(variablesMap);
}

ZoneList_t MacroCommandParser::getSourceZones() const
{
    return ZoneList_t();
}

//std::vector<std::string> MacroCommandParser::split