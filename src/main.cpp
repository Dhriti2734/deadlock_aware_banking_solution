#include "account.h"
#include <iostream>
using namespace std;
//without mutex
int main() 
{
    cout << "=== Banking System ===" << endl;
    Account acc1(101, 5000);
    Account acc2(102, 3000);
    Account acc3(103, 2000);
    cout << "Initial balances:" << endl;
    cout << "Account 101: " << acc1.get_balance() << endl;
    cout << "Account 102: " << acc2.get_balance() << endl;
    cout << "Account 103: " << acc3.get_balance() << endl;
    acc1.deposit(1000);
    acc2.withdraw(500);
    acc1.transfer_to(acc2, 1500);
    cout << "Final balances:" << endl;
    cout << "Account 101: " << acc1.get_balance() << endl;
    cout << "Account 102: " << acc2.get_balance() << endl;
    cout << "Account 103: " << acc3.get_balance() << endl;
    return 0;
}