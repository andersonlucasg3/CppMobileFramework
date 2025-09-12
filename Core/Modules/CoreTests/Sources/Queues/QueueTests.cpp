#include "QueueTests.h"

#include "Assert.h"

#include "Templates/Queue.h"

#include "SmartPointer/MakeAndCasts.h"

REGISTER_TEST_CLASS(QueueTests);

inline void TestIntQueue(CAssert* Assert)
{
	TQueue<int> IntQueue;

	ASSERT_TRUE(IntQueue.IsEmpty());

	IntQueue.Enqueue(0);

	ASSERT_FALSE(IntQueue.IsEmpty());

	int Value;
	ASSERT_TRUE(IntQueue.Peak(Value));
	ASSERT_EQUAL(Value, 0);

	ASSERT_FALSE(IntQueue.IsEmpty());

	for (int32_t i = 1; i <= 10; i++)
	{
		IntQueue.Enqueue(i);
	}

	for (int i = 0; i <= 10; i++)
	{
		ASSERT_TRUE(IntQueue.Dequeue(Value));
		ASSERT_EQUAL(Value, i);
	}
}

inline void TestSharedPtrQueue(CAssert* Assert)
{
	TQueue<TSharedPtr<int>> QueueOfPtrs;

	QueueOfPtrs.Enqueue(MakeShared<int>(1));
	QueueOfPtrs.Enqueue(MakeShared<int>(2));

	TSharedPtr<int> Deq;
	QueueOfPtrs.Dequeue(Deq);

	ASSERT_EQUAL(*Deq, 1);
}

inline void TestInterfaceQueue()
{
	class IInterfaceTest
	{
	public:
		virtual ~IInterfaceTest() = default;
		virtual void TestMethod() = 0;
	};

	class CConcreteTest : public IInterfaceTest
	{
	public:
		~CConcreteTest() override = default;
		void TestMethod() override { }
	};

	TQueue<TSharedPtr<IInterfaceTest>> Queue;

	Queue.Enqueue(MakeShared<CConcreteTest>());
	Queue.Enqueue(MakeShared<CConcreteTest>());

	TSharedPtr<IInterfaceTest> Ptr;
	Queue.Dequeue(Ptr);
}

void CQueueTests::TestCase()
{
    TestIntQueue(Assert);

	TestSharedPtrQueue(Assert);

	TestInterfaceQueue();
}