#include "../ListModelObserverInterface.h"

class ListModelObserverMock
    : public ListModelObserverInterface
{
public:
    MOCK_METHOD1(modelChanged,
                 void(const ListModelGenericInterface::ChangeInfo_s& info));
};
