/**
* @description: 2_1.cpp
* @brief 当前线程的函数已返回，新线程却仍能访问其局部变量
* @date: 2022/02/24 星期四 19:09:28
*/
#include <iostream>
#include <thread>
#include <unistd.h>

void do_something(int& i) {
    ++i;
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

void oops() {
    std::cout << "new thread" << std::endl;
    int some_local_state = 0;
    func my_func(some_local_state);
    std::thread my_thread(my_func);
    my_thread.detach();     // 不等待新线程结束
    // my_thread.join();
}   // 新线程可能仍在运行，而主线程的函数却已结束

int main() {

    oops();
    usleep(400);

    return 0;
}


/**
 * g++ -o main 2_1.cpp -lpthread -Wall
 * 调用detach后，oops结束，而my_thread新线程仍继续运行。
 * 由于some_local_state是局部变量，它会在oops结束后销毁
 * 在for循环里do_something的下一次调用就会访问已经被销毁的局部变量
 * 运行报错：Segmentation fault
 */