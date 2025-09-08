#include "ObjectCollectorTests.h"

#include "Assert.h"

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

    CRootedTestObject() = default;
    ~CRootedTestObject() override = default;
};

class CProxyTestObject : public CObject
{
public:
    PROPERTY(CRootedTestObject, RootedObject);

    CProxyTestObject() = default;
};

static void BasicGCTests(CAssert* Assert)
{
    CRootedTestObject* RootedObject = new CRootedTestObject;
    RootedObject->MakeRooted();

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

    ASSERT_EQUAL(GObjectCollector.AliveObjectCount(), 0);
}

static void TestCyclicCollect(CAssert* Assert)
{
    CRootedTestObject* RootedObject = new CRootedTestObject;
    RootedObject->MakeRooted();

    CMyTestObject* MyTestObject = new CMyTestObject;

    CProxyTestObject* ProxyObject = new CProxyTestObject;

    RootedObject->MyTestObject = MyTestObject;

    MyTestObject->Object1 = ProxyObject;
    MyTestObject->ObjectArray1.Add(ProxyObject);

    ProxyObject->RootedObject = RootedObject;

    ASSERT_EQUAL(GObjectCollector.AliveObjectCount(), 3);

    GObjectCollector.CollectGarbage();

    ASSERT_EQUAL(GObjectCollector.AliveObjectCount(), 3);

    RootedObject->MakeUnrooted();

    GObjectCollector.CollectGarbage();

    ASSERT_EQUAL(GObjectCollector.AliveObjectCount(), 0);

    (new CRootedTestObject())->MakeRooted();
    (new CRootedTestObject())->MakeRooted();
    (new CRootedTestObject())->MakeRooted();

    ASSERT_EQUAL(GObjectCollector.AliveObjectCount(), 3);

    // Will be deallocated on GC destructor
}

void CObjectCollectorTests::TestCase()
{
    BasicGCTests(Assert);
    TestCyclicCollect(Assert);
}