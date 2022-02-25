/**
* @description: 2_5.cpp
* @brief 从函数内部返回std::thread对象
* @date: 2022/02/25 星期五 15:11:24
*/
#include <iostream>
#include <thread>

void some_function() {
    std::cout << "This is some_function." << std::endl;
}

void some_other_function(int x) {
    std::cout << "This is some_other_function." << std::endl;
    std::cout << "x=" << x << std::endl;
}

std::thread f() {
    void some_function();
    return std::thread(some_function);
}

std::thread g() {
    void some_other_function(int x);
    std::thread t(some_other_function, 42);
    return t;
}

int main() {

    std::thread t1 = f();
    t1.join();
    std::thread t2 = g();
    t2.join();

    return 0;
}


/**
 * g++ -o main 2_5.cpp -lpthread -Wall
 *
 */