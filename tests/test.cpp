#include "../src/deadlock/bankers_algo.h"
#include <iostream>
using namespace std;
void test_bankers_algorithm() 
{
    BankersAlgorithm banker(5, 3);
    banker.print_system_state();
    cout << "\n Initial Safe State Check" << endl;
    banker.is_safe_state();
    cout << "\n Safe Resource Request" << endl;
    vector<int> request1 = {1, 0, 2};
    bool granted1 = banker.request_resources(1, request1);
    cout << "\n Another Safe Request" << endl;
    vector<int> request2 = {0, 2, 0};
    bool granted2 = banker.request_resources(0, request2);
    cout << "\n Large Request (Potentially Unsafe)" << endl;
    vector<int> request3 = {3, 3, 0};
    bool granted3 = banker.request_resources(4, request3);
    cout << "\n Final System State " << endl;
    banker.print_system_state();
    cout<<"\nCompleted"<<endl;
}
int main() 
{
    test_bankers_algorithm();
    return 0;
}