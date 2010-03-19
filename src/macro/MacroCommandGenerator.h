#ifndef MacroCommandGenerator_h__
#define MacroCommandGenerator_h__

#include <string>
#include "ADDGLBL.h"

class MacroCommandGenerator
{
public:
    std::string generateCommand(ZoneList_t const& zoneList) const;
};

#endif // MacroCommandGenerator_h__
