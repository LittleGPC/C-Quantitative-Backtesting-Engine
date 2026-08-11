#include "trade_log.h"

#include <fstream>
#include <iomanip>
#include <stdexcept>

void TradeLog::add(TradeRecord trade)
{
    trades_.push_back(std::move(trade));
}

void TradeLog::write_csv(const std::string &filename) const
{
    std::ofstream file(filename);
    if (!file)
    {
        throw std::runtime_error("Cannot write trade log: " + filename);
    }

    file << "date,symbol,side,quantity,price,cash_after,position_after\n";
    file << std::fixed << std::setprecision(2);
    for (const auto &trade : trades_)
    {
        file << trade.date << ',' << trade.symbol << ',' << trade.side << ','
             << trade.quantity << ',' << trade.price << ',' << trade.cash_after
             << ',' << trade.position_after << '\n';
    }
}

std::size_t TradeLog::size() const
{
    return trades_.size();
}
