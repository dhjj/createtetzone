#ifndef StateChangeNotifierMock_h__
#define StateChangeNotifierMock_h__

#include "toolbox/StateChangeNotifierInterface.h"
#include "toolbox/StateChangeListenerInterface.h"

namespace tecplot
{
namespace toolbox
{

class StateChangeNotifierMock
        : public StateChangeNotifierInterface
{
public:
    MOCK_METHOD1(addListener,
                 void(StateChangeListenerInterface * const listener));
    MOCK_METHOD1(removeListener,
                 void(StateChangeListenerInterface * const listener));
    MOCK_METHOD1(isListening,
                 bool(StateChangeListenerInterface * const listener));
    MOCK_METHOD1(setMode,
                 bool(StateChangeMode_e mode));
    MOCK_METHOD0(mode,
                 StateChangeMode_e());
private:
    MOCK_METHOD0(lockStart,
                 void());
    MOCK_METHOD0(lockFinish,
                 void());
};

}  // namespace toolbox
}  // namespace tecplot

#endif // StateChangeNotifierMock_h__
