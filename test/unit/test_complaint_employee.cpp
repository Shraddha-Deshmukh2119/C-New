#include "test_fixture.h"

#include "Complaint_E.h"

TEST_F(ShoppingSystemTest, ComplaintEmployee_AnswerViewAndClear)
{
    Complaint_E employeeComplaints("complaint_e.dat");
    Complaint_Base base("complaint_e.dat");

    const char client[] = "cust1";
    ASSERT_TRUE(base.write("Late delivery", client));

    EXPECT_TRUE(employeeComplaints.answer("We are sorry for the delay", client));

    Complaint readIn{};
    ASSERT_TRUE(base.see(readIn, client, false));
    EXPECT_STREQ(readIn.answer, "We are sorry for the delay");
    EXPECT_TRUE(readIn.answered);

    std::ostringstream summary;
    employeeComplaints.view(false, summary);
    EXPECT_NE(summary.str().find("cust1"), std::string::npos);

    std::ostringstream details;
    employeeComplaints.view(true, details);
    EXPECT_NE(details.str().find("Late delivery"), std::string::npos);

    employeeComplaints.clear();
    std::ifstream file("complaint_e.dat", std::ios::binary);
    EXPECT_TRUE(file.good());
    file.seekg(0, std::ios::end);
    EXPECT_EQ(file.tellg(), 0);
}
