#pragma once

#include <filesystem>
#include <fstream>
#include <memory>
#include <sstream>
#include <string>

#include <gtest/gtest.h>

#include "Server.h"
#include "Thread.h"

class ShoppingSystemTest : public ::testing::Test
{
protected:
    static void SetUpTestSuite();
    static void TearDownTestSuite();

    void SetUp() override;
    void TearDown() override;

    void writeGoodsFile();
    void writeCashFile(double initial = 10000.0, double cashIn = 0.0,
                       double cashOut = 0.0, double finalCash = 10000.0);
    void writeUserRecord(const std::string& fileName, const std::string& username,
                         const std::string& password, double balance = 500.0);
    void writeAdminRecord();
    void writeEmployeeRecord(const std::string& username = "emp1",
                             const std::string& password = "emppass");

    void queueRecv(Thread& thread, const std::initializer_list<std::string>& messages);
    std::string lastSent(const Thread& thread) const;

    std::unique_ptr<Server> server;
    std::unique_ptr<Thread> thread;

    static std::filesystem::path runtimeDir;
    std::filesystem::path previousCwd;
};
