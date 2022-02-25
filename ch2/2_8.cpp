/**
* @description: 2_8.cpp
* @brief 生成多个线程，并等待它们完成运行
* @date: 2022/02/25 星期五 16:39:38
*/

#include <iostream>
#include <thread>
#include <vector>

int count = 0;

void do_work(unsigned id) {
    count++;
    std::cout << count << std::endl;
}

void f() {
    std::vector<std::thread> threads;
    for(unsigned i = 0; i < 20; ++i) {
        threads.emplace_back(do_work, i);
    }

    for(auto& entry : threads)
        entry.join();
}

int main() {

    f();

    return 0;
}

/**
 * g++ -o main 2_8.cpp -lpthread -Wall
 *
 */