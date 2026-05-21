#include "test_fixture.h"

TEST_F(ShoppingSystemTest, ThreadModule_TestModeSendAndReceive)
{
    thread->clearSentMessages();
    thread->pushRecv("hello");
    thread->pushRecv("world");

    std::string received;
    EXPECT_GT(thread->Rec(received), 0);
    EXPECT_EQ(received, "hello");
    EXPECT_GT(thread->Rec(received), 0);
    EXPECT_EQ(received, "world");

    EXPECT_GT(thread->Send("response"), 0);
    ASSERT_EQ(thread->sentMessages().size(), 1u);
    EXPECT_EQ(thread->sentMessages().front(), "response");

    EXPECT_GT(thread->Send(42), 0);
    EXPECT_GT(thread->Send(3.14), 0);
}

TEST_F(ShoppingSystemTest, ThreadModule_StateFlags)
{
    Thread worker(server.get(), 1);
    EXPECT_TRUE(worker.available());
    EXPECT_TRUE(worker.isFree());
    EXPECT_FALSE(worker.isDestroyed());

    worker.setFree();
    EXPECT_TRUE(worker.isFree());
    EXPECT_TRUE(worker.isDestroyed());
}
