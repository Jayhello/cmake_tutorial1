#include <iostream>
#include <chrono>


int main() {
    std::cout << "Hello, World!" << std::endl;
    int count = 1;

    const int MAX_LOOP = 10000 * 10000;
    std::chrono::steady_clock::time_point beg = std::chrono::steady_clock::now();

    for (int i = 0; i < MAX_LOOP; ++i) {
        ++count;
    }

    std::chrono::steady_clock::time_point end= std::chrono::steady_clock::now();
    std::cout<<"after loop count is: "<<count<<std::endl;

    std::cout <<" Time difference = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - beg).count() <<std::endl;


    return 0;
}