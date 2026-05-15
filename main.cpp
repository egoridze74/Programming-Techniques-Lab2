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


template<typename Func>
double measure_search_time(Func search_func, int repeats = 5) {
    double total = 0.0;
    for (int i = 0; i < repeats; ++i) {
        auto start = std::chrono::high_resolution_clock::now();
        auto result = search_func();
        auto end = std::chrono::high_resolution_clock::now();
        total += std::chrono::duration<double, std::milli>(end - start).count();

        auto dummy = result.size();
        (void)dummy;
    }
    return total / repeats;
}

int main() {
    setlocale(LC_ALL, "Russian");
    
    std::vector<size_t> sizes = {100, 200, 500, 1000, 2000, 5000, 10000, 20000, 50000, 100000, 200000, 500000, 1000000};
    
    std::ofstream results("search_times.csv");
    results << "size,linear_ms,bintree_ms,rbtree_ms,hash_ms,multimap_ms,collision_rate\n";
    
    std::ofstream collisions_file("collisions.csv");
    collisions_file << "size,collision_rate\n";
    
    
    for (size_t size : sizes) {
        std::string filename = "data/zags_" + std::to_string(size) + ".csv";
        std::cout << "\n=== Testing size " << size << " ===" << std::endl;
        
        auto data = read_csv(filename);
        if (data.empty()) {
            std::cout << "File not found: " << filename << ", skipping..." << std::endl;
            continue;
        }
        
        // Выбираем случайное ФИО из данных для поиска (которое точно существует)
        std::string target = data[data.size() / 2].groom_fio;
        
        std::cout << "Searching for: " << target << std::endl;
        
        // 1. Линейный поиск
        double linear_time = measure_search_time(
            [&]() { return linear_search(data, target); }, 3);
        
        // 2. BinTree
        BinTree bintree;
        for (const auto& rec : data) bintree.insert(rec);
        double bintree_time = measure_search_time(
            [&]() { return bintree.search(target); }, 3);
        
        // 3. Красно-черное дерево
        RedBlackTree rbtree;
        for (const auto& rec : data) rbtree.insert(rec);
        double rbtree_time = measure_search_time(
            [&]() { return rbtree.search(target); }, 3);
        
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
                << bintree_time << ","
                << rbtree_time << ","
                << hash_time << ","
                << multimap_time << ","
                << collision_rate << "\n";
        
        collisions_file << size << "," << collision_rate << "\n";
        
        std::cout << "Linear:     " << linear_time << " ms" << std::endl;
        std::cout << "BinTree:    " << bintree_time << " ms" << std::endl;
        std::cout << "RBTree:     " << rbtree_time << " ms" << std::endl;
        std::cout << "Hash:       " << hash_time << " ms" << std::endl;
        std::cout << "Multimap:   " << multimap_time << " ms" << std::endl;
        std::cout << "Collisions: " << collision_rate * 100 << "%" << std::endl;
    }
    
    std::cout << "\nResults saved to search_times.csv and collisions.csv" << std::endl;
    return 0;
}