"""Download A-share daily prices and export them for the C++ backtester.

Example:
    python download_ashare.py 600519 --start 20200101 --end 20260809 --adjust qfq
"""

from __future__ import annotations

import argparse
from pathlib import Path

import akshare as ak


def parse_arguments() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Download A-share daily prices as a date,close CSV file."
    )
    parser.add_argument("symbol", help="Six-digit A-share symbol, e.g. 600519")
    parser.add_argument("--start", default="20200101", help="Start date: YYYYMMDD")
    parser.add_argument("--end", default="20260809", help="End date: YYYYMMDD")
    parser.add_argument(
        "--adjust",
        choices=("", "qfq", "hfq"),
        default="qfq",
        help="Price adjustment: qfq (default), hfq, or empty for unadjusted prices",
    )
    return parser.parse_args()


def main() -> None:
    args = parse_arguments()
    if not (args.symbol.isdigit() and len(args.symbol) == 6):
        raise SystemExit("symbol must be a six-digit A-share code, e.g. 600519")

    market_prefix = "sh" if args.symbol.startswith(("6", "9")) else "sz"

    data = ak.stock_zh_a_hist_tx(
        symbol=f"{market_prefix}{args.symbol}",
        start_date=args.start,
        end_date=args.end,
        adjust=args.adjust,
    )
    if data.empty:
        raise SystemExit("No data returned. Check the stock code and date range.")

    # 东方财富源使用中文列名，腾讯源使用英文列名；两种格式都兼容。
    date_column = "日期" if "日期" in data.columns else "date"
    close_column = "收盘" if "收盘" in data.columns else "close"
    if date_column not in data.columns or close_column not in data.columns:
        raise SystemExit(f"Unexpected data columns: {data.columns.tolist()}")

    result = data.loc[:, [date_column, close_column]].copy()
    result.columns = ["date", "close"]

    output = Path("data") / f"{args.symbol}.csv"
    output.parent.mkdir(exist_ok=True)
    result.to_csv(output, index=False, encoding="utf-8", lineterminator="\n")
    print(f"Saved {len(result)} daily bars to {output}")
    print(f"Range: {result.iloc[0]['date']} to {result.iloc[-1]['date']}")


if __name__ == "__main__":
    main()
