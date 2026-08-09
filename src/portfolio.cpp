#include "portfolio.h"

#include <stdexcept>

Portfolio::Portfolio(double initial_cash)
    : initial_cash_(initial_cash), cash_(initial_cash)
{
    if (initial_cash < 0.0)
    {
        throw std::invalid_argument("Initial cash cannot be negative");
    }
}

bool Portfolio::buy(const std::string &symbol, int quantity, double price)
{
    if (symbol.empty() || quantity <= 0 || price <= 0.0)
    {
        return false;
    }

    const double cost = quantity * price;
    if (cost > cash_)
    {
        return false;
    }

    cash_ -= cost;
    positions_[symbol] += quantity;
    return true;
}

bool Portfolio::sell(const std::string &symbol, int quantity, double price)
{
    if (symbol.empty() || quantity <= 0 || price <= 0.0)
    {
        return false;
    }

    const auto found = positions_.find(symbol);
    if (found == positions_.end() || found->second < quantity)
    {
        return false;
    }

    found->second -= quantity;
    cash_ += quantity * price;
    if (found->second == 0)
    {
        positions_.erase(found);
    }
    return true;
}

double Portfolio::initial_cash() const
{
    return initial_cash_;
}

double Portfolio::cash() const
{
    return cash_;
}

int Portfolio::position(const std::string &symbol) const
{
    const auto found = positions_.find(symbol);
    return found == positions_.end() ? 0 : found->second;
}

double Portfolio::net_asset_value(
    const std::map<std::string, double> &latest_prices) const
{
    double value = cash_;
    for (const auto &[symbol, quantity] : positions_)
    {
        const auto price = latest_prices.find(symbol);
        if (price == latest_prices.end())
        {
            throw std::invalid_argument("Missing latest price for " + symbol);
        }
        value += quantity * price->second;
    }
    return value;
}
