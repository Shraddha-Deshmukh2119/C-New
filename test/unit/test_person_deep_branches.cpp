#include "coverage_helpers.h"
#include "person_accessor.h"
#include "test_fixture.h"

#include "Customer.h"

TEST_F(ShoppingSystemTest, PersonDeep_BuyMultipleItemsThenConfirm)
{
    writeUserRecord("customer.txt", "cust1", "pass1", 10000.0);
    Customer customer(*thread);
    queueRecv(*thread, {
        "cust1", "pass1",
        "1",
        "1", "1", "1",
        "1", "2", "1",
        "2",
        "3", "4"
    });
    customer.login("customer.txt");
    EXPECT_TRUE(sentContains(*thread, "Your new balance"));
}

TEST_F(ShoppingSystemTest, PersonDeep_ProfileLookupMissThenHit)
{
    PersonTestAccessor person(*thread);
    person.testProfile("customer.txt", "Username: nobody");
    EXPECT_TRUE(thread->sentMessages().empty() || !sentContains(*thread, "Balance:"));

    thread->clearSentMessages();
    person.testProfile("customer.txt", "Username: cust1");
    EXPECT_TRUE(sentContains(*thread, "Balance:"));
}

TEST_F(ShoppingSystemTest, PersonDeep_BuyAddThenCancelWithoutConfirm)
{
    Customer customer(*thread);
    queueRecv(*thread, {"cust1", "pass1", "1", "1", "1", "1", "3", "4"});
    customer.login("customer.txt");
    EXPECT_TRUE(sentEquals(*thread, "ENOUGH"));
}
