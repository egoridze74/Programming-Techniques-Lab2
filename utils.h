/**
 * @file utils.h
 * @brief Вспомогательные функции: ввод/вывод, замеры времени
 */

#ifndef UTILS_H
#define UTILS_H

#include "zags_record.h"
#include <chrono>
#include <vector>


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
    double time = std::chrono::duration<double, std::milli>(end - start).count()
    std::cout << time;
    return time;
}

#endif
