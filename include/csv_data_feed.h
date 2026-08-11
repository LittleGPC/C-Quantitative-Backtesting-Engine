#pragma once

#include "market_data.h"

#include <string>
#include <vector>

class CsvDataFeed
{
public:
    // Reads a CSV with a header containing at least: date,close.
    // Throws std::runtime_error for a missing file or malformed data.
    [[nodiscard]] std::vector<MarketBar> load(const std::string &filename) const;
};
