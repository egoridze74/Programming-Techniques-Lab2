#include "utils.h"
#include <fstream>
#include <iostream>
#include <sstream>


std::vector<ZagsRecord> read_csv(const std::string& filename) {
    std::vector<ZagsRecord> data;
    data.reserve(100000);
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cout << "File not found: " << filename << std::endl;
        return data;
    }
    
    std::string line;
    std::getline(file, line);
    while (std::getline(file, line)) {
        ZagsRecord rec;
        rec.parse_csv(line);
        data.push_back(rec);
    }
    std::cout << "Loaded " << data.size() << " records from " << filename << std::endl;
    return data;
}


void write_csv(const std::vector<ZagsRecord>& data, const std::string& filename) {
    std::ofstream file(filename);
    file << "groom_fio,groom_birth,bride_fio,bride_birth,wedding_date,zags_number\n";
    for (const auto& rec : data) {
        file << rec.to_csv();
    }
    std::cout << "Saved to " << filename << std::endl;
}
