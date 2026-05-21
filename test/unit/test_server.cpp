#include "test_fixture.h"

#include <fstream>

TEST_F(ShoppingSystemTest, ServerModule_GetActivityReadsClientsFile)
{
    {
        std::ofstream activity("clients.txt");
        activity << "Client 1 connected from 127.0.0.1:1234\n";
    }

    EXPECT_NE(thread->getActivity().find("127.0.0.1"), std::string::npos);
}

TEST_F(ShoppingSystemTest, ServerModule_GetActivityHandlesMissingFile)
{
    std::filesystem::remove("clients.txt");
    EXPECT_TRUE(thread->getActivity().empty());
}
