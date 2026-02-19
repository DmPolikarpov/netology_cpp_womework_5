#include <iostream>
#include <thread>

class BankQueue {
private:
    int client_count;
    int max_clients;
    bool clients_done;

public:
    BankQueue(int max) : client_count(0), max_clients(max), clients_done(false) {}

    void addClient() 
    {
        while (true) 
        {
            std::this_thread::sleep_for(std::chrono::seconds(1));

            if (client_count >= max_clients) 
            {
                clients_done = true;
                break;
            }

            client_count++;
            std::cout << "A client has come. Current client number: " << client_count << std::endl;
        }
    }

    void serveClient() 
    {
        while (true) 
        {

            if (clients_done && client_count == 0) 
            {
                break;
            }

            if (client_count > 0) 
            {
                std::this_thread::sleep_for(std::chrono::seconds(2));

                client_count--;
                std::cout << "A client is served. Remain: " << client_count << std::endl;
            }
        }

        std::cout << "All done!" << std::endl;
    }

    void run() 
    {
        std::thread thread1(&BankQueue::addClient, this);
        std::thread thread2(&BankQueue::serveClient, this);

        thread1.join();
        thread2.join();
    }
};

int main()
{
    int max_clients;

    std::cout << "Enter max number of clients: ";
    std::cin >> max_clients;

    if (max_clients <= 0) {
        std::cout << "Number of clients should be a positive number" << std::endl;
        return 1;
    }

    BankQueue queue(max_clients);

    std::cout << "Work starts..." << std::endl;
    std::cout << "Max clients number: " << max_clients << std::endl;
    std::cout << "----------------------------------------" << std::endl;

    queue.run();


	return EXIT_SUCCESS;
}