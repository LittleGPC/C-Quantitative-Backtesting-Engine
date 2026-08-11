#pragma once

#include <cstddef>
#include <string>
#include <vector>

struct EquityPoint
{
    std::string date;
    double net_asset_value{};
};

class Performance
{
public:
    void record(const std::string &date, double net_asset_value);
    void write_equity_curve_csv(const std::string &filename) const;

    [[nodiscard]] double total_return() const;
    [[nodiscard]] double annualized_sharpe_ratio() const;
    [[nodiscard]] double max_drawdown() const;
    [[nodiscard]] std::size_t observations() const;

private:
    std::vector<EquityPoint> equity_curve_;
};
