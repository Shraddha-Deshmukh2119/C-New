#include "coverage_helpers.h"
#include "test_fixture.h"

#include "Complaint_Base.h"
#include "Employee.h"

TEST_F(ShoppingSystemTest, EmployeeCoverage_HomeDefaultInvalidChoice)
{
    Employee employee(*thread);
    queueRecv(*thread, {"emp1", "emppass", "99", "4"});
    employee.login("emp.txt");
    SUCCEED();
}

TEST_F(ShoppingSystemTest, EmployeeCoverage_ComplainSeeByNameTrueAndFalse)
{
    Complaint_Base base("complaint.dat");
    ASSERT_TRUE(base.write("Broken screen", "cust1"));

    Employee employee(*thread);
    queueRecv(*thread, {
        "emp1", "emppass",
        "2",
        "1", "cust1",
        "2", "cust1",
        "1", "unknown_client",
        "7", "4"
    });
    employee.login("emp.txt");
    EXPECT_GE(sentCount(*thread, "TRUE"), 2u);
    EXPECT_TRUE(sentEquals(*thread, "FALSE"));
}

TEST_F(ShoppingSystemTest, EmployeeCoverage_ComplainAnswerTrueAndFalse)
{
    Complaint_Base base("complaint.dat");
    ASSERT_TRUE(base.write("Late ship", "cust1"));

    Employee employee(*thread);
    queueRecv(*thread, {
        "emp1", "emppass",
        "2",
        "3", "cust1", "Approved refund",
        "3", "ghost", "n/a",
        "7", "4"
    });
    employee.login("emp.txt");
    EXPECT_TRUE(sentEquals(*thread, "TRUE"));
    EXPECT_TRUE(sentEquals(*thread, "FALSE"));
}

TEST_F(ShoppingSystemTest, EmployeeCoverage_ComplainViewSummaryAndDetails)
{
    Complaint_Base base("complaint.dat");
    ASSERT_TRUE(base.write("Issue A", "custA"));

    Employee employee(*thread);
    queueRecv(*thread, {"emp1", "emppass", "2", "4", "5", "6", "7", "4"});
    employee.login("emp.txt");
    EXPECT_TRUE(sentContains(*thread, "custA"));
    EXPECT_TRUE(sentContains(*thread, "Issue A"));
}

TEST_F(ShoppingSystemTest, EmployeeCoverage_ComplainDefaultInvalidInput)
{
    Employee employee(*thread);
    queueRecv(*thread, {"emp1", "emppass", "2", "99", "7", "4"});
    employee.login("emp.txt");
    SUCCEED();
}

TEST_F(ShoppingSystemTest, EmployeeCoverage_BuyConfirmWithItemsEmployeeDiscount)
{
    writeUserRecord("emp.txt", "emp1", "emppass", 5000.0);
    Employee employee(*thread);
    queueRecv(*thread, {
        "emp1", "emppass",
        "1", "1", "1", "2", "2", "3", "4"
    });
    employee.login("emp.txt");
    EXPECT_TRUE(sentContains(*thread, "Your new balance"));
}

TEST_F(ShoppingSystemTest, EmployeeCoverage_ProfileMenu)
{
    Employee employee(*thread);
    queueRecv(*thread, {"emp1", "emppass", "3", "4"});
    employee.login("emp.txt");
    EXPECT_TRUE(sentContains(*thread, "Balance:"));
}
