#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <iomanip>
#include <numeric>

class VectorSumCalculator 
{
private:
    std::vector<int> vec1;
    std::vector<int> vec2;
    std::vector<int> result;
    size_t size;

public:
    VectorSumCalculator(size_t n) : size(n) 
    {
        vec1.resize(n);
        vec2.resize(n);
        result.resize(n);

        // fill vectors
        for (size_t i = 0; i < n; ++i) 
        {
            vec1[i] = rand() % 100;
            vec2[i] = rand() % 100;
        }
    }

    void calcSum(int num_threads)
    {
        std::vector<std::thread> threads;
        size_t chunk_size = size / num_threads;

        auto worker = [this](size_t start, size_t end) {
                for (size_t i = start; i < end; ++i) 
                {
                    result[i] = vec1[i] + vec2[i];
                }
            };

        for (int i = 0; i < num_threads; ++i) 
        {
            size_t start = i * chunk_size;
            size_t end = (i == num_threads - 1) ? size : start + chunk_size;
            threads.emplace_back(worker, start, end);
        }

        for (auto& t : threads) 
        {
            t.join();
        }
    }
};

int main() {
    // get core numbers
    unsigned int num_cores = std::thread::hardware_concurrency();
    std::cout << "Core numbers - " << num_cores << std::endl;

    // thread counts
    std::vector<int> thread_counts = { 1, 2, 4, 8, 16 };

    // vector sizes
    std::vector<size_t> sizes = { 10000, 100000, 1000000 };


    // table header
    std::cout << std::left << std::setw(18) << " ";
    for (size_t size : sizes) 
    {
        std::cout << std::setw(15) << size;
    }
    std::cout << std::endl;

    for (int num_threads : thread_counts) 
    {
        // thread number
        std::cout << std::left << num_threads << " threads";

        // calc size and time
        for (size_t size : sizes) 
        {
            VectorSumCalculator calculator(size);

            auto start_time = std::chrono::high_resolution_clock::now();

            calculator.calcSum(num_threads);

            auto end_time = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed = end_time - start_time;
            std::cout << "\t" << std::setw(10) << std::fixed << std::setprecision(7) << elapsed.count();
        }
        std::cout << std::endl;
    }

    return 0;
}