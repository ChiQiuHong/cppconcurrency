/**
* @description: 2_9.cpp
* @brief 并行版的std::accumulate()的简单实现
* @date: 2022/02/25 星期五 17:22:54
*/

#include <iostream>
#include <thread>
#include <vector>
#include <numeric>
#include <ctime>
#include <unistd.h>
#include <boost/timer.hpp>
#include <chrono>

template<typename Iterator, typename T>
struct accumulate_block {
    void operator()(Iterator first, Iterator last, T& result) {
        result = std::accumulate(first, last, result);
    }
};

template<typename Iterator, typename T>
T paraller_accumulate(Iterator first, Iterator last, T init) {
    unsigned long const length = std::distance(first, last);
    // 如果传入的区间为空，就直接返回初始值 init
    if(!length)
        return init;
    // 每个线程的最低限定量
    unsigned long const min_per_thread = 25;
    // 得到线程的最大数量
    unsigned long const max_threads = 
        (length + min_per_thread - 1) / min_per_thread;
    unsigned long const hardware_threads = 
        std::thread::hardware_concurrency();
    // 对比算出最小线程数量和硬件线程数量，较小者即为实际需要运行的线程数量
    // 如果hardware_concurrency返回0，我们便要自己选择一个数量，这里设为2
    unsigned long const num_threads = 
        std::min(hardware_threads != 0 ? hardware_threads : 2, max_threads);
    // 得到各线程需分担的元素数量
    unsigned long const block_size = length / num_threads;

    std::vector<T> results(num_threads);    // 存放中间结果
    std::vector<std::thread> threads(num_threads - 1);  //存放线程
    
    Iterator block_start = first;
    for(unsigned long i = 0; i < (num_threads - 1); ++i) {
        Iterator block_end = block_start;
        std::advance(block_end, block_size);
        threads[i] = std::thread(
            accumulate_block<Iterator, T>(),
            block_start, block_end, std::ref(results[i]));
        block_start = block_end;
    }

    accumulate_block<Iterator, T>() (
        block_start, last, results[num_threads - 1]);
    
    for(auto& entry : threads)
        entry.join();
    
    return std::accumulate(results.begin(), results.end(), init);
};

int main() {
    std::vector<int> vi;
    for(int i = 0; i < 1000000; ++i) {
        vi.push_back(10);
    }

    int sum;
    // 多线程
    auto begin = std::chrono::high_resolution_clock::now();
    sum = paraller_accumulate(vi.begin(), vi.end(), 5);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> runtime {end - begin};
    std::cout << "sum = " << sum << "\tMulti-Thread runtime: \t" << runtime.count() << std::endl;

    // 单线程
    begin = std::chrono::high_resolution_clock::now();
    sum = std::accumulate(vi.begin(), vi.end(), 5);
    end = std::chrono::high_resolution_clock::now();
    runtime = end - begin;
    std::cout << "sum = " << sum << "\tOne Thread runtime: \t" << runtime.count() << std::endl;

    return 0;
}

/**
 * g++ -o main 2_9.cpp -lpthread -Wall -std=c++11
 *  
 * 
 */