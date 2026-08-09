# AstraQuant

AstraQuant is a modular C++17 quantitative backtesting engine for daily A-share market data. It provides a complete beginner-friendly backtesting loop:

```text
CSV market data -> moving-average strategy -> portfolio -> trade log -> performance report
```

## Features

- Reads daily market data from CSV files (`date,close`)
- Implements a 5/20-day moving-average crossover strategy
- Tracks cash, multi-symbol positions, and net asset value (NAV)
- Buys the maximum affordable A-share quantity in lots of 100 shares
- Records every executed buy and sell transaction to CSV
- Calculates total return, annualized Sharpe ratio, and maximum drawdown
- Exports a daily equity curve for charting in Excel or Python
- Downloads and normalizes A-share historical data using Python and AKShare

## Project Structure

```text
AstraQuant
├── CMakeLists.txt
├── download_ashare.py       # A-share data downloader
├── include
│   ├── csv_data_feed.h
│   ├── market_data.h
│   ├── moving_average_strategy.h
│   ├── performance.h
│   ├── portfolio.h
│   ├── strategy.h
│   └── trade_log.h
└── src
    ├── csv_data_feed.cpp
    ├── main.cpp
    ├── moving_average_strategy.cpp
    ├── performance.cpp
    ├── portfolio.cpp
    └── trade_log.cpp
```

## Prerequisites

- A C++17 compiler (MinGW `g++` is supported)
- Python 3.9+ (only needed to download A-share data)

Install the Python dependencies:

```powershell
pip install akshare pandas
```

## Download A-share Data

The downloader produces CSV files compatible with AstraQuant:

```powershell
python .\download_ashare.py 600519 --start 20200101 --end 20260809 --adjust qfq
```

Arguments:

| Argument | Description | Example |
|---|---|---|
| `symbol` | Six-digit A-share code | `600519` |
| `--start` | Start date in `YYYYMMDD` | `20200101` |
| `--end` | End date in `YYYYMMDD` | `20260809` |
| `--adjust` | `qfq` (forward-adjusted), `hfq`, or empty | `qfq` |

The command creates `data/600519.csv` in this format:

```csv
date,close
2020-01-02,870.74
2020-01-03,861.83
```

If your network blocks a public data provider, try switching networks or configuring your proxy correctly before running the downloader.

## Build

### Build with g++

```powershell
g++ -std=c++17 -Wall -Wextra -Wpedantic -Iinclude `
  src\main.cpp `
  src\csv_data_feed.cpp `
  src\portfolio.cpp `
  src\moving_average_strategy.cpp `
  src\trade_log.cpp `
  src\performance.cpp `
  -o build\quant.exe
```

### Build with CMake

```powershell
cmake -S . -B build
cmake --build build
```

## Run a Backtest

```powershell
.\build\quant.exe .\data\600519.csv
```

Example output:

```text
Loaded 1599 market bars
Initial Capital: 10000000.00
Cash: 17946.00
Position: 10600 shares
Portfolio NAV: 13895678.00
Total Return: 38.96%
Sharpe Ratio: 0.35
Max Drawdown: -46.90%
Trades: 97 (saved to trades.csv)
Equity curve: 1599 points (saved to equity_curve.csv)
```

The values above are one historical run with a particular date range and configuration; they are not a claim about future investment performance.

## Outputs

### `trades.csv`

Records successful trades only:

```csv
date,symbol,side,quantity,price,cash_after,position_after
2020-02-13,MARKET,BUY,12000,831.74,189120.00,12000
```

### `equity_curve.csv`

Records the account value after each trading day:

```csv
date,net_asset_value
2020-01-02,10000000.00
2020-01-03,10000000.00
```

## Strategy Logic

The default strategy uses a 5-day and 20-day simple moving average (SMA):

- **Golden cross**: the short SMA crosses above the long SMA; buy the maximum affordable number of shares, rounded down to 100-share A-share lots.
- **Death cross**: the short SMA crosses below the long SMA; sell the entire position.

## Performance Metrics

- **Total Return**: `(final NAV / initial NAV) - 1`
- **Annualized Sharpe Ratio**: mean daily return divided by sample daily-return volatility, annualized using 252 trading days and a zero risk-free rate
- **Maximum Drawdown**: the worst percentage decline from any prior NAV peak

## Current Limitations

This is an educational v1 backtester. It does not yet model:

- Commission, stamp duty, and slippage
- Intraday prices or limit-up / limit-down restrictions
- Delayed order execution
- Multiple strategies or parameter configuration files
- Formal unit tests and benchmarks

## Roadmap

- Add transaction costs and slippage
- Add configurable strategy parameters through JSON
- Add additional strategies (momentum and mean reversion)
- Add unit tests and performance benchmarks
- Add charts and an HTML report

## Disclaimer

This project is for education and research only. It is not investment advice.
