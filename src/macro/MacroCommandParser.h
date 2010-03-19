#ifndef MacroCommandParser_h__
#define MacroCommandParser_h__

#include <string>
#include <memory>
#include <boost/noncopyable.hpp>
#include "ADDGLBL.h"

class MacroCommandParser
    : public boost::noncopyable
{
public:
    MacroCommandParser();
    ~MacroCommandParser();
    void parse(std::string const& macroCommand);
    ZoneList_t getSourceZones() const;

    std::string zoneCommandString() const;

private:
    std::auto_ptr<struct MacroCommandParserImpl> m_impl;
};

#endif // MacroCommandParser_h__
