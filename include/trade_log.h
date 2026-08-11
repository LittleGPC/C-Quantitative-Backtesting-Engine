#pragma once

#include <cstddef>
#include <string>
#include <vector>

struct TradeRecord
{
    std::string date;
    std::string symbol;
    std::string side;
    int quantity{};
    double price{};
    double cash_after{};
    int position_after{};
};

class TradeLog
{
public:
    void add(TradeRecord trade);
    void write_csv(const std::string &filename) const;

    [[nodiscard]] std::size_t size() const;

private:
    std::vector<TradeRecord> trades_;
};
