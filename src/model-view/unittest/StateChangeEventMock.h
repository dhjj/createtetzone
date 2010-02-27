#ifndef StateChangeEventMock_h__
#define StateChangeEventMock_h__

#include "toolbox/StateChangeEventInterface.h"
#include "toolbox/Set.h"
#include "toolbox/StringList.h"

namespace tecplot
{
namespace toolbox
{

class StateChangeEventMock
    : public StateChangeEventInterface
{
public:
    MOCK_CONST_METHOD0(clone,
                       StateChangeEventInterface*());
    MOCK_CONST_METHOD0(hasIndex,
                       bool());
    MOCK_CONST_METHOD0(hasVarSet,
                       bool());
    MOCK_CONST_METHOD0(hasZoneSet,
                       bool());
    MOCK_CONST_METHOD0(hasArbEnum,
                       bool());
    MOCK_CONST_METHOD0(hasMap,
                       bool());
    MOCK_CONST_METHOD0(hasZone,
                       bool());
    MOCK_CONST_METHOD0(hasVar,
                       bool());
    MOCK_CONST_METHOD0(hasName,
                       bool());
    MOCK_CONST_METHOD0(hasPageUniqueID,
                       bool());
    MOCK_CONST_METHOD0(hasFrameUniqueID,
                       bool());
    MOCK_CONST_METHOD0(hasDataSetUniqueID,
                       bool());
    MOCK_CONST_METHOD0(hasUniqueID,
                       bool());
    MOCK_CONST_METHOD0(hasLineMapSet,
                       bool());
    MOCK_CONST_METHOD0(hasFieldMapSet,
                       bool());
    MOCK_CONST_METHOD0(hasStyleParams,
                       bool());
    MOCK_CONST_METHOD0(hasOffset1,
                       bool());
    MOCK_CONST_METHOD0(hasOffset2,
                       bool());
    MOCK_CONST_METHOD0(getStateChange,
                       StateChange_e());
    MOCK_CONST_METHOD0(getIndex,
                       LgIndex_t());
    MOCK_CONST_METHOD0(getVarSet,
                       const Set&());
    MOCK_CONST_METHOD0(getZoneSet,
                       const Set&());
    MOCK_CONST_METHOD0(getLineMapSet,
                       const Set&());
    MOCK_CONST_METHOD0(getFieldMapSet,
                       const Set&());
    MOCK_CONST_METHOD0(getArbEnum,
                       LgIndex_t());
    MOCK_CONST_METHOD0(getStyleParams,
                       StringList&());
    MOCK_CONST_METHOD1(getStyleParam,
                       std::string(int paramIndex));
    MOCK_CONST_METHOD0(getMap,
                       EntIndex_t());
    MOCK_CONST_METHOD0(getZone,
                       EntIndex_t());
    MOCK_CONST_METHOD0(getVar,
                       EntIndex_t());
    MOCK_CONST_METHOD0(getName,
                       std::string());
    MOCK_CONST_METHOD0(getPageUniqueID,
                       UniqueID_t());
    MOCK_CONST_METHOD0(getFrameUniqueID,
                       UniqueID_t());
    MOCK_CONST_METHOD0(getDataSetUniqueID,
                       UniqueID_t());
    MOCK_CONST_METHOD0(getUniqueID,
                       UniqueID_t());
    MOCK_CONST_METHOD0(getOffset1,
                       LgIndex_t());
    MOCK_CONST_METHOD0(getOffset2,
                       LgIndex_t());
};

}  // namespace toolbox
}  // namespace tecplot

#endif // StateChangeEventMock_h__
