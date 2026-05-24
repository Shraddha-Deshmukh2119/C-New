#include "coverage_helpers.h"
#include "test_fixture.h"

#include "Complaint_Base.h"
#include "Complaint_C.h"
#include "Complaint_E.h"

TEST_F(ShoppingSystemTest, ComplaintCoverage_WriteDuplicateReturnsFalse)
{
    Complaint_Base base("dup.dat");
    const char client[] = "userA";
    EXPECT_TRUE(base.write("First", client));
    EXPECT_FALSE(base.write("Second", client));
}

TEST_F(ShoppingSystemTest, ComplaintCoverage_SeeNotFoundReturnsFalse)
{
    Complaint_Base base("see.dat");
    Complaint found{};
    EXPECT_FALSE(base.see(found, "missing", true));
}

TEST_F(ShoppingSystemTest, ComplaintCoverage_UpdateNotFoundReturnsFalse)
{
    Complaint_Base base("upd.dat");
    bool status = true;
    EXPECT_FALSE(base.update("text", "ghost", status));
    EXPECT_FALSE(status);
}

TEST_F(ShoppingSystemTest, ComplaintCoverage_SeeWithoutMarkingSeen)
{
    Complaint_Base base("seen.dat");
    const char client[] = "c1";
    ASSERT_TRUE(base.write("Msg", client));

    Complaint readIn{};
    EXPECT_TRUE(base.see(readIn, client, false));
    EXPECT_FALSE(readIn.seen);
}

TEST_F(ShoppingSystemTest, ComplaintCoverage_EmployeeAnswerNotFound)
{
    Complaint_E emp("ans.dat");
    EXPECT_FALSE(emp.answer("No one", "missing"));
}

TEST_F(ShoppingSystemTest, ComplaintCoverage_EmployeeViewEmptyFile)
{
    Complaint_E emp("empty.dat");
    std::ostringstream os;
    emp.view(true, os);
    EXPECT_TRUE(os.str().empty());
}

TEST_F(ShoppingSystemTest, ComplaintCoverage_CustomerReviewWhenRegistered)
{
    Complaint_C customerSide("custX", "custx.dat");
    ASSERT_TRUE(customerSide.write("My problem"));

    std::ostringstream review;
    customerSide.reView(review);
    EXPECT_NE(review.str().find("My problem"), std::string::npos);
}

TEST_F(ShoppingSystemTest, ComplaintCoverage_WriteCreatesFileWhenMissing)
{
    std::filesystem::remove("brand_new.dat");
    Complaint_Base base("brand_new.dat");
    EXPECT_TRUE(base.write("Hello", "client1"));
    EXPECT_TRUE(std::filesystem::exists("brand_new.dat"));
}
