#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

using namespace std;

mutex chopsticks[5];

static void philosopher_safe(int id) {
    int left = id;
    int right = (id + 1) % 5;

    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "Filozof " << id << " czeka na pa³eczki\n";

        if (id % 2 == 0) {
            chopsticks[left].lock();
            chopsticks[right].lock();
        }
        else {
            chopsticks[right].lock();
            chopsticks[left].lock();
        }

        std::cout << "Filozof " << id << " je\n";
        std::this_thread::sleep_for(std::chrono::seconds(2 + rand() % 3));

        chopsticks[right].unlock();
        chopsticks[left].unlock();

        std::cout << "Filozof " << id << " myœli\n";
        std::this_thread::sleep_for(std::chrono::seconds(2 + rand() % 4));
    }
}

void run_safe() {
    std::thread philosophers[5];
    for (int i = 0; i < 5; ++i)
        philosophers[i] = std::thread(philosopher_safe, i);

    for (auto& p : philosophers) p.join();
}
