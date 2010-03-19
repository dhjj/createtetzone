#include "MacroCommandGenerator.h"
#include <sstream>
#include "MacroCommandParser.h"
#include "ZoneListSerializer.h"

std::string MacroCommandGenerator::generateCommand(ZoneList_t const& zoneList) const
{
    std::ostringstream command;
    command << "--" << MacroCommandParser().zoneCommandString() << '=' << ZoneListSerializer().serialize(zoneList);
    return command.str();
}
