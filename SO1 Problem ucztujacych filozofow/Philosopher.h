#pragma once
#include <thread>
#include <mutex>
#include <vector>

class Table;

class Philosopher {
public:
    Philosopher(int id, Table& table);
    void dine();
private:
    int id;
    Table& table;
    std::thread thread;
};
