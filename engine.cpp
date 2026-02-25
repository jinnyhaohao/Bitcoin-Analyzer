#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <iostream>
#include <vector>
#include <string>
#include <cmath>

namespace py = pybind11;

// 1. Core Data Structures
struct Tick {
    long long timestamp_ms;
    double bid_price;
    double ask_price;
    double bid_size;
    double ask_size;
};

enum class Side { BUY, SELL };

// 2. The Execution Engine
class BacktestEngine {
private:
    double cash;
    double position_qty;
    double realized_pnl;
    double fee_rate;
    int latency_ms; // Simulated network latency

public:
    BacktestEngine(double initial_cash, double fee) 
        : cash(initial_cash), position_qty(0.0), realized_pnl(0.0), fee_rate(fee), latency_ms(5) {}

    // Core Event Loop: Process one tick at a time
    void on_tick(const Tick& tick, int model_signal) {
        // model_signal: 1 (Buy), -1 (Sell), 0 (Hold) coming from PyTorch
        
        if (model_signal == 1 && cash > 0) {
            // Assume we cross the spread and pay the Ask price + Slippage
            double execution_price = tick.ask_price * 1.0001; // 1 bps slippage
            double qty_to_buy = (cash * 0.99) / execution_price; // Keep 1% reserve
            execute_trade(Side::BUY, qty_to_buy, execution_price);
        } 
        else if (model_signal == -1 && position_qty > 0) {
            // Sell at the Bid price - Slippage
            double execution_price = tick.bid_price * 0.9999; 
            execute_trade(Side::SELL, position_qty, execution_price);
        }
    }

    void execute_trade(Side side, double qty, double price) {
        double notional = qty * price;
        double fee = notional * fee_rate;

        if (side == Side::BUY) {
            cash -= (notional + fee);
            position_qty += qty;
        } else {
            cash += (notional - fee);
            realized_pnl += (price - (cash/position_qty)) * qty; // Simplified PnL
            position_qty -= qty;
        }
    }

    // High-performance batch processing for Python
    std::vector<double> run_simulation(const std::vector<Tick>& market_data, const std::vector<int>& predictions) {
        std::vector<double> equity_curve;
        equity_curve.reserve(market_data.size());

        for (size_t i = 0; i < market_data.size(); ++i) {
            on_tick(market_data[i], predictions[i]);
            
            // Calculate Mark-to-Market (MTM) Equity
            double current_mtm = cash + (position_qty * market_data[i].bid_price);
            equity_curve.push_back(current_mtm);
        }
        return equity_curve;
    }

    double get_final_equity() const { return cash; } // Assuming flat position
};

// 3. Pybind11 Python Bindings
PYBIND11_MODULE(quant_engine, m) {
    py::class_<Tick>(m, "Tick")
        .def(py::init<long long, double, double, double, double>());

    py::class_<BacktestEngine>(m, "BacktestEngine")
        .def(py::init<double, double>())
        .def("run_simulation", &BacktestEngine::run_simulation)
        .def("get_final_equity", &BacktestEngine::get_final_equity);
}