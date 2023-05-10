#ifndef SSM_FUTURE_H
#define SSM_FUTURE_H




#include <iostream>
#include <string>
#include <algorithm>
#include <math.h>
#include <numeric>
#include <vector>
#include <map>


template <typename  R, typename ...Ts>
auto match(const R& range, Ts ...ts)
{

    return (std::count(std::begin(range), std::end(range), ts) + ...);
}

template< typename T,typename ... Ts>
bool within(T min, T max, Ts ...ts)
{
    return ((min <= ts && ts<=max)&& ...);
}

template<typename T1, typename T2>
std::map<T2,T1> invert(const std::map<T1,T2> data)
{
    std::map<T2,T1> invert_map;
    for(const auto &item:data)
        invert_map[item.second] = item.first;
    return invert_map;
}



// calculate SMA usage history data [json,  etc.] and period {example 20, 50, 100 }
// usage price closing
float sma(std::vector<float> price_closing, int period = 0) {
    if(period > price_closing.size() || period < 0)
        return 0L;
    if(period != price_closing.size() && period != 0)
        price_closing.resize(period);
    return (float)std::accumulate(price_closing.begin(), price_closing.end(), 0) / price_closing.size();
}

float avg(std::vector<float> data)
{
    return std::accumulate(data.begin(), data.end(), 0) /data.size();
}

//Moving average
float MA(std::vector<float> data)
{
    return std::accumulate(data.begin(), data.end(), 0) /data.size();
}


// standart deviation for vector of prices
float std_dev(const std::vector<float> &data)
{
    float dev_mean = MA(data);
    float mean = 0L;
    for(const auto &item:data )
        mean += std::pow(dev_mean - item, 2);

    float dev = mean/data.size() ;
    return std::sqrt(dev);
}

//fill partS of vector
auto subset(int first, int last, std::vector<float> vec)
{
    std::vector<float>::const_iterator  begin = vec.begin()+ first;
    std::vector<float>::const_iterator end = vec.end() + last;

    std::vector<float> subset { begin, end};
    return subset;
}


// Defines the Bollinger bands
void bands(std::vector<float> prices, float standard_devs)
{
    auto top_band = MA(prices) + std_dev(prices) * standard_devs;
    auto bottom_band = MA(prices) - std_dev(prices) * standard_devs;

    std::vector<float>  top_band_vec, bottom_band_vec;
    //Fills in the vectors used for graphing.
    top_band_vec.push_back(top_band);
    bottom_band_vec.push_back(bottom_band);
}

bool buy_signal()
{
    bool signal { false };

    return signal;
}
bool sell_signal(){
    bool signal {false};
    return signal;
}


// weight for every indicators
class Indicator
{
    enum Type {
        BB,
        MA,
        RSI,
        ATR,
        MACD,
        DI,
    };

};

//common interface for all
class IStartegy{
    virtual void run() = 0;
};

class Strategy final  : public IStartegy
{
    enum Type {
        Scalping , // 0.5% - 1% with big volume
        DayTrade, //IntraDay
        BigVolume,
        Grid, //trader by grid price assets
        Swing,
        Investment
    };
    enum Trend { //global trend old timeframes [12h,1d,1w,1m] and local trend [4h,1h];
        Up,
        Down,
        Flat, //sideways
    };
public:
    Strategy() = default;
    void run() override {}
private:
    bool  signal { false };
};

//order ID  or hash ID?
//for our ticker own ID
class  Order {
    enum Type {
        Buy = 1,
        Sell,
        None //[hold] keep position
    };
    enum Attribute {
        Market = 1,
        StopLimit,
        StopLoss,
        TakeProfit,
        Trailing,
        OCO
    };
    //maybe type stop_loss, take_profit, market
    enum Status {
        Done,
        PartDone,
        Pendind,
        Cancelled,
        NoNe
    };
    struct Config {
        double currency_asset;
        double profit;
        double leverage;
        double asset;
        std::string_view asset_symbol; // ticker, symbol
        long timestamp; //time order to set
        long expiration_data;// data order is rancid
        long updateTimestamp;
    };
    long static ID; //orderID
    using OrderType = Type;
public:
    OrderType orderType;
    Status status {NoNe};
    std::string toOrderTypeString(OrderType type)
    {
        return type == Buy? std::string("buy"):std::string("sell");
    }
};

class OrderHistory {
public:
    OrderHistory () = default;
private:
    std::vector<Order> m_order;
};


// here must have wallet?
class BalanceInfo {
public:
    BalanceInfo () = default;
private:
    double currency_balance;
    std::string  asset; //ticker?;
};


class Transaction {
    enum TypeOperation {
        Refill = 1,
        Withdraw,
        Trade
    };
};

class Wallet {
    enum Symbol
    {
        USD,
        EUR,
        GBR,
        CNY,
        RUB
    };
public:
    std::vector<BalanceInfo> m_wallet;
    long commonDeposit; //basesum in baseSymbol;
    Symbol baseSymbol; // for convert
};

class Market {
    enum Status {
        Open = 1,
        Closed,
        Paused
    };
    struct Config {
        double bid {0.};
        double ask {0.};
        double volume {0.};
        double open {0.};
        double close {0.};
        double lastPrice {0.};
        double baseVolume {0.};
        double spread {0.};
        double priceTickSize {0.};
    };
public:
    Market() = default;

private:
    Status marketStatus;

};


/**
 * @brief   необходимо знать параметры сделки
 * [общий депозит, размер стопа для выхода из сделки
 * тип сделки - от тренда, тип стратегии и т.д.]
 *  \note  обычный риск 1 к 3, т.е при риски в 1% сделки закладывается 3% прибыли
 *  параметры
 *   необходимо учитывать налоги по сделке, чтобы вся прибыль не съедалась комиссиями биржи. налогами
 */
class RiskManagement {
public:
    RiskManagement() noexcept{}
    void run(){};
private:
    const double base_risk_level  { 3 } ; //in percentage  допустимый уровень риска 1 к 3 [1]   long  and 1 к 3 при  short
    double tax_broker; // налог  брокера
    double tax_market;// налог биржи
    double tax_profit;// налог на прибыль
    double common_tax;  // общий налог
    Order m_order;

};



//TODO: отдельные модули тестирования работы  алгоритмов на исторических данных [analyze, work with machine learning]
// модуль  налог по текущим транзакциям и "распечатка" в pdf
//optional module history  price data [data set]
/**
 * @brief тестирование алгоритма на исторических данных [fix/fast]
 *  \ результат список транзакций при работе определенного.алгоритма общий фин. результат
 */
class BackTest{  //Emulator
public:
    BackTest() = default;
    void setData() {}
    void getProviderData() {}
    void loadOrders(){}
    void loadBalance(){}
    void run(){}
private:

};









#endif //SSM_FUTURE_H
