/**
 * @file search_keys.h
 * @brief Реализация методов поиска по ключу (ФИО жениха)
 */

#ifndef SEARCH_KEYS_H
#define SEARCH_KEYS_H

#include "zags_record.h"
#include <vector>
#include <string>
#include <unordered_map>
#include <map>
#include <memory>

// ==================== Линейный поиск ====================
/**
 * @brief Линейный поиск всех записей с заданным ФИО жениха
 * @param data Массив записей
 * @param target Искомое ФИО
 * @return Вектор найденных записей
 */
std::vector<ZagsRecord> linear_search(const std::vector<ZagsRecord>& data, const std::string& target);

// ==================== BST (бинарное дерево поиска) ====================
struct BSTNode {
    ZagsRecord record;
    std::unique_ptr<BSTNode> left;
    std::unique_ptr<BSTNode> right;
    BSTNode(const ZagsRecord& rec) : record(rec), left(nullptr), right(nullptr) {}
};

class BST {
private:
    std::unique_ptr<BSTNode> root;
    void insert(std::unique_ptr<BSTNode>& node, const ZagsRecord& rec);
    void search_all(BSTNode* node, const std::string& target, std::vector<ZagsRecord>& result) const;
public:
    void insert(const ZagsRecord& rec);
    std::vector<ZagsRecord> search(const std::string& target) const;
    size_t size() const;
};

// ==================== Красно-черное дерево ====================
enum class Color { RED, BLACK };

struct RBTNode {
    ZagsRecord record;
    Color color;
    std::unique_ptr<RBTNode> left;
    std::unique_ptr<RBTNode> right;
    RBTNode* parent;
    RBTNode(const ZagsRecord& rec) 
        : record(rec), color(Color::RED), left(nullptr), right(nullptr), parent(nullptr) {}
};

class RedBlackTree {
private:
    std::unique_ptr<RBTNode> root;
    void rotate_left(RBTNode* x);
    void rotate_right(RBTNode* y);
    void fix_insert(RBTNode* z);
    void search_all(RBTNode* node, const std::string& target, std::vector<ZagsRecord>& result) const;
public:
    void insert(const ZagsRecord& rec);
    std::vector<ZagsRecord> search(const std::string& target) const;
    size_t size() const;
};

// ==================== Хеш-таблица ====================
/**
 * @brief Хеш-функция для строки (ФИО)
 * @param key Строка ключа
 * @param table_size Размер таблицы
 * @return Хеш-значение
 */
size_t hash_string(const std::string& key, size_t table_size);

/**
 * @brief Хеш-таблица с методом цепочек для разрешения коллизий
 */
class HashTable {
private:
    std::vector<std::vector<ZagsRecord>> table;
    size_t collision_count;
    size_t total_inserts;
    
public:
    HashTable(size_t size = 10007); // простое число
    void insert(const ZagsRecord& rec);
    std::vector<ZagsRecord> search(const std::string& target) const;
    double get_collision_rate() const;
    size_t get_collision_count() const { return collision_count; }
};

#endif