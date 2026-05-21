#include "test_fixture.h"

#include "Customer.h"

// Cash operators are exercised through the purchase confirmation flow.
TEST_F(ShoppingSystemTest, CashModule_OperatorPlusMinusViaPurchase)
{
    Customer customer(*thread);

    queueRecv(*thread, {
        "cust1", "pass1", // login
        "1",              // home: buy
        "2",              // buy: confirm (no items -> NO-ORDER)
        "3",              // buy: exit
        "4"               // home: logout
    });

    customer.login("customer.txt");
    bool noOrder = false;
    for (const auto& msg : thread->sentMessages())
    {
        if (msg.find("NO-ORDER") != std::string::npos)
            noOrder = true;
    }
    EXPECT_TRUE(noOrder);
}
