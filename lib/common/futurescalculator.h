#ifndef SSM_FUTURESCALCULATOR_H
#define SSM_FUTURESCALCULATOR_H

#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <iomanip>

using namespace std;
using std::cout;
using std::cin;

class FuturesCalculator final {
public:
    FuturesCalculator() noexcept;

    struct InstrumentParams {
        int lot_size;       // Размер лота (количество базового актива)
        double price_step;  // Минимальный шаг цены
        double step_value;  // Стоимость одного шага цены в рублях

        InstrumentParams(int lot, double step, double value)
                : lot_size(lot), price_step(step), step_value(value) {}
    };

    struct Trade {
        std::string ticker;
        bool is_long;       // false for short
        double open_price;
        double close_price;
        int quantity;
        double exchange_fee;
        double broker_fee;
        InstrumentParams params;

        Trade(const std::string &ticker, bool is_long, double open, double close,
              int qty, double exch_fee, double brk_fee, const InstrumentParams &ip)
                : ticker(ticker), is_long(is_long), open_price(open), close_price(close),
                  quantity(qty), exchange_fee(exch_fee), broker_fee(brk_fee),
                  params(ip) {}
    };

    void run();

private:
    Trade inputTradeData();
    InstrumentParams getInstrumentParams(const std::string &ticker);
    double calculatePnl(const Trade &trade) const;
    void printInstrumentParams(const std::string &ticker) const;
    void printTradeReport(const Trade &trade) const;
    void printSummaryReport() const;
    bool askToContinue() const;

    std::map<std::string, InstrumentParams> m_instrumentDb;
    std::vector<Trade> m_trades;

};

#endif //SSM_FUTURESCALCULATOR_H
