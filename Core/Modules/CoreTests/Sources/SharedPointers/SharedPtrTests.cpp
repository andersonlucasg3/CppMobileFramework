#include "SharedPtrTests.h"

#include "Assert.h"

#include "SmartPointer/SharedPointer.h"
#include "SmartPointer/WeakPointer.h"
#include "SmartPointer/MakeAndCasts.h"

REGISTER_TEST_CLASS(SharedPtrTests);

class ITestType 
{
public:
    virtual ~ITestType() = default;

    virtual int MyTestMethod() = 0;
};

class CTestType : public ITestType 
{
public:
    ~CTestType() override = default;

    virtual int MyTestMethod() override { return 1234; }
};

void CSharedPtrTests::TestCase() 
{
    TSharedPtr<int> NullSharedPtr;

    ASSERT_EQUAL(NullSharedPtr, nullptr);
    ASSERT_FALSE(NullSharedPtr);

    int *SharedPtr = nullptr;

    {
        TSharedPtr<int> SharedIntPtr = MakeShared<int>();
        SharedPtr = SharedIntPtr.Raw();

        ASSERT_EQUAL(*SharedIntPtr, 0);
        ASSERT_TRUE(SharedIntPtr);
        ASSERT_EQUAL(*SharedIntPtr, 0);

        *SharedIntPtr = 10;

        ASSERT_EQUAL(*SharedIntPtr, 10);

        *SharedIntPtr = 20;

        ASSERT_EQUAL(*SharedIntPtr, 20);

        ASSERT_EQUAL(SharedIntPtr.RefCount(), 1);

        {
            TSharedPtr<int> CopySharedIntPtr = SharedIntPtr;

            ASSERT_EQUAL(SharedIntPtr, CopySharedIntPtr);

            ASSERT_EQUAL(CopySharedIntPtr.RefCount(), 2);
        }

        ASSERT_EQUAL(SharedIntPtr.RefCount(), 1);
    }

    // testing inheritance

    TSharedPtr<ITestType> TestTypePtr = MakeShared<CTestType>();

    ASSERT_TRUE(TestTypePtr);

    // testing equality

    TSharedPtr<CTestType> OtherTestTypePtr = StaticCastSharedPtr<CTestType>(TestTypePtr);

    ASSERT_EQUAL(OtherTestTypePtr->MyTestMethod(), 1234);

    ASSERT_EQUAL(TestTypePtr, OtherTestTypePtr);

    // testing assertion

    // Uncomment to test assertion
    // OtherTestTypePtr = TestTypePtr;

    // testing weak pointers

    TWeakPtr<int> WeakPtr1;
    TWeakPtr<int> WeakPtr2;
    TWeakPtr<int> WeakPtr3;

    ASSERT_FALSE(WeakPtr1);
    ASSERT_EQUAL(WeakPtr1, nullptr);

    {
        TSharedPtr<int> TempPtr = MakeShared<int>(20);

        WeakPtr1 = TempPtr;
        WeakPtr2 = TempPtr;
        WeakPtr3 = WeakPtr2;

        ASSERT_TRUE(WeakPtr1);
        ASSERT_EQUAL(WeakPtr1, TempPtr);
        ASSERT_EQUAL(*WeakPtr1, 20);
    }

    ASSERT_FALSE(WeakPtr1);
    ASSERT_FALSE(WeakPtr2);
    ASSERT_FALSE(WeakPtr3);

    TWeakPtr<CTestType> WeakObject = OtherTestTypePtr;

    TWeakPtr<ITestType> OtherWeakObject = OtherTestTypePtr;

    {
        TWeakPtr<CTestType> AnotherWeakObject =
            StaticCastWeakPtr<CTestType>(OtherWeakObject);
    }
}