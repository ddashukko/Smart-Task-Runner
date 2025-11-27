#include <iostream>
#include <string>
#include <future>
#include <chrono>
#include <thread>
#include <syncstream>

using namespace std;

void compute(const string& name, int sec)
{
    this_thread::sleep_for(chrono::seconds(sec));
    osyncstream(cout) << name << "\n";            
}

void work()
{
    using namespace chrono;

    auto start = steady_clock::now();

    auto future_branch1 = async(launch::async, []()
        {
            compute("A1", 7);      
            compute("B1", 7);      
            compute("C1", 1);      
        });

    auto future_branch2 = async(launch::async, []()
        {
            compute("A2", 1);      
            compute("B2", 1);      
            compute("C2", 1);      
        });

    future_branch1.get();
    future_branch2.get();

    compute("D", 1);

    auto end = steady_clock::now();
    auto total = duration_cast<seconds>(end - start).count();

    cout << "Time: " << total << " seconds\n";
    cout << "Work is done!\n";
}

int main()
{
    work();
    return 0;
}
