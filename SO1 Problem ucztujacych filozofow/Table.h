#pragma once
#include <mutex>
#include <vector>

class Table {
public:
    Table(int size);
    void pick_up(int id);
    void put_down(int id);
private:
    int size;
    std::vector<std::mutex> chopsticks;
};
