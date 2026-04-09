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

// ==================== BST ====================
void BST::insert(std::unique_ptr<BSTNode>& node, const ZagsRecord& rec) {
    if (!node) {
        node = std::make_unique<BSTNode>(rec);
        return;
    }
    
    if (rec.groom_fio < node->record.groom_fio) {
        insert(node->left, rec);
    } else {
        insert(node->right, rec);
    }
}

void BST::insert(const ZagsRecord& rec) {
    insert(root, rec);
}

void BST::search_all(BSTNode* node, const std::string& target, std::vector<ZagsRecord>& result) const {
    if (!node) return;
    
    if (target < node->record.groom_fio) {
        search_all(node->left.get(), target, result);
    } else if (node->record.groom_fio < target) {
        search_all(node->right.get(), target, result);
    } else {
        // Нашли узел с нужным ФИО — нужно собрать все равные
        result.push_back(node->record);
        
        // Ищем в левом поддереве (могут быть равные)
        BSTNode* curr = node->left.get();
        while (curr) {
            if (curr->record.groom_fio == target) {
                result.push_back(curr->record);
                curr = curr->right.get();
            } else {
                curr = curr->right.get();
            }
        }
        
        // Ищем в правом поддереве
        curr = node->right.get();
        while (curr) {
            if (curr->record.groom_fio == target) {
                result.push_back(curr->record);
                curr = curr->left.get();
            } else {
                curr = curr->left.get();
            }
        }
    }
}

std::vector<ZagsRecord> BST::search(const std::string& target) const {
    std::vector<ZagsRecord> result;
    search_all(root.get(), target, result);
    return result;
}

size_t BST::size() const {
    size_t count = 0;
    std::stack<BSTNode*> stack;
    if (root) stack.push(root.get());
    while (!stack.empty()) {
        BSTNode* node = stack.top();
        stack.pop();
        count++;
        if (node->left) stack.push(node->left.get());
        if (node->right) stack.push(node->right.get());
    }
    return count;
}

// ==================== Красно-черное дерево ====================
void RedBlackTree::rotate_left(RBTNode* x) {
    if (!x || !x->right) return;
    RBTNode* y = x->right.get();
    x->right = std::move(y->left);
    if (x->right) x->right->parent = x;
    y->parent = x->parent;
    
    if (!x->parent) {
        root.reset(y);
    } else if (x == x->parent->left.get()) {
        x->parent->left.reset(y);
    } else {
        x->parent->right.reset(y);
    }
    y->left.reset(x);
    x->parent = y;
}

void RedBlackTree::rotate_right(RBTNode* y) {
    if (!y || !y->left) return;
    RBTNode* x = y->left.get();
    y->left = std::move(x->right);
    if (y->left) y->left->parent = y;
    x->parent = y->parent;
    
    if (!y->parent) {
        root.reset(x);
    } else if (y == y->parent->left.get()) {
        y->parent->left.reset(x);
    } else {
        y->parent->right.reset(x);
    }
    x->right.reset(y);
    y->parent = x;
}

void RedBlackTree::fix_insert(RBTNode* z) {
    while (z != root.get() && z->parent->color == Color::RED) {
        if (z->parent == z->parent->parent->left.get()) {
            RBTNode* y = z->parent->parent->right.get();
            if (y && y->color == Color::RED) {
                z->parent->color = Color::BLACK;
                y->color = Color::BLACK;
                z->parent->parent->color = Color::RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right.get()) {
                    z = z->parent;
                    rotate_left(z);
                }
                z->parent->color = Color::BLACK;
                z->parent->parent->color = Color::RED;
                rotate_right(z->parent->parent);
            }
        } else {
            RBTNode* y = z->parent->parent->left.get();
            if (y && y->color == Color::RED) {
                z->parent->color = Color::BLACK;
                y->color = Color::BLACK;
                z->parent->parent->color = Color::RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left.get()) {
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
    RBTNode* z = new RBTNode(rec);
    RBTNode* y = nullptr;
    RBTNode* x = root.get();
    
    while (x) {
        y = x;
        if (rec.groom_fio < x->record.groom_fio) {
            x = x->left.get();
        } else {
            x = x->right.get();
        }
    }
    
    z->parent = y;
    if (!y) {
        root.reset(z);
    } else if (rec.groom_fio < y->record.groom_fio) {
        y->left.reset(z);
    } else {
        y->right.reset(z);
    }
    
    fix_insert(z);
}

void RedBlackTree::search_all(RBTNode* node, const std::string& target, std::vector<ZagsRecord>& result) const {
    if (!node) return;
    
    if (target < node->record.groom_fio) {
        search_all(node->left.get(), target, result);
    } else if (node->record.groom_fio < target) {
        search_all(node->right.get(), target, result);
    } else {
        result.push_back(node->record);
        
        // Поиск равных в левом поддереве
        RBTNode* curr = node->left.get();
        while (curr && curr->record.groom_fio == target) {
            result.push_back(curr->record);
            curr = curr->right.get();
        }
        
        // Поиск равных в правом поддереве
        curr = node->right.get();
        while (curr && curr->record.groom_fio == target) {
            result.push_back(curr->record);
            curr = curr->left.get();
        }
    }
}

std::vector<ZagsRecord> RedBlackTree::search(const std::string& target) const {
    std::vector<ZagsRecord> result;
    search_all(root.get(), target, result);
    return result;
}

size_t RedBlackTree::size() const {
    size_t count = 0;
    std::stack<RBTNode*> stack;
    if (root) stack.push(root.get());
    while (!stack.empty()) {
        RBTNode* node = stack.top();
        stack.pop();
        count++;
        if (node->left) stack.push(node->left.get());
        if (node->right) stack.push(node->right.get());
    }
    return count;
}

// ==================== Хеш-таблица ====================
size_t hash_string(const std::string& key, size_t table_size) {
    // Полиномиальный хеш (эффективен для строк)
    size_t hash = 0;
    const size_t p = 31; // простое число
    size_t p_pow = 1;
    
    for (char c : key) {
        hash = (hash + (c * p_pow) % table_size) % table_size;
        p_pow = (p_pow * p) % table_size;
    }
    return hash;
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