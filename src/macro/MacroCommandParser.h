#ifndef MacroCommandParser_h__
#define MacroCommandParser_h__

#include <string>
#include "ADDGLBL.h"

class MacroCommandParser
{
public:
    void parse(std::string const& macroCommand);
    ZoneList_t getSourceZones() const;
private:
};

#endif // MacroCommandParser_h__
