#include "test_fixture.h"

#include "Complaint_Base.h"
#include "Complaint_E.h"

TEST_F(ShoppingSystemTest, ComplaintBase_WriteAndViewRecord)
{
    const std::string file = "complaint_base.dat";
    Complaint_Base base(file);

    const char text[] = "Delivery was late";
    const char client[] = "cust1";

    EXPECT_TRUE(base.write(text, client));

    Complaint readIn{};
    EXPECT_TRUE(base.see(readIn, client, true));
    EXPECT_STREQ(readIn.text, text);
    EXPECT_STREQ(readIn.client, client);
    EXPECT_TRUE(readIn.seen);

    std::ostringstream output;
    base.view(readIn, output);
    EXPECT_NE(output.str().find("Delivery was late"), std::string::npos);
}

TEST_F(ShoppingSystemTest, ComplaintBase_UpdateRejectedWhenAlreadyAnswered)
{
    const std::string file = "complaint_answered.dat";
    Complaint_Base base(file);
    Complaint_E employeeSide(file);

    const char client[] = "cust1";
    ASSERT_TRUE(base.write("Initial issue", client));
    ASSERT_TRUE(employeeSide.answer("Resolved", client));

    bool status = false;
    EXPECT_FALSE(base.update("Updated issue", client, status));
    EXPECT_TRUE(status);
}

TEST_F(ShoppingSystemTest, ComplaintBase_UpdateTextWhenNotAnswered)
{
    const std::string file = "complaint_update.dat";
    Complaint_Base base(file);

    const char client[] = "cust1";
    ASSERT_TRUE(base.write("Old text", client));

    bool status = false;
    EXPECT_TRUE(base.update("New text", client, status));
    EXPECT_FALSE(status);

    Complaint readIn{};
    ASSERT_TRUE(base.see(readIn, client, false));
    EXPECT_STREQ(readIn.text, "New text");
}
