#pragma once

#include <string>

// A single daily bar read from a CSV file.
struct MarketBar
{
    std::string date;
    double close{};
};
