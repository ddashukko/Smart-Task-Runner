//variant 12
#include <iostream>
#include <string>
#include <future>
#include <chrono>
#include <thread>
#include <syncstream>
#include <vector>

using namespace std;

void compute(const string& name, int sec)
{
    this_thread::sleep_for(chrono::seconds(sec));
    osyncstream(cout) << "[" << this_thread::get_id() << "] "
        << name << " completed (" << sec << " sec)\n";
}

void work()
{
    using namespace chrono;

    cout << "Main thread ID: " << this_thread::get_id() << "\n";
    cout << "Starting work...\n";

    auto start = steady_clock::now();

    auto future_branch1 = async(launch::async, []()
        {
            compute("A1", 7);

            auto f_c1 = async(launch::async, []() { compute("C1", 1); });
            compute("B1", 7);
             
            f_c1.get();
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

    cout << "Total Time: " << total << " seconds\n";
    cout << "Work is done!\n";
}

int main()
{
    work();
    return 0;
}