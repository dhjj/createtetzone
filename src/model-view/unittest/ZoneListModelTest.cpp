#include "GMockInclude.h"
#include "ZoneListModel.h"
#include "StateChangeEventMock.h"
#include "StateChangeNotifierMock.h"
#include "ListModelObserverMock.h"

namespace tbx = tecplot::toolbox;
using namespace testing;

/**
 * Some globals to satisfy the linker, since we're not linking against the engine or tptoolbox.
 */
AddOn_pa AddOnID;
LINKTOADDON void STDCALL TecUtilLockStart(AddOn_pa) {}
LINKTOADDON void STDCALL TecUtilLockFinish(AddOn_pa) {}
namespace tecplot
{
    namespace toolbox
    {
        TPTOOLBOX_DLLAPI std::ostream& operator<<(std::ostream& outputStream,
            const Set&    /*set*/)
        {
            return outputStream;
        }

    }
}

class TecUtilDataSetMock
    : public TecUtilDataSetInterface
{
public:
    virtual EntIndex_t numZones() const
    {
        return m_numZones;
    }

    virtual std::string zoneName(EntIndex_t zoneNumber) const
    {
        m_zoneNumRequested = zoneNumber;
        return m_zoneName;
    }

    virtual ZoneList_t enabledZones() const
    {
        return m_enabledZones;
    }

    mutable EntIndex_t m_zoneNumRequested;

    EntIndex_t m_numZones;
    std::string m_zoneName;
    ZoneList_t m_enabledZones;
};

// Default setup just creates standard mocks for dependency injection
class ZoneListModelDefaultSetupTest
    : public testing::Test
{
public:
    void SetUp()
    {
        dataSetMock.reset(new TecUtilDataSetMock);
        notifierMock.reset(new tbx::StateChangeNotifierMock);
    }

protected:
    std::tr1::shared_ptr<TecUtilDataSetMock> dataSetMock;
    std::tr1::shared_ptr<tbx::StateChangeNotifierMock> notifierMock;
};

TEST_F(ZoneListModelDefaultSetupTest, modelReportsItsSizeEqualToTheNumberOfZonesInTheDataSet)
{
    //Given: Dataset contains a single zone, zone 10, named "Zone Name"
    EntIndex_t const expectedNumZones = 10;
    dataSetMock->m_numZones = expectedNumZones;

    //When: the model size is queried
    //Then: the model responds with the number of zones in the dataset
    ZoneListModel model(dataSetMock, notifierMock);
    ASSERT_EQ(expectedNumZones, model.size());
}

TEST_F(ZoneListModelDefaultSetupTest, modelDisplayStringIsTheZoneNumberConcatenatedWithTheZoneName)
{
    //Given: Dataset contains a single zone, zone 10, named "Zone Name"
    EntIndex_t const zoneNumber = 10;
    std::string const zoneName = "Zone Name";
    TecUtilDataSetInterface::ZoneList_t zoneSet;
    zoneSet.push_back(zoneNumber);

    dataSetMock->m_enabledZones = zoneSet;
    dataSetMock->m_zoneName = zoneName;

    //When: Display string for index 0 is requested
    //Then: The model returns the string "10 : Zone Name"
    ZoneListModel model(dataSetMock, notifierMock);
    std::string const expectedDisplayString = "10 : Zone Name";
    ASSERT_EQ(expectedDisplayString, model.displayString(0));
}

class ZoneListModelInactiveZonesTest
    : public ZoneListModelDefaultSetupTest
{
public:
    void SetUp()
    {
        ZoneListModelDefaultSetupTest::SetUp();
        TecUtilDataSetInterface::ZoneList_t zoneSet;
        zoneSet.push_back(1);
        zoneSet.push_back(3);
        dataSetMock->m_enabledZones = zoneSet;
    }
};

TEST_F(ZoneListModelInactiveZonesTest, modelHandlesInactiveZonesForDataRequests)
{
    //Given: Dataset contains two zones: 1 & 3 (zone 2 is inactive)
    //       This is done in test fixture setup

    //When: Data for index 1 is requested
    //Then: The model provides zone 3 as the data
    ZoneListModel model(dataSetMock, notifierMock);
    ASSERT_EQ(3, model.data(1));
}

TEST_F(ZoneListModelInactiveZonesTest, modelHandlesInactiveZonesForDisplayStringRequests)
{
    //Given: Dataset contains two zones: 1 & 3 (zone 2 is inactive)
    //       This is done in test fixture setup

    //When: Display string for index 1 (i.e. zone 3) is requested
    //Then: The model requests the name for zone 3
    ZoneListModel model(dataSetMock, notifierMock);
    model.displayString(1);
    ASSERT_EQ(3, dataSetMock->m_zoneNumRequested);
}

TEST_F(ZoneListModelDefaultSetupTest, whenModelIsConstructed_ItStartsListeningForStateChanges)
{
    //Given: the default setup
    //When: a ZoneListModel is constructed
    //Then: it starts listening to the given state change notifier
    tbx::StateChangeNotifierMock& notifierMockRef = *notifierMock;
    EXPECT_CALL(notifierMockRef, addListener(_));
    ZoneListModel model(dataSetMock, notifierMock);
}

TEST_F(ZoneListModelDefaultSetupTest, whenModelIsDestructed_ItStopsListeningForStateChanges)
{
    //Given: the default setup
    //When: a ZoneListModel is destructed
    //Then: it stops listening to the given state change notifier
    tbx::StateChangeNotifierMock& notifierMockRef = *notifierMock;
    EXPECT_CALL(notifierMockRef, removeListener(_));
    ZoneListModel model(dataSetMock, notifierMock);
}

TEST_F(ZoneListModelDefaultSetupTest, whenStateChangeOccurs_ModelSendsResetMessageToObserver)
{
    //Given: The model has an observer
    ZoneListModel model(dataSetMock, notifierMock);

    ListModelObserverMock observer;
    model.addObserver(observer);

    //When: A state change occurs
    //Then: Model notifies its observers with a Change_ModelReset message
    ListModelGenericInterface::ChangeInfo_s change(ListModelGenericInterface::ModelReset);
    EXPECT_CALL(observer, modelChanged(Eq(change)));

    tbx::StateChangeEventMock event;
    model.stateChangeCallback(tbx::StateChangeEventMock(), *notifierMock);
}

TEST_F(ZoneListModelDefaultSetupTest, whenStateChangeOccurs_ModelRebuildsItself)
{
    //Given: A dataset with a single zone 5 is initially active
    dataSetMock->m_enabledZones.push_back(5);
    ZoneListModel model(dataSetMock, notifierMock);

    //When: A state change occurs, and dataset with a single zone 10 is now active
    //Then: A request for data at index 0 returns 10
    dataSetMock->m_enabledZones.clear();
    EntIndex_t const expectedZoneNumber = 10;
    dataSetMock->m_enabledZones.push_back(expectedZoneNumber);
    model.stateChangeCallback(tbx::StateChangeEventMock(), *notifierMock);

    ASSERT_EQ(expectedZoneNumber, model.data(0));
}

//Given:
//When:
//Then:
