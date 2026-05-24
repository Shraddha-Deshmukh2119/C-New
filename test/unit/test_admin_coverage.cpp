#include "test_fixture.h"
#include "coverage_helpers.h"
#include "person_accessor.h"

#include "Admin.h"
#include "Complaint_Base.h"

TEST_F(ShoppingSystemTest, AdminCoverage_AllHomeMenuCases)
{
    Admin admin(*thread);
    std::vector<std::string> q = {
        "admin1", "adminpass",
        "3",  // profile
        "5",  // view customers
        "6",  // view employees
        "8",  // accounts
        "15", // activity
        "17"  // exit
    };
    {
        std::ofstream activity("clients.txt");
        activity << "Client activity line\n";
    }
    queueRecvAll(*thread, q);
    admin.login("admin.txt");
    EXPECT_TRUE(sentContains(*thread, "Username: cust1"));
    EXPECT_TRUE(sentContains(*thread, "Final cash"));
}

TEST_F(ShoppingSystemTest, AdminCoverage_ViewMissingFile)
{
    std::filesystem::remove("customer.txt");
    Admin admin(*thread);
    queueRecv(*thread, {"admin1", "adminpass", "5", "17"});
    admin.login("admin.txt");
    EXPECT_FALSE(sentContains(*thread, "Username:"));
}

TEST_F(ShoppingSystemTest, AdminCoverage_AddEmployeeTrueAndFalse)
{
    Admin admin(*thread);
    queueRecv(*thread, {
        "admin1", "adminpass",
        "4",
        "E1", "25", "M", "01", "01", "2000",
        "42101-2222222-2", "e@x.com", "03002222222",
        "empnew", "passnew",
        "17"
    });
    admin.login("admin.txt");
    EXPECT_TRUE(sentEquals(*thread, "TRUE"));

    thread->clearSentMessages();
    PersonTestAccessor helper(*thread);
    queueRecv(*thread, {
        "E2", "25", "M", "01", "01", "2000",
        "42101-3333333-3", "e2@x.com", "03003333333",
        "empbad", "passbad"
    });
    EXPECT_FALSE(helper.testInput("Z:\\no_dir_oss\\emp.txt"));
}

TEST_F(ShoppingSystemTest, AdminCoverage_SearchAllKeysFoundAndNotFound)
{
    Admin admin(*thread);
    queueRecv(*thread, {
        "admin1", "adminpass",
        "10", "1", "12345-1234567-1", "6",
        "10", "2", "user@example.com", "6",
        "10", "3", "03001234567", "6",
        "10", "4", "cust1", "6",
        "10", "5", "pass1", "6",
        "10", "4", "nobody", "6",
        "17"
    });
    admin.login("admin.txt");
    EXPECT_GE(sentCount(*thread, "TRUE"), 5u);
    EXPECT_TRUE(sentEquals(*thread, "FALSE"));
}

TEST_F(ShoppingSystemTest, AdminCoverage_SearchEmployeeFile)
{
    Admin admin(*thread);
    queueRecv(*thread, {"admin1", "adminpass", "9", "4", "emp1", "6", "17"});
    admin.login("admin.txt");
    EXPECT_TRUE(sentEquals(*thread, "TRUE"));
}

TEST_F(ShoppingSystemTest, AdminCoverage_DeleteEmployeeRecord)
{
    writeEmployeeRecord("delme", "pass");
    Admin admin(*thread);
    queueRecv(*thread, {"admin1", "adminpass", "11", "delme", "17"});
    admin.login("admin.txt");

    std::ifstream file("emp.txt");
    const std::string contents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    EXPECT_EQ(contents.find("delme"), std::string::npos);
}

TEST_F(ShoppingSystemTest, AdminCoverage_BalanceUpdateFalseForUnknownUser)
{
    Admin admin(*thread);
    queueRecv(*thread, {"admin1", "adminpass", "13", "missing", "99", "17"});
    admin.login("admin.txt");
    EXPECT_TRUE(sentEquals(*thread, "FALSE"));
}

TEST_F(ShoppingSystemTest, AdminCoverage_BalanceUpdateEmployeeFile)
{
    Admin admin(*thread);
    queueRecv(*thread, {"admin1", "adminpass", "14", "emp1", "950", "17"});
    admin.login("admin.txt");
    EXPECT_TRUE(sentEquals(*thread, "TRUE"));
}

TEST_F(ShoppingSystemTest, AdminCoverage_StockInvalidProductAndInsufficientCash)
{
    writeCashFile(50, 0, 0, 50);
    Admin admin(*thread);
    queueRecv(*thread, {
        "admin1", "adminpass",
        "7", "1", "NotAProduct", "7", "1", "Product1", "99999", "17"
    });
    admin.login("admin.txt");
    EXPECT_GE(sentCount(*thread, "FALSE"), 2u);
}

TEST_F(ShoppingSystemTest, AdminCoverage_StockDefaultSwitchBranch)
{
    Admin admin(*thread);
    queueRecv(*thread, {"admin1", "adminpass", "7", "9", "17"});
    admin.login("admin.txt");
    EXPECT_TRUE(sentContains(*thread, "Name: Product1"));
}

TEST_F(ShoppingSystemTest, AdminCoverage_AccountsMissingCashFile)
{
    std::filesystem::remove("cash.txt");
    Admin admin(*thread);
    queueRecv(*thread, {"admin1", "adminpass", "8", "17"});
    admin.login("admin.txt");
    EXPECT_FALSE(sentContains(*thread, "Final cash"));
}

TEST_F(ShoppingSystemTest, AdminCoverage_StockInitFailure)
{
    std::filesystem::remove("goods.txt");
    Admin admin(*thread);
    queueRecv(*thread, {"admin1", "adminpass", "7", "17"});
    admin.login("admin.txt");
    EXPECT_FALSE(sentContains(*thread, "Name: Product"));
}

TEST_F(ShoppingSystemTest, AdminCoverage_EndServerCase16)
{
    Admin admin(*thread);
    queueRecv(*thread, {"admin1", "adminpass", "16"});
    admin.login("admin.txt");
    SUCCEED();
}

TEST_F(ShoppingSystemTest, AdminCoverage_AdminBuyMenu)
{
    Admin admin(*thread);
    queueRecv(*thread, {"admin1", "adminpass", "1", "2", "3", "17"});
    admin.login("admin.txt");
    EXPECT_TRUE(sentContains(*thread, "NO-ORDER"));
}

TEST_F(ShoppingSystemTest, AdminCoverage_EmployeeComplainMenu)
{
    Complaint_Base base("complaint.dat");
    ASSERT_TRUE(base.write("Help", "cust1"));

    Admin admin(*thread);
    queueRecv(*thread, {
        "admin1", "adminpass",
        "2", "1", "cust1", "7", "17"
    });
    admin.login("admin.txt");
    EXPECT_TRUE(sentEquals(*thread, "TRUE"));
}
