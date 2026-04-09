/**
 * @file zags_record.h
 * @brief Структуры данных для записей ЗАГСа
 */

#ifndef ZAGS_RECORD_H
#define ZAGS_RECORD_H

#include <string>
#include <tuple>
#include <sstream>
#include <iomanip>
#include <vector>


/**
 * @struct Date
 * @brief Дата в формате dd.mm.yyyy
 */
struct Date {
    /** Поле дня, месяца, года */
    int day, month, year;
    
    /**
     * @brief Парсинг строки "dd.mm.yyyy"
     * @param s Входная строка даты
     */
    void parse(const std::string& s) {
        std::sscanf(s.c_str(), "%d.%d.%d", &day, &month, &year);
    }

    /**
     * @brief Форматированный вывод dd.mm.yyyy
     * @return Строка даты
     */
    std::string to_string() const {
        std::ostringstream oss;
        oss << std::setfill('0') << std::setw(2) << day << "."
            << std::setw(2) << month << "." << year;
        return oss.str();
    }

    bool operator==(const Date& other) const {
        return year == other.year && month == other.month && day == other.day;
    }

    bool operator!=(const Date& other) const {
        return !(*this == other);
    }
    
    bool operator<(const Date& other) const {
        return std::tie(year, month, day) < std::tie(other.year, other.month, other.day);
    }
    
    bool operator>(const Date& other) const { return other < *this; }
    bool operator<=(const Date& other) const { return !(other < *this); }
    bool operator>=(const Date& other) const { return !(*this < other); }
};


/**
 * @struct ZagsRecord
 * @brief Запись ЗАГСа: ФИО, даты рождения, дата свадьбы, номер ЗАГСа
 */
struct ZagsRecord {
    /** ФИО жениха */
    std::string groom_fio;
    /** Дата рождения жениха */
    Date groom_birth;
    /** ФИО невесты */
    std::string bride_fio;
    /** Дата рождения невесты */
    Date bride_birth;
    /** Дата бракосочетания */
    Date wedding_date;
    /** Номер ЗАГСа */
    int zags_number;
    
    /**
     * @brief Парсинг CSV строки
     * @param line Строка CSV без заголовка
     */
    void parse_csv(const std::string& line) {
        std::istringstream iss(line);
        std::string field;
        std::getline(iss, groom_fio, ',');
        std::getline(iss, field, ','); groom_birth.parse(field);
        std::getline(iss, bride_fio, ',');
        std::getline(iss, field, ','); bride_birth.parse(field);
        std::getline(iss, field, ','); wedding_date.parse(field);
        std::getline(iss, field, ','); zags_number = std::stoi(field);
    }
    
    /**
     * @brief CSV представление записи
     * @return Строка для записи в CSV файл
     */
    std::string to_csv() const {
        return groom_fio + "," + groom_birth.to_string() + "," +
               bride_fio + "," + bride_birth.to_string() + "," +
               wedding_date.to_string() + "," + std::to_string(zags_number) + "\n";
    }

    /**
     * @brief Сравнение ZagsRecord
     * @details Приоритет: zags_number -> wedding_date -> groom_fio
     * @param a Первая запись
     * @param b Вторая запись
     * @return true если a < b по заданному правилу
     */
    bool operator<(const ZagsRecord& other) const {
        if (zags_number != other.zags_number) return zags_number < other.zags_number;
        if (wedding_date != other.wedding_date) return other.wedding_date < wedding_date;
        return groom_fio < other.groom_fio;
    }
    
    bool operator>(const ZagsRecord& other) const { return other < *this; }
    bool operator<=(const ZagsRecord& other) const { return !(other < *this); }
    bool operator>=(const ZagsRecord& other) const { return !(*this < other); }
};

#endif