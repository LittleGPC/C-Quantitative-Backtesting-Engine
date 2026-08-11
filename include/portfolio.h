#pragma once

#include <map>
#include <string>

class Portfolio
{
public:
    explicit Portfolio(double initial_cash);

    // Returns false when cash is insufficient or the order is invalid.
    bool buy(const std::string &symbol, int quantity, double price);

    // Returns false when the portfolio does not hold enough shares.
    bool sell(const std::string &symbol, int quantity, double price);

    [[nodiscard]] double initial_cash() const;
    [[nodiscard]] double cash() const;
    [[nodiscard]] int position(const std::string &symbol) const;

    // Calculates: cash + sum(position * latest price).
    [[nodiscard]] double net_asset_value(
        const std::map<std::string, double> &latest_prices) const;

private:
    double initial_cash_;
    double cash_;
    std::map<std::string, int> positions_;
};
