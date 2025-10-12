#include "account.h"
#include <iostream>
//no mutex included
using namespace std;
Account::Account(int id, double initial_balance) : account_id(id), balance(initial_balance)
{}
bool Account::deposit(double amount)
{
    if(amount > 0)
    {
        balance = balance + amount;
        add_to_history("Deposit: +" + to_string(amount));
        cout << "Deposited " << amount << " to Account " << account_id << endl;
        return true;
    }
    return false;
}
bool Account::withdraw(double amount)
{
    if(amount > 0 && amount <= balance)
    {
        balance = balance - amount;
        add_to_history("Withdraw: -" + to_string(amount));
        cout << "Withdrawn " << amount << " from Account " << account_id << endl;
        return true;
    }
    cout << "Withdrawal failed: Insufficient balance in Account " << account_id << endl;
    return false;
}
bool Account::transfer_to(Account &target_account, double amount)
{
    if(amount > 0 && amount <= balance)
    {
        balance = balance - amount;
        target_account.balance += amount;
        
        add_to_history("Transfer to Acc" + to_string(target_account.get_account_id()) + ": -" + to_string(amount));
        target_account.add_to_history("Transfer from Acc" + to_string(account_id) + ": +" + to_string(amount));
        
        cout << "Transferred " << amount << " from Account " << account_id 
             << " to Account " << target_account.get_account_id() << endl;
        return true;
    }
    cout << "Transfer failed: Insufficient balance in Account " << account_id << endl;
    return false;
}
double Account::get_balance()
{
    return balance;
}

int Account::get_account_id()
{
    return account_id;
}

void Account::add_to_history(const string& transaction)
{
    transaction_history.push_back(transaction);
}

void Account::print_transaction_history()
{
    cout << "Transaction history for account " << account_id << ":" << endl;
    for(const auto& transaction : transaction_history)
    {
        cout << "  " << transaction << endl;
    }
}