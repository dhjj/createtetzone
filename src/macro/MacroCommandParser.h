#ifndef MacroCommandParser_h__
#define MacroCommandParser_h__

#include <string>
#include "ADDGLBL.h"

namespace boost { 
namespace program_options {
    class options_description;
}
}

class MacroCommandParser
{
public:
    void parse(std::string const& macroCommand);
    ZoneList_t getSourceZones() const;

private:
    void throwUsageError(boost::program_options::options_description const& description);
};

#endif // MacroCommandParser_h__
