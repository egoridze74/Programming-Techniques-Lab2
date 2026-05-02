#include "search_keys.h"
#include <algorithm>
#include <queue>
#include <stack>
#include <iostream>
#include <functional>


// ==================== Линейный поиск ====================
std::vector<ZagsRecord> linear_search(const std::vector<ZagsRecord>& data, const std::string& target) {
    std::vector<ZagsRecord> result;
    for (const auto& rec : data) {
        if (rec.groom_fio == target) {
            result.push_back(rec);
        }
    }
    return result;
}

// ==================== BinTree ====================
void BinTree::insert(BinTreeNode*& node, const ZagsRecord& rec) {
    if (!node) {
        node = new BinTreeNode(rec);
        return;
    }
    
    if (rec.groom_fio < node->record.groom_fio) {
        insert(node->left, rec);
    } else {
        insert(node->right, rec);
    }
}

void BinTree::insert(const ZagsRecord& rec) {
    insert(root, rec);
}

void BinTree::search_all(BinTreeNode* node, const std::string& target, std::vector<ZagsRecord>& result) const {
    if (!node) return;
    
    // Проверяем текущий узел
    if (node->record.groom_fio == target) {
        result.push_back(node->record);
    }
    
    // Ищем в левом поддереве
    if (target <= node->record.groom_fio) {
        search_all(node->left, target, result);
    }
    
    // Ищем в правом поддереве
    if (target >= node->record.groom_fio) {
        search_all(node->right, target, result);
    }
}

std::vector<ZagsRecord> BinTree::search(const std::string& target) const {
    std::vector<ZagsRecord> result;
    search_all(root, target, result);
    return result;
}

void BinTree::clear(BinTreeNode* node) {
    if (!node) return;
    clear(node->left);
    clear(node->right);
    delete node;
}

size_t BinTree::size() const {
    size_t count = 0;
    std::stack<BinTreeNode*> stack;
    if (root) stack.push(root);
    while (!stack.empty()) {
        BinTreeNode* node = stack.top();
        stack.pop();
        count++;
        if (node->left) stack.push(node->left);
        if (node->right) stack.push(node->right);
    }
    return count;
}

// ==================== Красно-черное дерево ====================
void RedBlackTree::rotate_left(RBTreeNode* x) {
    if (!x || !x->right) return;
    RBTreeNode* y = x->right;
    x->right = y->left;
    if (x->right) x->right->parent = x;
    y->parent = x->parent;
    
    if (!x->parent) {
        root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}

void RedBlackTree::rotate_right(RBTreeNode* y) {
    if (!y || !y->left) return;
    RBTreeNode* x = y->left;
    y->left = x->right;
    if (y->left) y->left->parent = y;
    x->parent = y->parent;
    
    if (!y->parent) {
        root = x;
    } else if (y == y->parent->left) {
        y->parent->left = x;
    } else {
        y->parent->right = x;
    }
    x->right = y;
    y->parent = x;
}

void RedBlackTree::fix_insert(RBTreeNode* z) {
    while (z != root && z->parent->color == Color::RED) {
        if (z->parent == z->parent->parent->left) {
            RBTreeNode* y = z->parent->parent->right;
            if (y && y->color == Color::RED) {
                z->parent->color = Color::BLACK;
                y->color = Color::BLACK;
                z->parent->parent->color = Color::RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    rotate_left(z);
                }
                z->parent->color = Color::BLACK;
                z->parent->parent->color = Color::RED;
                rotate_right(z->parent->parent);
            }
        } else {
            RBTreeNode* y = z->parent->parent->left;
            if (y && y->color == Color::RED) {
                z->parent->color = Color::BLACK;
                y->color = Color::BLACK;
                z->parent->parent->color = Color::RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    rotate_right(z);
                }
                z->parent->color = Color::BLACK;
                z->parent->parent->color = Color::RED;
                rotate_left(z->parent->parent);
            }
        }
    }
    root->color = Color::BLACK;
}

void RedBlackTree::insert(const ZagsRecord& rec) {
    RBTreeNode* z = new RBTreeNode(rec);
    RBTreeNode* y = nullptr;
    RBTreeNode* x = root;
    
    while (x) {
        y = x;
        if (rec.groom_fio < x->record.groom_fio) {
            x = x->left;
        } else {
            x = x->right;
        }
    }
    
    z->parent = y;
    if (!y) {
        root = z;
    } else if (rec.groom_fio < y->record.groom_fio) {
        y->left = z;
    } else {
        y->right = z;
    }
    
    fix_insert(z);
}

void RedBlackTree::search_all(RBTreeNode* node, const std::string& target, std::vector<ZagsRecord>& result) const {
    if (!node) return;
    
    // Проверяем текущий узел
    if (node->record.groom_fio == target) {
        result.push_back(node->record);
    }
    
    // Ищем в левом поддереве
    if (target <= node->record.groom_fio) {
        search_all(node->left, target, result);
    }
    
    // Ищем в правом поддереве
    if (target >= node->record.groom_fio) {
        search_all(node->right, target, result);
    }
}

std::vector<ZagsRecord> RedBlackTree::search(const std::string& target) const {
    std::vector<ZagsRecord> result;
    search_all(root, target, result);
    return result;
}

void RedBlackTree::clear(RBTreeNode* node) {
    if (!node) return;
    clear(node->left);
    clear(node->right);
    delete node;
}

size_t RedBlackTree::size() const {
    size_t count = 0;
    std::stack<RBTreeNode*> stack;
    if (root) stack.push(root);
    while (!stack.empty()) {
        RBTreeNode* node = stack.top();
        stack.pop();
        count++;
        if (node->left) stack.push(node->left);
        if (node->right) stack.push(node->right);
    }
    return count;
}

// ==================== Хеш-таблица ====================
size_t hash_string(const std::string& key, size_t table_size) {
    unsigned int hash = 0;
    for (char c : key) {
        hash += (unsigned char)(c);
        hash -= (hash << 13) | (hash >> 19);
    }
    return hash % table_size;
}

HashTable::HashTable(size_t size) : table(size), collision_count(0), total_inserts(0) {}

void HashTable::insert(const ZagsRecord& rec) {
    size_t idx = hash_string(rec.groom_fio, table.size());
    if (!table[idx].empty()) {
        collision_count++;
    }
    table[idx].push_back(rec);
    total_inserts++;
}

std::vector<ZagsRecord> HashTable::search(const std::string& target) const {
    size_t idx = hash_string(target, table.size());
    std::vector<ZagsRecord> result;
    for (const auto& rec : table[idx]) {
        if (rec.groom_fio == target) {
            result.push_back(rec);
        }
    }
    return result;
}

double HashTable::get_collision_rate() const {
    if (total_inserts == 0) return 0.0;
    return static_cast<double>(collision_count) / total_inserts;
}