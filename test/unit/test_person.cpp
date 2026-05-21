#include "test_fixture.h"

#include "Customer.h"

TEST_F(ShoppingSystemTest, PersonModule_LoginSuccessAndProfile)
{
    Customer customer(*thread);

    queueRecv(*thread, {
        "cust1", "pass1", // login credentials
        "3",              // profile from home menu
        "4"               // logout from home
    });

    customer.login("customer.txt");

    bool profileSent = false;
    for (const auto& msg : thread->sentMessages())
    {
        if (msg.find("Balance:") != std::string::npos)
            profileSent = true;
    }
    EXPECT_TRUE(profileSent);
}

TEST_F(ShoppingSystemTest, PersonModule_LoginWrongUserThenSuccess)
{
    Customer customer(*thread);

    queueRecv(*thread, {
        "unknown", "bad", // first failed attempt
        "cust1", "pass1", // second successful attempt
        "4"               // exit home
    });

    customer.login("customer.txt");

    EXPECT_EQ(lastSent(*thread), "Correct");
}

TEST_F(ShoppingSystemTest, PersonModule_BuyAddItemAndConfirm)
{
    Customer customer(*thread);

    queueRecv(*thread, {
        "cust1", "pass1",
        "1",       // home: buy
        "1",       // buy: add to cart
        "1",       // product index
        "2",       // quantity
        "2",       // buy: confirm order
        "3",       // buy: exit
        "4"        // home: logout
    });

    customer.login("customer.txt");

    bool enoughResponse = false;
    bool orderConfirmed = false;
    for (const auto& msg : thread->sentMessages())
    {
        if (msg == "ENOUGH")
            enoughResponse = true;
        if (msg.find("Cost of the order") != std::string::npos)
            orderConfirmed = true;
    }

    EXPECT_TRUE(enoughResponse);
    EXPECT_TRUE(orderConfirmed);
}

TEST_F(ShoppingSystemTest, PersonModule_BuyInsufficientStockBranch)
{
    Customer customer(*thread);

    queueRecv(*thread, {
        "cust1", "pass1",
        "1",     // home: buy
        "1",     // buy: add
        "1",     // item
        "9999",  // excessive quantity
        "3",     // buy: exit
        "4"      // home: logout
    });

    customer.login("customer.txt");

    bool notEnough = false;
    for (const auto& msg : thread->sentMessages())
    {
        if (msg == "NOT-ENOUGH")
            notEnough = true;
    }
    EXPECT_TRUE(notEnough);
}
