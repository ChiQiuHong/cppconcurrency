/**
* @description: 2_2.cpp
* @brief 等待线程结束
* @date: 2022/02/24 星期四 19:09:28
*/
#include <iostream>
#include <thread>
#include <unistd.h>

void do_something(int& i) {
    ++i;
}

void do_something_in_current_thread() {
    std::cout << "this is main thread." << std::endl;
} 

struct func {
    int& i;
    func(int& i_)
        :i(i_) {
    }

    void operator()() {
        std::cout << "before: " << i << std::endl;
        for(unsigned j = 0; j < 1000000; ++j) {
            do_something(i);    // 隐患：可能访问悬空引用
        }
        std::cout << "after: " << i << std::endl;
    }
};

void f() {
    int some_local_state = 0;
    func my_func(some_local_state);
    std::thread t(my_func);
    try {
        do_something_in_current_thread();
    } catch(...) {
        t.join();
        throw;
    }
    t.join();
}

int main() {

    f();

    return 0;
}


/**
 * g++ -o main 2_2.cpp -lpthread -Wall
 * try/catch块的使用保证了新线程在函数f()退出前终结
 * （无论f()是正常退出还是因为异常而退出）
 * try/ catch 块稍显冗余，还容易引发作用域的轻微错乱
 * 故它并非理想方案。
 */