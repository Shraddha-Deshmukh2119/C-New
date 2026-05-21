#include "test_fixture.h"

#include "Complaint_C.h"

TEST_F(ShoppingSystemTest, ComplaintCustomer_WriteReviewAndUpdate)
{
    Complaint_C complaint("cust1", "complaint_c.dat");

    const char text[] = "Wrong item received";
    EXPECT_TRUE(complaint.write(text));

    std::ostringstream review;
    complaint.reView(review);
    EXPECT_NE(review.str().find("Wrong item received"), std::string::npos);

    bool status = false;
    EXPECT_TRUE(complaint.update("Updated complaint text", status));
    EXPECT_FALSE(status);
}

TEST_F(ShoppingSystemTest, ComplaintCustomer_ReviewShowsMessageWhenMissing)
{
    Complaint_C complaint("missing_user", "complaint_missing.dat");

    std::ostringstream review;
    complaint.reView(review);
    EXPECT_TRUE(review.str().empty());
}
