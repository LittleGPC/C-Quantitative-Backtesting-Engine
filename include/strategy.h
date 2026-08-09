#pragma once

#include "market_data.h"

#include <string>

class Portfolio;
class TradeLog;

// A strategy receives each market bar and may trade through the portfolio.
class Strategy {
public:
    virtual ~Strategy() = default;

    virtual void on_bar(const MarketBar& bar, Portfolio& portfolio,
                        TradeLog& trade_log, const std::string& symbol) = 0;
};
