#include "csv_data_feed.h"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>

namespace {

std::string trim(std::string text) {
    const auto first = text.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) {
        return {};
    }
    const auto last = text.find_last_not_of(" \t\r\n");
    return text.substr(first, last - first + 1);
}

}  // namespace

std::vector<MarketBar> CsvDataFeed::load(const std::string& filename) const {
    std::ifstream file(filename);
    if (!file) {
        throw std::runtime_error("Cannot open CSV file: " + filename);
    }

    std::string line;
    if (!std::getline(file, line)) {
        throw std::runtime_error("CSV file is empty: " + filename);
    }

    std::vector<MarketBar> bars;
    std::size_t line_number = 1;
    while (std::getline(file, line)) {
        ++line_number;
        if (trim(line).empty()) {
            continue;
        }

        std::stringstream row(line);
        std::string date;
        std::string close_text;
        if (!std::getline(row, date, ',') || !std::getline(row, close_text, ',')) {
            throw std::runtime_error("Malformed CSV at line " + std::to_string(line_number));
        }

        date = trim(date);
        close_text = trim(close_text);
        if (date.empty() || close_text.empty()) {
            throw std::runtime_error("Missing date or close price at line " + std::to_string(line_number));
        }

        try {
            std::size_t parsed_characters = 0;
            const double close = std::stod(close_text, &parsed_characters);
            if (parsed_characters != close_text.size()) {
                throw std::invalid_argument("trailing characters");
            }
            bars.push_back({date, close});
        } catch (const std::exception&) {
            throw std::runtime_error("Invalid close price at line " + std::to_string(line_number));
        }
    }

    if (bars.empty()) {
        throw std::runtime_error("CSV file contains no market data: " + filename);
    }
    return bars;
}
