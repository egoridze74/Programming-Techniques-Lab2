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

// Линейный поиск
/**
 * @brief Линейный поиск всех записей с заданным ФИО жениха
 * @param data Массив записей
 * @param target Искомое ФИО
 * @return Вектор найденных записей
 */
std::vector<ZagsRecord> linear_search(const std::vector<ZagsRecord>& data, const std::string& target);

// BinTree (бинарное дерево поиска)
struct BinTreeNode {
    ZagsRecord record;
    BinTreeNode* left;
    BinTreeNode* right;
    BinTreeNode(const ZagsRecord& rec) : record(rec), left(nullptr), right(nullptr) {}
};

class BinTree {
private:
    BinTreeNode* root;
    void insert(BinTreeNode*& node, const ZagsRecord& rec);
    void search_all(BinTreeNode* node, const std::string& target, std::vector<ZagsRecord>& result) const;
    void clear(BinTreeNode* node);

public:
     BinTree() : root(nullptr) {}
    ~BinTree() { clear(root); }
    
    BinTree(const BinTree&) = delete;
    BinTree& operator=(const BinTree&) = delete;

    void insert(const ZagsRecord& rec);
    std::vector<ZagsRecord> search(const std::string& target) const;
    size_t size() const;
};

// Красно-черное дерево
enum class Color { RED, BLACK };

struct RBTreeNode {
    ZagsRecord record;
    Color color;
    RBTreeNode* left;
    RBTreeNode* right;
    RBTreeNode* parent;
    RBTreeNode(const ZagsRecord& rec) 
        : record(rec), color(Color::RED), left(nullptr), right(nullptr), parent(nullptr) {}
};

class RedBlackTree {
private:
    RBTreeNode* root;

    void rotate_left(RBTreeNode* x);
    void rotate_right(RBTreeNode* y);
    void fix_insert(RBTreeNode* z);
    void search_all(RBTreeNode* node, const std::string& target, std::vector<ZagsRecord>& result) const;
    void clear(RBTreeNode* node);
    
public:
    RedBlackTree() : root(nullptr) {}
    ~RedBlackTree() { clear(root); }
    
    RedBlackTree(const RedBlackTree&) = delete;
    RedBlackTree& operator=(const RedBlackTree&) = delete;

    void insert(const ZagsRecord& rec);
    std::vector<ZagsRecord> search(const std::string& target) const;
    size_t size() const;
};

// Хеш-таблица
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
    size_t collision_count;
    size_t total_inserts;
    
public:
    std::vector<std::vector<ZagsRecord>> table;
    HashTable(size_t size = 10007);
    void insert(const ZagsRecord& rec);
    std::vector<ZagsRecord> search(const std::string& target) const;
    double get_collision_rate() const;
    size_t get_collision_count() const { return collision_count; }
};

#endif