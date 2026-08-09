#include "csv_data_feed.h"
#include "moving_average_strategy.h"
#include "performance.h"
#include "portfolio.h"
#include "trade_log.h"

#include <exception>
#include <iomanip>
#include <iostream>
#include <map>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: quant <market_data.csv>\n";
        return 1;
    }

    try {
        const CsvDataFeed data_feed;
        const auto bars = data_feed.load(argv[1]);

        std::cout << "Loaded " << bars.size() << " market bars\n";
        std::cout << "First: " << bars.front().date << ", " << std::fixed
                  << std::setprecision(2) << bars.front().close << '\n';
        std::cout << "Last:  " << bars.back().date << ", " << bars.back().close << '\n';

        const std::string symbol = "MARKET";
        Portfolio portfolio(10000000.0);
        // Buy with all available cash, rounded down to A-share lots of 100 shares.
        MovingAverageStrategy strategy(5, 20, 100);
        TradeLog trade_log;
        Performance performance;
        std::map<std::string, double> latest_prices;

        for (const auto& bar : bars) {
            latest_prices[symbol] = bar.close;
            strategy.on_bar(bar, portfolio, trade_log, symbol);
            performance.record(bar.date, portfolio.net_asset_value(latest_prices));
        }

        constexpr const char* trade_log_file = "trades.csv";
        constexpr const char* equity_curve_file = "equity_curve.csv";
        trade_log.write_csv(trade_log_file);
        performance.write_equity_curve_csv(equity_curve_file);

        std::cout << "Initial Capital: " << portfolio.initial_cash() << '\n';
        std::cout << "Cash: " << portfolio.cash() << '\n';
        std::cout << "Position: " << portfolio.position(symbol) << " shares\n";
        std::cout << "Portfolio NAV: " << portfolio.net_asset_value(latest_prices) << '\n';
        std::cout << "Total Return: " << performance.total_return() * 100.0 << "%\n";
        std::cout << "Sharpe Ratio: " << performance.annualized_sharpe_ratio() << '\n';
        std::cout << "Max Drawdown: " << performance.max_drawdown() * 100.0 << "%\n";
        std::cout << "Trades: " << trade_log.size() << " (saved to " << trade_log_file
                  << ")\n";
        std::cout << "Equity curve: " << performance.observations() << " points (saved to "
                  << equity_curve_file << ")\n";
    } catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }
}
