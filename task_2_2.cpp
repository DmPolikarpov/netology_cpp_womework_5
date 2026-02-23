#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>
#include <string>
#include <iomanip>

// console access mutex
std::mutex console_mtx;

// function to set cursor position, where:
// line - number of line to move the cursor
// col - cursor position on the line
void set_cursor_position(int line, int col) {
    std::cout << "\033[" << line << ";" << col << "H";
}

// function to fill progress bar in the console
void worker(int index, int total_steps) {
    // start time
    auto start_time = std::chrono::steady_clock::now();

    // thread id
    std::thread::id thread_id = std::this_thread::get_id();

    // progress bar length
    int bar_width = 20;
    // iterate through all steps and fill progress bar
    for (int i = 0; i <= total_steps; ++i) {
        // delay
        int delay = 10 * ( index + 1 ) * (i + 1) * rand() % 1000;
        // random pause for better visual effect
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        // get start data for current step
        float progress = static_cast<float>(i) / total_steps;
        int filled = static_cast<int>(progress * bar_width);
        // block the mutex
        std::lock_guard<std::mutex> lock(console_mtx);

        // set cursor position
        set_cursor_position(index + 1, 1);
        // show thread id
        std::cout << index << " | ID: " << thread_id << " \t | \t [";
        // fill progress bar
        for (int j = 0; j < bar_width; ++j) {
            if (j < filled) std::cout << "*"; // filled part
            else std::cout << " ";           // empty part
        }

        std::cout << "] " << std::fixed << std::setprecision(0);
        std::cout << std::flush;
    }
    // calculate common time to fill the whole progress bas to the current thread
    auto end_time = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed = end_time - start_time;

    // block mutex again to show common time
    std::lock_guard<std::mutex> lock(console_mtx);
    set_cursor_position(index + 1, 1);
    set_cursor_position(index + 1, 55);
    std::cout << "| Time: " << std::fixed << std::setprecision(2) << elapsed.count() << "s" << std::endl;
}

int main() {
    
    // create and fill variables
    int num_threads{ 1 };
    int calculation_steps{ 1 };

    std::cout << "Enter number of threads: ";
    std::cin >> num_threads;

    std::cout << "Enter number of calculation steps: ";
    std::cin >> calculation_steps;

    // clear console
    std::cout << "\033[2J";

    // create vector to store all created threads
    std::vector<std::thread> threads;

    // create and run threads
    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back(worker, i, calculation_steps);
    }

    for (auto& t : threads) {
        t.join();
    }

    // move cursor to the end line
    set_cursor_position(num_threads + 2, 1);
    std::cout << "Calculation finished." << std::endl;

    return 0;
}