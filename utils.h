/**
 * @file utils.h
 * @brief Вспомогательные функции: ввод/вывод, замеры времени
 */

#ifndef UTILS_H
#define UTILS_H

#include "zags_record.h"
#include <chrono>
#include <vector>
#include <map>
#include <iostream>


/**
 * @brief Чтение записей ЗАГСа из CSV файла
 * @param filename Путь к входному CSV файлу
 * @return Вектор ZagsRecord
 */
std::vector<ZagsRecord> read_csv(const std::string& filename);

/**
 * @brief Запись отсортированных записей в CSV
 * @param data Отсортированный вектор ZagsRecord
 * @param filename Путь к выходному CSV файлу
 */
void write_csv(const std::vector<ZagsRecord>& data, const std::string& filename);

/**
 * @brief Измерение времени выполнения сортировки
 * @tparam Func Тип функции сортировки
 * @param sort_func Функция сортировки (void(vector&))
 * @param original_data Исходные данные
 * @return Время выполнения в миллисекундах
 */
template<typename Func>
double measure_time(Func sort_func, const std::vector<ZagsRecord>& original_data) {
    auto copy = original_data;
    auto start = std::chrono::high_resolution_clock::now();
    sort_func(copy);
    auto end = std::chrono::high_resolution_clock::now();
    double time = std::chrono::duration<double, std::milli>(end - start).count();
    std::cout << time << std::endl;
    return time;
}

/**
 * @brief Линейный поиск всех вхождений по ключу
 * @param data Вектор записей
 * @param key Ключ поиска (ФИО жениха)
 * @return Вектор найденных записей
 * @details Сложность: O(n) — последовательный перебор всех элементов
 */
std::vector<ZagsRecord> linear_search(const std::vector<ZagsRecord>& data, const std::string& key);

/**
 * @brief Поиск в std::multimap по ключу
 * @param mmap Ассоциативный массив (мультиотображение)
 * @param key Ключ поиска
 * @return Вектор найденных записей
 * @details Сложность: O(log n + k) — бинарный поиск по сбалансированному дереву
 */
std::vector<ZagsRecord> multimap_search(const std::multimap<std::string, ZagsRecord>& mmap, const std::string& key);

/**
 * @brief Анализ коллизий хэш-функции для разных размеров таблицы
 * @param data Вектор записей
 * @param sizes Массив размеров данных для тестирования
 */
void analyzeHashCollisions(const std::vector<ZagsRecord>& data, const std::vector<int>& sizes);

#endif
