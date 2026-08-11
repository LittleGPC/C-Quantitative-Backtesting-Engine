#pragma once

#include "strategy.h"

#include <deque>

class MovingAverageStrategy final : public Strategy
{
public:
    MovingAverageStrategy(std::size_t short_window, std::size_t long_window,
                          int lot_size);

    void on_bar(const MarketBar &bar, Portfolio &portfolio, TradeLog &trade_log,
                const std::string &symbol) override;

private:
    [[nodiscard]] double average_of_last(std::size_t count) const;

    std::size_t short_window_;
    std::size_t long_window_;
    int lot_size_;
    std::deque<double> closes_;
    bool has_previous_signal_ = false;
    bool previous_short_above_long_ = false;
};
