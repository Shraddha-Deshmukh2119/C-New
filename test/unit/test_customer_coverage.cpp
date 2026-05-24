#include "coverage_helpers.h"
#include "test_fixture.h"

#include "Complaint_Base.h"
#include "Complaint_E.h"
#include "Customer.h"

TEST_F(ShoppingSystemTest, CustomerCoverage_ComplainWriteTrueAndDuplicateFalse)
{
    Customer customer(*thread);
    std::vector<std::string> q = {"1"};
    appendSignupFields(q, "cmpuser", "cmppass");
    q.insert(q.end(), {
        "2", "cmpuser", "cmppass",
        "2", "1", "First complaint", "1", "Second complaint",
        "4", "4", "3"
    });
    queueRecvAll(*thread, q);
    customer.start();
    EXPECT_TRUE(sentEquals(*thread, "TRUE"));
    EXPECT_TRUE(sentEquals(*thread, "FALSE"));
}

TEST_F(ShoppingSystemTest, CustomerCoverage_ComplainReviewAndUpdateBranches)
{
    Customer customer(*thread);
    std::vector<std::string> q = {"1"};
    appendSignupFields(q, "revuser", "revpass");
    q.insert(q.end(), {
        "2", "revuser", "revpass",
        "2", "1", "Need help",
        "2",
        "3", "Updated text",
        "4", "4", "3"
    });
    queueRecvAll(*thread, q);
    customer.start();
    EXPECT_TRUE(sentContains(*thread, "Need help"));
    EXPECT_TRUE(sentEquals(*thread, "TRUE"));

    Complaint_E emp("complaint.dat");
    ASSERT_TRUE(emp.answer("Done", "revuser"));

    Customer customer2(*thread);
    queueRecv(*thread, {"2", "revuser", "revpass", "2", "3", "Too late", "4", "4", "3"});
    customer2.start();
    EXPECT_TRUE(sentEquals(*thread, "FAILED"));
}

TEST_F(ShoppingSystemTest, CustomerCoverage_ComplainUpdateFalseWhenMissing)
{
    Customer customer(*thread);
    queueRecv(*thread, {"2", "cust1", "pass1", "2", "3", "No complaint yet", "4", "4", "3"});
    customer.start();
    EXPECT_TRUE(sentEquals(*thread, "FALSE"));
}

TEST_F(ShoppingSystemTest, CustomerCoverage_AllHomeMenuPaths)
{
    Customer customer(*thread);
    queueRecv(*thread, {"2", "cust1", "pass1", "3", "1", "2", "3", "4", "3"});
    customer.start();
    EXPECT_TRUE(sentContains(*thread, "NO-ORDER"));
    EXPECT_TRUE(sentContains(*thread, "Balance:"));
}

TEST_F(ShoppingSystemTest, CustomerCoverage_SignupThenExitStartMenu)
{
    Customer customer(*thread);
    std::vector<std::string> q = {"1"};
    appendSignupFields(q, "onlysignup", "passonly");
    q.push_back("3");
    queueRecvAll(*thread, q);
    customer.start();

    std::ifstream file("customer.txt");
    const std::string contents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    EXPECT_NE(contents.find("onlysignup"), std::string::npos);
}
