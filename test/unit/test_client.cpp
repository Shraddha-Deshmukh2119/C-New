#include <gtest/gtest.h>

#include "Client.h"

// Client networking module (compile-time linkage coverage).
TEST(ClientModule, ConstructAndDestructSafely)
{
    Client client("127.0.0.1", 17777);
    SUCCEED();
}

TEST(ClientModule, SendHelpersCompileWithInvalidSocket)
{
    Client client;
    EXPECT_EQ(client.Send("ping"), -1);
    EXPECT_EQ(client.Send(10), -1);
    EXPECT_EQ(client.Send(2.5), -1);

    std::string received;
    EXPECT_EQ(client.Rec(received), -1);
}
