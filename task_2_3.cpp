#include <iostream>
#include <thread>
#include <mutex>

class Data
{
public:
	int prop1;
	float prop2;
	char prop3;
	std::mutex mtx;

	Data(int pr1, float pr2, char pr3) : prop1{pr1}, prop2{pr2}, prop3{pr3} {}

	void printData()
	{
		std::cout << "Prop 1 : " << prop1 << "; Prop 2 : " << prop2 << "; Prop 3 : " << prop3 << ";" << std::endl;
	}
};

void print_all_data(Data& a, Data& b)
{
	std::cout << "A:" << std::endl;
	a.printData();
	std::cout << "B:" << std::endl;
	b.printData();
}

void swap_data(Data& a, Data& b)
{
	std::swap(a.prop1, b.prop1);
	std::swap(a.prop2, b.prop2);
	std::swap(a.prop3, b.prop3);
}

void swap_lock(Data& a, Data& b)
{
	if (&a == &b) return;

	std::cout << "Before [std::lock]!" << std::endl;

	print_all_data(a, b);

	std::lock(a.mtx, b.mtx);

	std::lock_guard<std::mutex> lock1(a.mtx, std::adopt_lock);
	std::lock_guard<std::mutex> lock2(b.mtx, std::adopt_lock);

	swap_data(a, b);

	std::cout << "After [std::lock]!" << std::endl;

	print_all_data(a, b);

	std::cout << "-------------------------------" << std::endl;

}

void swap_scoped_lock(Data& a, Data& b) 
{
	if (&a == &b) return;

	std::cout << "Before [std::scoped_lock]!" << std::endl;

	print_all_data(a, b);

	std::scoped_lock lock(a.mtx, b.mtx);

	swap_data(a, b);

	std::cout << "After [std::scoped_lock]!" << std::endl;

	print_all_data(a, b);

	std::cout << "-------------------------------" << std::endl;
}

void swap_unique_lock(Data& a, Data& b) 
{
	if (&a == &b) return;

	std::cout << "Before [std::unique_lock]!" << std::endl;

	print_all_data(a, b);

	std::unique_lock<std::mutex> lock1(a.mtx, std::defer_lock);
	std::unique_lock<std::mutex> lock2(b.mtx, std::defer_lock);

	std::lock(lock1, lock2);

	swap_data(a, b);

	std::cout << "After [std::unique_lock]!" << std::endl;

	print_all_data(a, b);

	std::cout << "-------------------------------" << std::endl;
}

int main() 
{
	Data a(10, 0.5, 'a'), b(20, 1, 'b');

	// lock
	swap_lock(a, b);

	// scoped_lock
	swap_scoped_lock(a, b);

	// unique_lock
	swap_unique_lock(a, b);

	return 0;
}