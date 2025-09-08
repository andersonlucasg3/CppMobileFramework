#include "ObjectCollectorTests.h"

#include "Assert.h"

#include "Logger/Logger.h"
#include "Object/Object.h"
#include "Object/Collector/CollectorScope.h"
#include "Object/Collector/ObjectCollector.h"

REGISTER_TEST_CLASS(ObjectCollectorTests);

class CMyTestObject : public CObject
{
public:
    PROPERTY(CObject, Object1);
    ARRAYPROPERTY(CObject, ObjectArray1);

    CMyTestObject() = default;
    ~CMyTestObject() override = default;
};

class CRootedTestObject : public CObject
{
public:
    PROPERTY(CMyTestObject, MyTestObject);

    CRootedTestObject() : CObject()
    {
        MakeRooted();
    }
};

void CObjectCollectorTests::TestCase()
{
    CRootedTestObject* RootedObject = new CRootedTestObject;

    new CObject;

    ASSERT_EQUAL(GObjectCollector.AliveObjectCount(), 2);

    GObjectCollector.CollectGarbage();
    
    ASSERT_EQUAL(GObjectCollector.AliveObjectCount(), 1);

    {
        SCollectorScope Scope;

        new CObject;

        RootedObject->MyTestObject = new CMyTestObject;

        RootedObject->MyTestObject->Object1 = new CObject;

        ASSERT_EQUAL(GObjectCollector.AliveObjectCount(), 4);
    }

    ASSERT_EQUAL(GObjectCollector.AliveObjectCount(), 3);

    RootedObject->MakeUnrooted();

    GObjectCollector.CollectGarbage();
}