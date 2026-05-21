#include "test_fixture.h"

#include "Complaint_Base.h"
#include "Employee.h"

TEST_F(ShoppingSystemTest, EmployeeModule_LoginBuyWithDiscountAndProfile)
{
    Employee employee(*thread);

    queueRecv(*thread, {
        "emp1", "emppass",
        "1",    // buy (employee discount branch)
        "2",    // confirm with no order
        "3",    // exit buy
        "3",    // profile
        "4"     // exit home
    });

    employee.login("emp.txt");

    bool noOrder = false;
    for (const auto& msg : thread->sentMessages())
    {
        if (msg.find("NO-ORDER") != std::string::npos)
            noOrder = true;
    }
    EXPECT_TRUE(noOrder);
}

TEST_F(ShoppingSystemTest, EmployeeModule_ComplaintAnswerFlow)
{
    Complaint_Base base("complaint.dat");
    const char client[] = "cust1";
    ASSERT_TRUE(base.write("Need refund", client));

    Employee employee(*thread);

    queueRecv(*thread, {
        "emp1", "emppass",
        "2",               // home: complain
        "3",               // complain: answer
        "cust1",
        "Refund approved",
        "7",               // complain: exit
        "4"                // home: logout
    });

    employee.login("emp.txt");

    bool answered = false;
    for (const auto& msg : thread->sentMessages())
    {
        if (msg == "TRUE")
            answered = true;
    }
    EXPECT_TRUE(answered);
}
