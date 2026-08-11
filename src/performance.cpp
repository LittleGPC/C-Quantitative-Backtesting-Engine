#include "performance.h"

#include <cmath>
#include <fstream>
#include <iomanip>
#include <numeric>
#include <stdexcept>
#include <vector>

void Performance::record(const std::string &date, double net_asset_value)
{
    if (net_asset_value < 0.0)
    {
        throw std::invalid_argument("Net asset value cannot be negative");
    }
    equity_curve_.push_back({date, net_asset_value});
}

void Performance::write_equity_curve_csv(const std::string &filename) const
{
    std::ofstream file(filename);
    if (!file)
    {
        throw std::runtime_error("Cannot write equity curve: " + filename);
    }

    file << "date,net_asset_value\n";
    file << std::fixed << std::setprecision(2);
    for (const auto &point : equity_curve_)
    {
        file << point.date << ',' << point.net_asset_value << '\n';
    }
}

double Performance::total_return() const
{
    if (equity_curve_.size() < 2 || equity_curve_.front().net_asset_value == 0.0)
    {
        return 0.0;
    }
    return equity_curve_.back().net_asset_value / equity_curve_.front().net_asset_value - 1.0;
}

double Performance::annualized_sharpe_ratio() const
{
    if (equity_curve_.size() < 3)
    {
        return 0.0;
    }

    std::vector<double> daily_returns;
    daily_returns.reserve(equity_curve_.size() - 1);
    for (std::size_t index = 1; index < equity_curve_.size(); ++index)
    {
        const double previous = equity_curve_[index - 1].net_asset_value;
        if (previous == 0.0)
        {
            return 0.0;
        }
        daily_returns.push_back(equity_curve_[index].net_asset_value / previous - 1.0);
    }

    const double mean = std::accumulate(daily_returns.begin(), daily_returns.end(), 0.0) /
                        static_cast<double>(daily_returns.size());
    double squared_difference_sum = 0.0;
    for (const double daily_return : daily_returns)
    {
        const double difference = daily_return - mean;
        squared_difference_sum += difference * difference;
    }

    const double standard_deviation = std::sqrt(
        squared_difference_sum / static_cast<double>(daily_returns.size() - 1));
    if (standard_deviation == 0.0)
    {
        return 0.0;
    }

    constexpr double trading_days_per_year = 252.0;
    return std::sqrt(trading_days_per_year) * mean / standard_deviation;
}

double Performance::max_drawdown() const
{
    if (equity_curve_.empty())
    {
        return 0.0;
    }

    double peak = equity_curve_.front().net_asset_value;
    double worst_drawdown = 0.0;
    for (const auto &point : equity_curve_)
    {
        peak = std::max(peak, point.net_asset_value);
        if (peak > 0.0)
        {
            worst_drawdown = std::min(
                worst_drawdown, point.net_asset_value / peak - 1.0);
        }
    }
    return worst_drawdown;
}

std::size_t Performance::observations() const
{
    return equity_curve_.size();
}
