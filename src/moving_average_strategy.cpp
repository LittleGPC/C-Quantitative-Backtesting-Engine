#include "moving_average_strategy.h"

#include "portfolio.h"
#include "trade_log.h"

#include <numeric>
#include <stdexcept>

MovingAverageStrategy::MovingAverageStrategy(std::size_t short_window,
                                             std::size_t long_window,
                                             int lot_size)
    : short_window_(short_window), long_window_(long_window),
      lot_size_(lot_size)
{
    if (short_window == 0 || long_window == 0 || short_window >= long_window ||
        lot_size <= 0)
    {
        throw std::invalid_argument(
            "Require 0 < short window < long window and a positive lot size");
    }
}

void MovingAverageStrategy::on_bar(const MarketBar &bar, Portfolio &portfolio,
                                   TradeLog &trade_log, const std::string &symbol)
{
    closes_.push_back(bar.close);
    if (closes_.size() > long_window_)
    {
        closes_.pop_front();
    }
    if (closes_.size() < long_window_)
    {
        return;
    }

    const bool short_above_long = average_of_last(short_window_) >
                                  average_of_last(long_window_);
    if (!has_previous_signal_)
    {
        has_previous_signal_ = true;
        previous_short_above_long_ = short_above_long;
        return;
    }

    // Golden cross: enter a long position. Death cross: close the position.
    if (short_above_long && !previous_short_above_long_)
    {
        const int affordable_shares = static_cast<int>(portfolio.cash() / bar.close);
        const int quantity = affordable_shares / lot_size_ * lot_size_;
        if (quantity > 0 && portfolio.buy(symbol, quantity, bar.close))
        {
            trade_log.add({bar.date, symbol, "BUY", quantity, bar.close,
                           portfolio.cash(), portfolio.position(symbol)});
        }
    }
    else if (!short_above_long && previous_short_above_long_)
    {
        const int shares_to_sell = portfolio.position(symbol);
        if (shares_to_sell > 0 && portfolio.sell(symbol, shares_to_sell, bar.close))
        {
            trade_log.add({bar.date, symbol, "SELL", shares_to_sell, bar.close,
                           portfolio.cash(), portfolio.position(symbol)});
        }
    }
    previous_short_above_long_ = short_above_long;
}

double MovingAverageStrategy::average_of_last(std::size_t count) const
{
    const auto first = closes_.end() - static_cast<std::ptrdiff_t>(count);
    const double total = std::accumulate(first, closes_.end(), 0.0);
    return total / static_cast<double>(count);
}
