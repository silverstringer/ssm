#include "futurescalculator.h"



FuturesCalculator::FuturesCalculator() noexcept {
    m_instrumentDb = {
            {"SBERF", InstrumentParams(100, 0.01, 1.0)},   // Фьючерс на Сбербанк
            {"GAZPF", InstrumentParams(100, 0.01, 1.0)},   // Фьючерс на Газпром
            {"BRF4",  InstrumentParams(10,  0.1,  10.0)},  // Фьючерс на нефть Brent
            {"RTSF",  InstrumentParams(1,   10,   1.0)},   // Фьючерс на индекс РТС
            {"SIH4",  InstrumentParams(1000, 0.01, 1.0)},  // Фьючерс на серебро
            {"USDRUBF",  InstrumentParams(1000, 0.01, 10)},// Фьючерс на доллар однодневный
            {"USDRUBK",  InstrumentParams(1000, 1, 1)}     // Фьючерс на доллар  квартальный
    };
}

FuturesCalculator::InstrumentParams FuturesCalculator::getInstrumentParams(const std::string& ticker)
{

    auto it = m_instrumentDb.find(ticker);
    if (it != m_instrumentDb.end()) {
        printInstrumentParams(ticker);
        return it->second;
    }

    cout << "Tool not found. Please enter parameters manually:\n";
    int lot_size;
    double price_step, step_value;

    cout << "Size of lots: ";
    cin >> lot_size;
    cout << "Price step: ";
    cin >> price_step;
    cout << "Cost of price step: ";
    cin >> step_value;

    return InstrumentParams(lot_size, price_step, step_value);
}



double FuturesCalculator::calculatePnl(const Trade& trade) const {
    const double price_diff = trade.is_long
                              ? trade.close_price - trade.open_price
                              : trade.open_price - trade.close_price;

    const double steps = price_diff / trade.params.price_step;
    const double gross_profit = steps * trade.params.step_value * trade.quantity;
    const double total_fees = (trade.exchange_fee + trade.broker_fee) * 2 * trade.quantity;

    return gross_profit - total_fees;
}

FuturesCalculator::Trade FuturesCalculator::inputTradeData() {
    std::string ticker;
    cout << "\nВведите тикер фьючерса (например, SBERF): ";
    cin >> ticker;

    InstrumentParams params = getInstrumentParams(ticker);

    char direction;
    cout << "Тип позиции (L - лонг, S - шорт): ";
    cin >> direction;

    double open_price, close_price;
    cout << "Цена открытия: ";
    cin >> open_price;
    cout << "Цена закрытия: ";
    cin >> close_price;

    int quantity;
    cout << "Количество контрактов: ";
    cin >> quantity;

    double exchange_fee, broker_fee;
    cout << "Комиссия биржи за контракт: ";
    cin >> exchange_fee;
    cout << "Комиссия брокера за контракт: ";
    cin >> broker_fee;

    return Trade(ticker, toupper(direction) == 'L', open_price, close_price,
                 quantity, exchange_fee, broker_fee, params);
}



void FuturesCalculator::printInstrumentParams(const std::string& ticker) const {

    const auto& params = m_instrumentDb.at(ticker);
    cout << "Tool parameters:\n";
    cout << " - Size of lots: " << params.lot_size << "\n";
    cout << " - Step of price: " << params.price_step << "\n";
    cout << " - Cost per step: " << params.step_value << " ₽\n";
}


void FuturesCalculator::printTradeReport(const Trade& trade) const {


    const double profit = calculatePnl(trade);

    cout << "\n=== Transaction Report ===\n";
    cout << "Тикер: " << trade.ticker << "\n";
    cout << "Позиция: " << (trade.is_long ? "Лонг" : "Шорт") << "\n";
    cout << "Количество контрактов: " << trade.quantity << "\n";
    cout << std::fixed << std::setprecision(2);
    cout << "Цена открытия: " << trade.open_price << " ₽\n";
    cout << "Цена закрытия: " << trade.close_price << " ₽\n";
    cout << "Размер лота: " << trade.params.lot_size << "\n";
    cout << "Шаг цены: " << trade.params.price_step << "\n";
    cout << "Стоимость шага: " << trade.params.step_value << " ₽\n";
    cout << "Комиссия биржи: " << trade.exchange_fee << " ₽ за контракт\n";
    cout << "Комиссия брокера: " << trade.broker_fee << " ₽ за контракт\n";
    cout << "-------------------------\n";
    cout << "Результат: " << (profit >= 0 ? "Прибыль" : "Убыток")
         << ": " << std::abs(profit) << " ₽\n";
    cout << "=========================\n\n";
}

void FuturesCalculator::printSummaryReport() const {
    double total_profit = 0.0;
    cout << "\n=== Final report ===\n";

    for (const auto& trade : m_trades) {
        const double profit = calculatePnl(trade);
        total_profit += profit;
        cout << trade.ticker << ": " << (profit >= 0 ? "+" : "")
             << profit << " ₽\n";
    }

    cout << "-----------------------\n";
    cout << "Common result: " << (total_profit >= 0 ? "+" : "")
         << total_profit << " ₽\n";
}

void FuturesCalculator::run() {
    cout << " \nCalculate pnl by futures\n";
    cout << "----------------------------------------\n";

    while (true) {
        Trade trade = inputTradeData();
        m_trades.push_back(trade);
        printTradeReport(trade);

        if (!askToContinue()) break;
    }

    printSummaryReport();
}


bool FuturesCalculator::askToContinue() const {
    char choice;
    cout << "Add another deal? (Y/N): ";
    cin >> choice;
    return toupper(choice) == 'Y';
}


