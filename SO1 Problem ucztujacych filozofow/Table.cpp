#include "Table.h"

Table::Table(int size) : size(size), chopsticks(size) {}

void Table::pick_up(int id) {
    int left = id;
    int right = (id + 1) % size;

    // Aby unikn¹æ zakleszczenia: zmieniamy kolejnoœæ dla parzystych
    if (id % 2 == 0) {
        chopsticks[left].lock();
        chopsticks[right].lock();
    }
    else {
        chopsticks[right].lock();
        chopsticks[left].lock();
    }
}

void Table::put_down(int id) {
    int left = id;
    int right = (id + 1) % size;
    chopsticks[left].unlock();
    chopsticks[right].unlock();
}
