#include <iostream>
#include <vector>
#include <future>
#include <numeric>

template <typename Iterator, typename Function>
void parallel_for_each(Iterator begin, Iterator end, Function func) {
    auto size = std::distance(begin, end);

    if (size < 11) {
        std::for_each(begin, end, func);
        return;
    }


    Iterator mid = begin;
    std::advance(mid, size / 2);

    auto first_half = std::async(parallel_for_each<Iterator, Function>, begin, mid, func);
    parallel_for_each(mid, end, func);

    first_half.get();
}


int main() {
    int size = 100;
    std::vector<int> numbers(size);
    std::iota(numbers.begin(), numbers.end(), 1);

    parallel_for_each(numbers.begin(), numbers.end(), [](int& x) {
        x = x * x;
        });


    for (size_t i = 0; i < size; ++i) {
        std::cout << numbers[i] << " ";
    }

    return EXIT_SUCCESS;
}