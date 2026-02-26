#include <iostream>
#include <future>

void printArray(int *arr, int size)
{
	for (size_t i = 0; i < size; i++)
	{
		std::cout << arr[i] << "\t";
	}
}

void getMinIndex(int* arr, int i, int size, std::promise<int> Promise)
{
	int min = arr[i];
	int index = i;
	for (size_t j = i + 1; j < size; j++)
	{
		if (arr[j] < min)
		{
			min = arr[j];
			index = j;
		}
	}
	Promise.set_value(index);
}


int main()
{
	const int size = 9;
	int numbers[size]{ 12, 1, 0, -5, 23, 4, 10, 12, 2 };

	std::cout << "Before:\t";
	printArray(numbers, size);
	std::cout << std::endl;

	for (size_t i = 0; i < size - 1; i++)
	{
		std::promise<int> Promise;
		std::future<int> Future = Promise.get_future();
		auto res = std::async(getMinIndex, numbers, i, size, std::move(Promise));
		Future.wait();
		int index = Future.get();
		int min = numbers[index];
		int temp = numbers[i];
		numbers[i] = min;
		numbers[index] = temp;
	}

	std::cout << "After:\t";
	printArray(numbers, size);
	std::cout << std::endl;
	
	return EXIT_SUCCESS;
}