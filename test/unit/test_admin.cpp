#include "test_fixture.h"

#include "Admin.h"

TEST_F(ShoppingSystemTest, AdminModule_ViewCustomerRecords)
{
    Admin admin(*thread);

    queueRecv(*thread, {
        "admin1", "adminpass",
        "5",  // view customers
        "17"  // exit (no endServer)
    });

    admin.login("admin.txt");

    bool sentRecords = false;
    for (const auto& msg : thread->sentMessages())
    {
        if (msg.find("Username: cust1") != std::string::npos)
            sentRecords = true;
    }
    EXPECT_TRUE(sentRecords);
}

TEST_F(ShoppingSystemTest, AdminModule_SearchByUsername)
{
    Admin admin(*thread);

    queueRecv(*thread, {
        "admin1", "adminpass",
        "10",        // search customers
        "4",         // search by username
        "cust1",
        "6",         // exit search menu
        "17"
    });

    admin.login("admin.txt");

    bool searchHit = false;
    for (const auto& msg : thread->sentMessages())
    {
        if (msg == "TRUE")
            searchHit = true;
    }
    EXPECT_TRUE(searchHit);
}

TEST_F(ShoppingSystemTest, AdminModule_DeleteCustomerRecord)
{
    {
        std::ofstream extra("customer.txt", std::ios::app);
        extra << "Username: todelete\n"
              << "Password: removeme\n"
              << "Name: Delete Me\n"
              << "Age: 20\n"
              << "Sex: M\n"
              << "Date of birth: 01/01/2001\n"
              << "CNIC: 11111-1111111-1\n"
              << "Email: del@example.com\n"
              << "Phone number: 03000000000\n"
              << "Balance: 10\n"
              << "\n";
    }

    Admin admin(*thread);

    queueRecv(*thread, {
        "admin1", "adminpass",
        "12",       // delete customer
        "todelete",
        "17"
    });

    admin.login("admin.txt");

    std::ifstream file("customer.txt");
    std::string contents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    EXPECT_EQ(contents.find("todelete"), std::string::npos);
    EXPECT_NE(contents.find("cust1"), std::string::npos);
}

TEST_F(ShoppingSystemTest, AdminModule_UpdateBalance)
{
    Admin admin(*thread);

    queueRecv(*thread, {
        "admin1", "adminpass",
        "13",      // update customer balance
        "cust1",
        "1500",
        "17"
    });

    admin.login("admin.txt");
    EXPECT_EQ(lastSent(*thread), "TRUE");

    std::ifstream file("customer.txt");
    std::string line;
    bool updated = false;
    while (std::getline(file, line))
    {
        if (line == "Balance: 1500")
            updated = true;
    }
    EXPECT_TRUE(updated);
}

TEST_F(ShoppingSystemTest, AdminModule_StockManualReorderBranch)
{
    Admin admin(*thread);

    queueRecv(*thread, {
        "admin1", "adminpass",
        "7",           // stock menu
        "1",           // manual reorder
        "Product1",
        "5",           // quantity
        "17"           // exit admin home
    });

    admin.login("admin.txt");

    bool stockListed = false;
    bool reorderTrue = false;
    for (const auto& msg : thread->sentMessages())
    {
        if (msg.find("Name: Product1") != std::string::npos)
            stockListed = true;
        if (msg == "TRUE")
            reorderTrue = true;
    }
    EXPECT_TRUE(stockListed);
    EXPECT_TRUE(reorderTrue);
}

TEST_F(ShoppingSystemTest, AdminModule_StockPriceUpdateBranch)
{
    Admin admin(*thread);

    queueRecv(*thread, {
        "admin1", "adminpass",
        "7",
        "2",           // update price/reorder settings
        "Product2",
        "25.5",
        "4",
        "8",
        "17"
    });

    admin.login("admin.txt");

    bool priceUpdateOk = false;
    for (const auto& msg : thread->sentMessages())
    {
        if (msg == "TRUE")
            priceUpdateOk = true;
    }
    EXPECT_TRUE(priceUpdateOk);
}

TEST_F(ShoppingSystemTest, AdminModule_AccountsAndAddEmployee)
{
    Admin admin(*thread);

    queueRecv(*thread, {
        "admin1", "adminpass",
        "8",  // accounts
        "4",  // add employee
        "Emp Two", "28", "F", "02", "02", "1992",
        "42101-9999999-9", "emp2@mail.com", "03001112222",
        "emp2", "pass2",
        "17"
    });

    admin.login("admin.txt");

    std::ifstream empFile("emp.txt");
    std::string empContents((std::istreambuf_iterator<char>(empFile)), std::istreambuf_iterator<char>());
    EXPECT_NE(empContents.find("Username: emp2"), std::string::npos);

    bool cashSent = false;
    for (const auto& msg : thread->sentMessages())
    {
        if (msg.find("Final cash") != std::string::npos)
            cashSent = true;
    }
    EXPECT_TRUE(cashSent);
}
