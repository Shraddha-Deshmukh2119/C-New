#include "coverage_helpers.h"
#include "test_fixture.h"

#include "Admin.h"
#include "Complaint_Base.h"

// One test per Admin::home switch case to maximize branch coverage.
class AdminHomeCaseTest : public ShoppingSystemTest
{
protected:
    void runAdminCase(const std::vector<std::string>& afterLogin)
    {
        std::vector<std::string> q = {"admin1", "adminpass"};
        q.insert(q.end(), afterLogin.begin(), afterLogin.end());
        q.push_back("17");
        queueRecvAll(*thread, q);
        Admin admin(*thread);
        admin.login("admin.txt");
    }
};

TEST_F(AdminHomeCaseTest, AdminHomeCase01_Buy)
{
    runAdminCase({"1", "2", "3"});
    EXPECT_TRUE(sentContains(*thread, "NO-ORDER"));
}

TEST_F(AdminHomeCaseTest, AdminHomeCase02_Complain)
{
    Complaint_Base base("complaint.dat");
    ASSERT_TRUE(base.write("x", "cust1"));
    runAdminCase({"2", "1", "cust1", "7"});
    EXPECT_TRUE(sentEquals(*thread, "TRUE"));
}

TEST_F(AdminHomeCaseTest, AdminHomeCase03_Profile)
{
    runAdminCase({"3"});
    EXPECT_TRUE(sentContains(*thread, "Balance:"));
}

TEST_F(AdminHomeCaseTest, AdminHomeCase04_AddEmployee)
{
    runAdminCase({
        "4", "E", "25", "M", "1", "1", "2000",
        "CNIC", "e@e.com", "0300", "newe", "pw"
    });
    EXPECT_TRUE(sentEquals(*thread, "TRUE"));
}

TEST_F(AdminHomeCaseTest, AdminHomeCase05_ViewCustomers)
{
    runAdminCase({"5"});
    EXPECT_TRUE(sentContains(*thread, "cust1"));
}

TEST_F(AdminHomeCaseTest, AdminHomeCase06_ViewEmployees)
{
    runAdminCase({"6"});
    EXPECT_TRUE(sentContains(*thread, "emp1"));
}

TEST_F(AdminHomeCaseTest, AdminHomeCase07_StockDefault)
{
    runAdminCase({"7", "0"});
    EXPECT_TRUE(sentContains(*thread, "Product1"));
}

TEST_F(AdminHomeCaseTest, AdminHomeCase08_Accounts)
{
    runAdminCase({"8"});
    EXPECT_TRUE(sentContains(*thread, "Final cash"));
}

TEST_F(AdminHomeCaseTest, AdminHomeCase09_SearchEmployee)
{
    runAdminCase({"9", "4", "emp1", "6"});
    EXPECT_TRUE(sentEquals(*thread, "TRUE"));
}

TEST_F(AdminHomeCaseTest, AdminHomeCase10_SearchCustomer)
{
    runAdminCase({"10", "4", "cust1", "6"});
    EXPECT_TRUE(sentEquals(*thread, "TRUE"));
}

TEST_F(AdminHomeCaseTest, AdminHomeCase11_DeleteEmployee)
{
    writeEmployeeRecord("gone", "gone");
    runAdminCase({"11", "gone"});
    std::ifstream f("emp.txt");
    const auto text = std::string((std::istreambuf_iterator<char>(f)), {});
    EXPECT_EQ(text.find("gone"), std::string::npos);
}

TEST_F(AdminHomeCaseTest, AdminHomeCase12_DeleteCustomer)
{
    std::ofstream extra("customer.txt", std::ios::app);
    extra << "Username: gonec\nPassword: p\nName: X\nAge: 1\nSex: M\n"
          << "Date of birth: 1/1/1\nCNIC: 1\nEmail: e\nPhone number: 1\nBalance: 1\n\n";
    runAdminCase({"12", "gonec"});
    std::ifstream f("customer.txt");
    const auto text = std::string((std::istreambuf_iterator<char>(f)), {});
    EXPECT_EQ(text.find("gonec"), std::string::npos);
}

TEST_F(AdminHomeCaseTest, AdminHomeCase13_BalanceCustomer)
{
    runAdminCase({"13", "cust1", "1200"});
    EXPECT_TRUE(sentEquals(*thread, "TRUE"));
}

TEST_F(AdminHomeCaseTest, AdminHomeCase14_BalanceEmployee)
{
    runAdminCase({"14", "emp1", "333"});
    EXPECT_TRUE(sentEquals(*thread, "TRUE"));
}

TEST_F(AdminHomeCaseTest, AdminHomeCase15_Activity)
{
    std::ofstream a("clients.txt");
    a << "Client activity log line\n";
    a.close();
    runAdminCase({"15"});
    EXPECT_FALSE(thread->sentMessages().empty());
}

TEST_F(AdminHomeCaseTest, AdminHomeCase16_EndServer)
{
    queueRecv(*thread, {"admin1", "adminpass", "16"});
    Admin admin(*thread);
    admin.login("admin.txt");
    SUCCEED();
}

TEST_F(AdminHomeCaseTest, AdminHomeCase17_Exit)
{
    runAdminCase({});
    SUCCEED();
}
