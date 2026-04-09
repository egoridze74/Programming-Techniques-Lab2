/**
 * @file main.cpp
 * @brief Сравнение методов поиска по ФИО жениха
 */

#include "zags_record.h"
#include "utils.h"
#include "search_keys.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <chrono>
#include <random>
#include <set>

template<typename Func>
double measure_search_time(Func search_func, int repeats = 5) {
    double total = 0.0;
    for (int i = 0; i < repeats; ++i) {
        auto start = std::chrono::high_resolution_clock::now();
        auto result = search_func();
        auto end = std::chrono::high_resolution_clock::now();
        total += std::chrono::duration<double, std::milli>(end - start).count();

        volatile auto dummy = result.size();
        (void)dummy;
    }
    return total / repeats;
}

int main() {
    std::vector<size_t> sizes = {100, 500, 1000, 2000, 5000, 10000, 20000, 50000, 100000, 200000, 500000, 1000000};
    
    std::ofstream results("search_times.csv");
    results << "size,linear_ms,bst_ms,rbt_ms,hash_ms,multimap_ms,collision_rate\n";
    
    std::ofstream collisions_file("collisions.csv");
    collisions_file << "size,collision_rate\n";
    
    std::random_device rd;
    std::mt19937 gen(rd());
    
    for (size_t size : sizes) {
        std::string filename = "data/zags_" + std::to_string(size) + ".csv";
        std::cout << "\n=== Testing size " << size << " ===" << std::endl;
        
        auto data = read_csv(filename);
        if (data.empty()) {
            std::cout << "File not found: " << filename << ", skipping..." << std::endl;
            continue;
        }
        
        // Выбираем случайное ФИО из данных для поиска (которое точно существует)
        std::uniform_int_distribution<size_t> dist(0, data.size() - 1);
        std::string target = data[dist(gen)].groom_fio;
        
        std::cout << "Searching for: " << target << std::endl;
        
        // 1. Линейный поиск
        double linear_time = measure_search_time(
            [&]() { return linear_search(data, target); }, 3);
        
        // 2. BST
        BST bst;
        for (const auto& rec : data) bst.insert(rec);
        double bst_time = measure_search_time(
            [&]() { return bst.search(target); }, 3);
        
        // 3. Красно-черное дерево
        RedBlackTree rbt;
        for (const auto& rec : data) rbt.insert(rec);
        double rbt_time = measure_search_time(
            [&]() { return rbt.search(target); }, 3);
        
        // 4. Хеш-таблица (размер ~2x от данных, но не меньше 10007)
        size_t table_size = std::max(size_t(10007), size * 2);
        HashTable ht(table_size);
        for (const auto& rec : data) ht.insert(rec);
        double hash_time = measure_search_time(
            [&]() { return ht.search(target); }, 3);
        double collision_rate = ht.get_collision_rate();
        
        // 5. std::multimap (ключ = строка)
        std::multimap<std::string, ZagsRecord> mmap;
        for (const auto& rec : data) mmap.insert({rec.groom_fio, rec});
        double multimap_time = measure_search_time(
            [&]() {
                std::vector<ZagsRecord> res;
                auto range = mmap.equal_range(target);
                for (auto it = range.first; it != range.second; ++it) {
                    res.push_back(it->second);
                }
                return res;
            }, 3);
        
        results << size << ","
                << linear_time << ","
                << bst_time << ","
                << rbt_time << ","
                << hash_time << ","
                << multimap_time << ","
                << collision_rate << "\n";
        
        collisions_file << size << "," << collision_rate << "\n";
        
        std::cout << "Linear:     " << linear_time << " ms" << std::endl;
        std::cout << "BST:        " << bst_time << " ms" << std::endl;
        std::cout << "RBT:        " << rbt_time << " ms" << std::endl;
        std::cout << "Hash:       " << hash_time << " ms" << std::endl;
        std::cout << "multimap:   " << multimap_time << " ms" << std::endl;
        std::cout << "Collisions: " << collision_rate * 100 << "%" << std::endl;
    }
    
    std::cout << "\nResults saved to search_times.csv and collisions.csv" << std::endl;
    return 0;
}