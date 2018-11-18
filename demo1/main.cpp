#include <iostream>
#include <chrono>

using namespace std;

void print_msg(const string& msg);

int main() {
    std::cout << "Hello, World!" << std::endl;

    print_msg("just from cpp");

    int count = 1;

//    const int MAX_LOOP = 10000 * 10000;
    const int MAX_LOOP = 10000 * 1;
    std::chrono::steady_clock::time_point beg = std::chrono::steady_clock::now();

    for (int i = 0; i < MAX_LOOP; ++i) {
        ++count;
    }

    std::chrono::steady_clock::time_point end= std::chrono::steady_clock::now();
    std::cout<<"after loop count is: "<<count<<std::endl;

    std::cout <<" Time difference = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - beg).count() <<std::endl;


    return 0;
}