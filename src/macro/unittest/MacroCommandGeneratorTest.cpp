#include "GMockInclude.h"
#include "../MacroCommandGenerator.h"
#include "ADDGLBL.h"

//Given:
//When:
//Then:

TEST(MacroCommandGeneratorTest, generatorCreatesTetrahedralizeCommand)
{
    //Given: a MacroCommandGenerator and a ZoneList containing a zone
    MacroCommandGenerator generator;
    ZoneList_t zoneList;
    zoneList.push_back(1);
    //When: asked for the macro command
    std::string command = generator.generateCommand(zoneList);
    //Then: it returns the correct command
    std::string expectedCommand = "--source-zones=1";
    ASSERT_EQ(expectedCommand, command);
}
