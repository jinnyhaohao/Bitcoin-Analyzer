# Bitcoin Volatility Analyzer & TFT Trading Engine

An applied machine learning and quantitative research framework designed to forecast Bitcoin price volatility and simulate trading execution. 

This project leverages a **Temporal Fusion Transformer (TFT)** to model non-linear price dynamics and time-series indicators, feeding the predictions into a custom, vectorized backtesting engine that simulates real-world trading costs.

## 🚀 Key Features

* **Deep Learning Time-Series Forecasting:** Utilizes `pytorch-forecasting` to train a Temporal Fusion Transformer, capturing complex temporal dependencies and incorporating exogenous variables (volume, momentum).
* **Advanced Feature Engineering:** Vectorized calculation of market microstructure and momentum indicators, including MACD (with slope/curl detection), Multi-Timeframe RSI, KDJ, and Exponential Moving Averages.
* **Vectorized Backtesting Engine:** A custom Pandas/NumPy execution environment that simulates portfolio PnL, factoring in taker fees (`0.015%`), slippage, and dynamic position sizing.
* **Hybrid Strategy Logic:** Combines neural network probability outputs with strict technical guardrails to trigger entries and exits.
* **Live Exchange Integration:** Includes scripts utilizing the `ccxt` library for live market data ingestion (e.g., Pepe/ETH lead-lag analysis).

## 🛠️ Tech Stack

* **Modeling:** PyTorch, PyTorch Lightning, PyTorch Forecasting
* **Data Processing:** Pandas, NumPy, Kaggle API
* **Market Data:** CCXT (Live), Kaggle (Historical Minutely Data)
* **Visualization:** Matplotlib

## 📂 Project Architecture

1. **Data Ingestion (`Cell 0-2`):** * Automates the retrieval of high-resolution (minutely) historical BTC/USD data and resamples it into hourly/daily periods to reduce noise while maintaining structural integrity.
2. **Feature Engineering (`Cell 3-6`):** * Calculates continuous momentum features and logical regime flags (e.g., Bullish/Bearish Trend, MACD Curls) to feed as static and time-varying inputs to the transformer.
3. **Model Training (`Cell 9-11`):** * Defines the TFT architecture, configuring lookback windows, hidden sizes, and attention heads. Optimized using Quantile Loss to understand prediction confidence bounds.
4. **Inference & Execution (`Cell 14-20`):** * Runs the trained model over out-of-sample data. Generates a simulated equity curve comparing the hybrid TFT strategy against a standard Buy & Hold baseline.

## ⚙️ Installation & Usage

1. Clone the repository:
   ```bash
   git clone [https://github.com/yourusername/btc-volatility-analyzer.git](https://github.com/yourusername/btc-volatility-analyzer.git)# Bitcoin-Analyzer