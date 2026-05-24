#pragma once

#include "Customer.h"
#include "Goods.h"
#include "Cash.h"

// Person.h defines N(10) which breaks Google Test templates using parameter name N.
#undef N

// Exposes protected Person helpers for direct branch-coverage tests.
class PersonTestAccessor : public Customer
{
public:
    explicit PersonTestAccessor(Thread& server) : Customer(server) {}

    bool testInput(const std::string& fileName) { return input(fileName); }
    bool testTransferToFile(const std::string& fileName) { return transfer_to_file(fileName); }
    bool testInitializeGoods(Goods* goods) { return initialize_goods(goods); }
    bool testInitializeBalance(const std::string& fileName) { return initialize_balance(fileName); }
    bool testInitializeCash(Cash& cash) { return initialize_cash(cash); }
    bool testUpdateGoods(Goods* goods) { return update_goods(goods); }
    bool testUpdateBalance(const std::string& fileName) { return update_balance(fileName); }
    bool testUpdateCash(Cash& cash) { return update_cash(cash); }
    void testBuy(const std::string& fileName) { buy(fileName); }
    void testProfile(const std::string& fileName, const std::string& lookup) { profile(fileName, lookup); }
    void testLogin(const std::string& fileName) { login(fileName); }

    void setLookupValue(const std::string& value) { lookup = value; }
    void setUsernameValue(const std::string& value) { username = value; }
    void setBalanceValue(const std::string& value) { balance = value; }
};
