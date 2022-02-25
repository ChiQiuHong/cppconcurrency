/**
* @description: 2_3.cpp
* @brief 利用RAII过程等待线程结束
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

class thread_guard {
    std::thread& t;
public:
    explicit thread_guard(std::thread& t_)
        :t(t_) {    
    }
    ~thread_guard() {
        if(t.joinable()) {
            t.join();   // join()只能被调用一次，多次调用join()是错误行为
        }
    }
    // "=delete"限令编译器不得自动生成相关代码
    thread_guard(thread_guard const&) = delete;
    thread_guard& operator=(thread_guard const&) = delete;
};

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
    thread_guard g(t);
    do_something_in_current_thread();
}

int main() {

    f();

    return 0;
}


/**
 * g++ -o main 2_3.cpp -lpthread -Wall
 *
 */