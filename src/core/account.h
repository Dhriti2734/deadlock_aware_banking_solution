#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <vector>
#include <string>
using namespace std;

class Account
{
private:
    int account_id;
    double balance;
    vector<string> transaction_history;

public:
    Account(int id, double initial_balance);
    bool deposit(double amount);
    bool withdraw(double amount);
    bool transfer_to(Account &target_account, double amount);
    double get_balance();
    int get_account_id();
    void add_to_history(const string &transaction);
    void print_transaction_history();
};

#endif