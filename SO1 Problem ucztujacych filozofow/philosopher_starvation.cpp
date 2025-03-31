#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

using namespace std;

std::mutex chopsticks[5];
std::mutex table;

void philosopher_starve(int id) {
    int left = id;
    int right = (id + 1) % 5;

    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "Filozof " << id << " czeka na pa³eczki\n";

        table.lock();  // tylko jeden filozof mo¿e próbowaæ
        chopsticks[left].lock();
        chopsticks[right].lock();
        table.unlock();

        std::cout << "Filozof " << id << " je\n";
        std::this_thread::sleep_for(std::chrono::seconds(2 + rand() % 3));

        chopsticks[right].unlock();
        chopsticks[left].unlock();

        std::cout << "Filozof " << id << " myœli\n";
        std::this_thread::sleep_for(std::chrono::seconds(2 + rand() % 4));
    }
}

void run_starvation() {
    std::thread philosophers[5];
    for (int i = 0; i < 5; ++i)
        philosophers[i] = std::thread(philosopher_starve, i);

    for (auto& p : philosophers) p.join();
}
