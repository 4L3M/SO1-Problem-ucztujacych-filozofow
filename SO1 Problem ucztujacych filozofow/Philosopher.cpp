#include "Philosopher.h"
#include "Table.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <random>

Philosopher::Philosopher(int id, Table& table) : id(id), table(table) {
    thread = std::thread(&Philosopher::dine, this);
    thread.detach(); // Mo¿na te¿ u¿yæ join w Simulation
}

void Philosopher::dine() {
    while (true) {
        std::cout << "Filozof " << id << " czeka na pa³eczki\n";
        table.pick_up(id);

        std::cout << "Filozof " << id << " je\n";
        std::this_thread::sleep_for(std::chrono::seconds(2 + rand() % 3));

        table.put_down(id);
        std::cout << "Filozof " << id << " myœli\n";
        std::this_thread::sleep_for(std::chrono::seconds(2 + rand() % 4));
    }
}
