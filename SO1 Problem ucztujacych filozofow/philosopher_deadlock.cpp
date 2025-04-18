#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <random>

using namespace std;

mutex chopsticks[5];

static void philosopher_dead(int id) {
    int left = id;
    int right = (id + 1) % 5;

    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "Filozof " << id << " czeka na pa�eczki\n";

        chopsticks[left].lock();
        chopsticks[right].lock();

        std::cout << "Filozof " << id << " je\n";
        std::this_thread::sleep_for(std::chrono::seconds(2 + rand() % 3));

        chopsticks[right].unlock();
        chopsticks[left].unlock();

        std::cout << "Filozof " << id << " my�li\n";
        std::this_thread::sleep_for(std::chrono::seconds(2 + rand() % 4));
    }
}

void run_deadlock() {
    std::thread philosophers[5];
    for (int i = 0; i < 5; ++i)
        philosophers[i] = std::thread(philosopher_dead, i);

    for (auto& p : philosophers) p.join();
}
