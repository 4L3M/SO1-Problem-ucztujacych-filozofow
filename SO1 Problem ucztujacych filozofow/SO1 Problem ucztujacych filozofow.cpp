#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <atomic>
#include <chrono>
#include <random>
#include <csignal>
#include <string>

constexpr int NUM_PHILOSOPHERS = 5;
std::mutex mutexes[NUM_PHILOSOPHERS];
std::atomic<int> chopstick_state[NUM_PHILOSOPHERS]; // -1: wolna, 0–4: zajęta
std::atomic<int> eat_count[NUM_PHILOSOPHERS];       // licznik posiłków
std::atomic<bool> running{ true };
int mode = 3;

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> think_time(1000, 4000);
std::uniform_int_distribution<> eat_time(2000, 5000);

const std::string names[NUM_PHILOSOPHERS] = { "Yoda", "Gandalf", "Sokrates", "Nietzsche", "Laozi" };

void signal_handler(int) {
    running = false;
}

void print_state() {
    while (running) {
        std::cout << "\n[STAN PAŁECZEK]\n";
        for (int i = 0; i < NUM_PHILOSOPHERS; ++i) {
            int user = chopstick_state[i];
            if (user == -1)
                std::cout << "Pałeczka " << i << ": wolna\n";
            else
                std::cout << "Pałeczka " << i << ": zajęta przez " << names[user] << "\n";
        }

        std::cout << "\n[NAJEDZENIE FILOZOFÓW]:\n";
        for (int i = 0; i < NUM_PHILOSOPHERS; ++i)
            std::cout << names[i] << ": " << eat_count[i] << " razy\n";

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void philosopher_deadlock(int id) {
    int left = id;
    int right = (id + 1) % NUM_PHILOSOPHERS;

    while (running) {
        mutexes[right].lock();
        chopstick_state[right] = id;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        mutexes[left].lock();
        chopstick_state[left] = id;

        std::cout << names[id] << " je 🍜 (deadlock)\n";
        eat_count[id]++;
        std::this_thread::sleep_for(std::chrono::milliseconds(eat_time(gen)));

        chopstick_state[left] = -1;
        chopstick_state[right] = -1;
        mutexes[left].unlock();
        mutexes[right].unlock();

        std::cout << names[id] << " myśli 💭\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(think_time(gen)));
    }
}

void philosopher_starvation(int id) {
    int left = id;
    int right = (id + 1) % NUM_PHILOSOPHERS;

    while (running) {
        if (mutexes[left].try_lock()) {
            if (mutexes[right].try_lock()) {
                chopstick_state[left] = id;
                chopstick_state[right] = id;

                std::cout << names[id] << " je 🍜 (starvation)\n";
                eat_count[id]++;
                std::this_thread::sleep_for(std::chrono::milliseconds(eat_time(gen)));

                chopstick_state[left] = -1;
                chopstick_state[right] = -1;
                mutexes[right].unlock();
                mutexes[left].unlock();
            }
            else {
                mutexes[left].unlock();
            }
        }

        std::cout << names[id] << " myśli 💭\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(think_time(gen)));
    }
}

void philosopher_safe(int id) {
    int left = id;
    int right = (id + 1) % NUM_PHILOSOPHERS;

    while (running) {
        if (id % 2 == 0) {
            mutexes[left].lock();
            chopstick_state[left] = id;
            mutexes[right].lock();
            chopstick_state[right] = id;
        }
        else {
            mutexes[right].lock();
            chopstick_state[right] = id;
            mutexes[left].lock();
            chopstick_state[left] = id;
        }

        std::cout << names[id] << " je 🍜 (safe)\n";
        eat_count[id]++;
        std::this_thread::sleep_for(std::chrono::milliseconds(eat_time(gen)));

        chopstick_state[left] = -1;
        chopstick_state[right] = -1;
        mutexes[left].unlock();
        mutexes[right].unlock();

        std::cout << names[id] << " myśli 💭\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(think_time(gen)));
    }
}

void philosopher_runner(int id) {
    switch (mode) {
    case 1:
        philosopher_deadlock(id);
        break;
    case 2:
        philosopher_starvation(id);
        break;
    case 3:
    default:
        philosopher_safe(id);
        break;
    }
}

int main() {
    std::cout << "Wybierz tryb działania:\n";
    std::cout << "1 – Deadlock (zakleszczenie)\n";
    std::cout << "2 – Starvation (zagłodzenie)\n";
    std::cout << "3 – Safe (brak zakleszczenia i zagłodzenia)\n";
    std::cin >> mode;

    std::signal(SIGINT, signal_handler);

    for (int i = 0; i < NUM_PHILOSOPHERS; ++i) {
        chopstick_state[i] = -1;
        eat_count[i] = 0;
    }

    std::vector<std::thread> threads;
    for (int i = 0; i < NUM_PHILOSOPHERS; ++i)
        threads.emplace_back(philosopher_runner, i);

    std::thread monitor(print_state);

    for (auto& t : threads)
        t.join();
    monitor.join();

    std::cout << "Symulacja zakończona.\n";
    return 0;
}
