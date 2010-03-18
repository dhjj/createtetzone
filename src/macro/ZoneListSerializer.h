#ifndef ZoneListSerializer_h__
#define ZoneListSerializer_h__

#include <string>
#include <vector>
#include <utility>
#include "ADDGLBL.h"

class ZoneListSerializer
{
public:
    std::string serialize(ZoneList_t const& zoneList) const;
    ZoneList_t deserialize(std::string const& zoneListString) const;
private:
    typedef std::pair<EntIndex_t, EntIndex_t> ZoneRange_t;
    ZoneRange_t getZoneRangeFromString(std::string &curString) const;
};

#endif // ZoneListSerializer_h__