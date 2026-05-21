#include "test_fixture.h"

#include <chrono>
#include <iomanip>
#include <sstream>

std::filesystem::path ShoppingSystemTest::runtimeDir;

int test_getch()
{
    return 0;
}

void ShoppingSystemTest::SetUpTestSuite()
{
    runtimeDir = std::filesystem::temp_directory_path() / "online_shopping_tests";
    std::filesystem::create_directories(runtimeDir);
}

void ShoppingSystemTest::TearDownTestSuite()
{
    std::error_code ec;
    std::filesystem::remove_all(runtimeDir, ec);
}

void ShoppingSystemTest::SetUp()
{
    previousCwd = std::filesystem::current_path();

    const auto stamp = std::chrono::steady_clock::now().time_since_epoch().count();
    const auto caseDir = runtimeDir / (std::string("case_") + ::testing::UnitTest::GetInstance()->current_test_info()->name() + "_" + std::to_string(stamp));
    std::filesystem::create_directories(caseDir);
    std::filesystem::current_path(caseDir);

    writeGoodsFile();
    writeCashFile();
    writeUserRecord("customer.txt", "cust1", "pass1", 1000.0);
    writeAdminRecord();
    writeEmployeeRecord();

    auto handler = [](Thread&) {};
    server = std::make_unique<Server>(handler, 17999);
    thread = std::make_unique<Thread>(server.get(), 0);
    thread->enableTestMode();
}

void ShoppingSystemTest::TearDown()
{
    thread.reset();
    server.reset();
    std::filesystem::current_path(previousCwd);
}

void ShoppingSystemTest::writeGoodsFile()
{
    std::ofstream file("goods.txt");
    for (int i = 1; i <= 10; ++i)
    {
        file << "Name: Product" << i << "\n"
             << "Stock: " << (50 + i) << "\n"
             << "Cost: " << (10.0 + i) << "\n"
             << "Price: " << (20.0 + i) << "\n"
             << "Reorder Level: 5\n"
             << "Reorder Amount: 10\n"
             << "\n";
    }
}

void ShoppingSystemTest::writeCashFile(double initial, double cashIn, double cashOut, double finalCash)
{
    std::ofstream file("cash.txt");
    file << "Initial cash: " << initial << "\n"
         << "Cash in: " << cashIn << "\n"
         << "Cash out: " << cashOut << "\n"
         << "Final cash: " << finalCash << "\n";
}

void ShoppingSystemTest::writeUserRecord(const std::string& fileName, const std::string& username,
                                         const std::string& password, double balance)
{
    std::ofstream file(fileName, std::ios::trunc);
    file << "Username: " << username << "\n"
         << "Password: " << password << "\n"
         << "Name: Test User\n"
         << "Age: 25\n"
         << "Sex: M\n"
         << "Date of birth: 01/01/2000\n"
         << "CNIC: 12345-1234567-1\n"
         << "Email: user@example.com\n"
         << "Phone number: 03001234567\n"
         << "Balance: " << balance << "\n"
         << "\n";
}

void ShoppingSystemTest::writeAdminRecord()
{
    writeUserRecord("admin.txt", "admin1", "adminpass", 2000.0);
}

void ShoppingSystemTest::writeEmployeeRecord(const std::string& username, const std::string& password)
{
    writeUserRecord("emp.txt", username, password, 800.0);
}

void ShoppingSystemTest::queueRecv(Thread& t, const std::initializer_list<std::string>& messages)
{
    for (const auto& message : messages)
        t.pushRecv(message);
}

std::string ShoppingSystemTest::lastSent(const Thread& t) const
{
    const auto& sent = t.sentMessages();
    if (sent.empty())
        return "";
    return sent.back();
}
