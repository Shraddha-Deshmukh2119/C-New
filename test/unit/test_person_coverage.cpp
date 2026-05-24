#include <array>

#include "test_fixture.h"
#include "coverage_helpers.h"
#include "person_accessor.h"

#include "Admin.h"
#include "Employee.h"

namespace
{
constexpr std::size_t kGoodsCount = 10;
}

// --- Person::login branches ---
TEST_F(ShoppingSystemTest, PersonCoverage_LoginIncorrectUserThenCorrect)
{
    PersonTestAccessor person(*thread);
    queueRecv(*thread, {"ghost", "x", "cust1", "pass1", "4"});
    person.testLogin("customer.txt");
    EXPECT_TRUE(sentEquals(*thread, "Incorrect-User"));
    EXPECT_TRUE(sentEquals(*thread, "Correct"));
}

TEST_F(ShoppingSystemTest, PersonCoverage_LoginIncorrectPasswordThenCorrect)
{
    PersonTestAccessor person(*thread);
    queueRecv(*thread, {"cust1", "wrong", "cust1", "pass1", "4"});
    person.testLogin("customer.txt");
    EXPECT_TRUE(sentEquals(*thread, "Incorrect-Pass"));
    EXPECT_TRUE(sentEquals(*thread, "Correct"));
}

TEST_F(ShoppingSystemTest, PersonCoverage_LoginMissingFileReturnsEarly)
{
    std::filesystem::remove("customer.txt");
    PersonTestAccessor person(*thread);
    queueRecv(*thread, {"cust1", "pass1"});
    person.testLogin("customer.txt");
    EXPECT_FALSE(sentEquals(*thread, "Correct"));
}

// --- Person::input / transfer_to_file ---
TEST_F(ShoppingSystemTest, PersonCoverage_InputSuccessSendsTrue)
{
    PersonTestAccessor person(*thread);
    queueRecv(*thread, {
        "Name", "22", "M", "01", "02", "2001",
        "42101-1111111-1", "a@b.com", "03001111111",
        "inuser", "inpass"
    });
    EXPECT_TRUE(person.testInput("customer.txt"));
    EXPECT_TRUE(sentEquals(*thread, "true"));
}

TEST_F(ShoppingSystemTest, PersonCoverage_InputFailureSendsFalse)
{
    PersonTestAccessor person(*thread);
    queueRecv(*thread, {
        "Name", "22", "M", "01", "02", "2001",
        "42101-1111111-1", "a@b.com", "03001111111",
        "bad", "bad"
    });
    const std::string invalidPath = "Z:\\no_such_dir_oss\\customer.txt";
    EXPECT_FALSE(person.testInput(invalidPath));
    EXPECT_TRUE(sentEquals(*thread, "false"));
}

// --- initialize / update helpers ---
TEST_F(ShoppingSystemTest, PersonCoverage_InitializeGoodsMissingFile)
{
    std::filesystem::remove("goods.txt");
    PersonTestAccessor person(*thread);
    std::array<Goods, kGoodsCount> goods{};
    EXPECT_FALSE(person.testInitializeGoods(goods.data()));
}

TEST_F(ShoppingSystemTest, PersonCoverage_InitializeBalanceMissingFile)
{
    PersonTestAccessor person(*thread);
    person.setLookupValue("Username: cust1");
    std::filesystem::remove("customer.txt");
    EXPECT_FALSE(person.testInitializeBalance("customer.txt"));
}

TEST_F(ShoppingSystemTest, PersonCoverage_InitializeCashMissingFile)
{
    std::filesystem::remove("cash.txt");
    PersonTestAccessor person(*thread);
    Cash cash{};
    EXPECT_FALSE(person.testInitializeCash(cash));
}

TEST_F(ShoppingSystemTest, PersonCoverage_UpdateHelpersRoundTrip)
{
    PersonTestAccessor person(*thread);
    person.setLookupValue("Username: cust1");
    person.setBalanceValue("777");

    std::array<Goods, kGoodsCount> goods{};
    ASSERT_TRUE(person.testInitializeGoods(goods.data()));
    Cash cash{};
    ASSERT_TRUE(person.testInitializeCash(cash));
    cash + 50.0;
    EXPECT_TRUE(person.testUpdateGoods(goods.data()));
    EXPECT_TRUE(person.testUpdateBalance("customer.txt"));
    EXPECT_TRUE(person.testUpdateCash(cash));
}

// --- Person::profile ---
TEST_F(ShoppingSystemTest, PersonCoverage_ProfileMissingFileCallsEndServer)
{
    PersonTestAccessor person(*thread);
    std::filesystem::remove("customer.txt");
    person.testProfile("customer.txt", "Username: cust1");
    EXPECT_FALSE(sentContains(*thread, "Balance:"));
}

TEST_F(ShoppingSystemTest, PersonCoverage_ProfileSendsRecord)
{
    PersonTestAccessor person(*thread);
    person.testProfile("customer.txt", "Username: cust1");
    EXPECT_TRUE(sentContains(*thread, "Balance:"));
}

// --- Person::buy branches ---
TEST_F(ShoppingSystemTest, PersonCoverage_BuyInitFailureWhenGoodsMissing)
{
    std::filesystem::remove("goods.txt");
    PersonTestAccessor person(*thread);
    person.setLookupValue("Username: cust1");
    queueRecv(*thread, {"3"});
    person.testBuy("customer.txt");
    EXPECT_FALSE(sentContains(*thread, "Price($)"));
}

TEST_F(ShoppingSystemTest, PersonCoverage_BuyEnoughStockPath)
{
    PersonTestAccessor person(*thread);
    person.setLookupValue("Username: cust1");
    queueRecv(*thread, {"1", "1", "2", "3"});
    person.testBuy("customer.txt");
    EXPECT_TRUE(sentEquals(*thread, "ENOUGH"));
}

TEST_F(ShoppingSystemTest, PersonCoverage_BuyInsufficientBalanceBranch)
{
    writeUserRecord("customer.txt", "cust1", "pass1", 5.0);
    PersonTestAccessor person(*thread);
    person.setLookupValue("Username: cust1");
    queueRecv(*thread, {"1", "1", "1", "2", "3"});
    person.testBuy("customer.txt");
    EXPECT_TRUE(sentContains(*thread, "Cost of the order"));
    EXPECT_TRUE(sentContains(*thread, "Your balance"));
    EXPECT_FALSE(sentContains(*thread, "Your new balance"));
}

TEST_F(ShoppingSystemTest, PersonCoverage_BuySuccessfulOrderAndReorder)
{
    writeUserRecord("customer.txt", "cust1", "pass1", 5000.0);
    writeGoodsFileLowStockProduct1(6);
    writeCashFile(50000, 0, 0, 50000);

    PersonTestAccessor person(*thread);
    person.setLookupValue("Username: cust1");
    queueRecv(*thread, {"1", "1", "2", "2", "3"});
    person.testBuy("customer.txt");
    EXPECT_TRUE(sentContains(*thread, "Your new balance"));
}

TEST_F(ShoppingSystemTest, PersonCoverage_BuyEmployeeDiscountBranch)
{
    Employee employee(*thread);
    queueRecv(*thread, {
        "emp1", "emppass",
        "1", "1", "1", "1",
        "2", "3", "4"
    });
    employee.login("emp.txt");
    EXPECT_TRUE(sentContains(*thread, "Your new balance"));
}

TEST_F(ShoppingSystemTest, PersonCoverage_BuyAdminDiscountBranch)
{
    Admin admin(*thread);
    queueRecv(*thread, {
        "admin1", "adminpass",
        "1", "1", "1", "1",
        "2", "3", "17"
    });
    admin.login("admin.txt");
    EXPECT_TRUE(sentContains(*thread, "Your new balance"));
}
