/**
* @description: 2_6.cpp
* @brief scoped_thread类及其用例
* @date: 2022/02/25 星期五 15:31:01
*/

#include <iostream>
#include <thread>

void do_something(int& i) {
    ++i;
}

void do_something_in_current_thread() {
    std::cout << "this is main thread." << std::endl;
} 

class scoped_thread {
    std::thread t;
public:
    explicit scoped_thread(std::thread t_)
        :t(std::move(t_)) {
        if(!t.joinable()) {
            throw std::logic_error("No thread");
        }
    }

    ~scoped_thread() {
        t.join();
    }

    scoped_thread(scoped_thread const&) = delete;
    scoped_thread& operator=(scoped_thread const&) = delete;
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
    // 这里直接向scoped_thread构造函数传入新线程，而非为其创建单独的具名变量
    scoped_thread t{std::thread(func(some_local_state))};
    do_something_in_current_thread();
}

int main() {

    f();

    return 0;
}


/**
 * g++ -o main 2_6.cpp -lpthread -Wall
 *
 */