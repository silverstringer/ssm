#ifndef SSM_TRADELOG_H
#define SSM_TRADELOG_H

#include <string>
#include <string_view>
#include <vector>
#include <iostream>

#pragma pop(1)

/**
 * @brief файл журнала сделок
 * @future возможно отдельный модуль
 * @note for example;
 * @todo   type data exchange;
 * @note  usage in future  struct from future
 * @note скорее всего выгрузка из таблицы с id trader [ или  id  можно внести в общий конфиг]
 * @note in [.csv -> grid]
 */

class TradeLog final {
    using  string = std::string;
public:
    explicit TradeLog([[maybe_unused]] ushort uid_trader) {};
protected:
    struct Record { //order
    string timestampOpen; //dd-mm-yy [hh::mm::ss]
    string timestampClose; //dd-mm-yy
    string orderType;
    string typePosition; //[long, short,hold]
    string ticker;
    string Market; //idmarket
    double account_balance;
    double entry_price;
    long long  amount;
    string SL; //stop loss price
    string TP; //take profit price;
    string planned_risk_reward;
    string actual_exit_price;
    string trade_cost;//издержки сделки, стоимость сделки,  [taxes, transaction_fee]
    double percentage_account_risked; //risk_level;
    double closed_position_PL; // [profit - loss]
    double account_change_in_symbol;
    double account_change_in_percentage;
    string pattern_usage_enter; //  why i entered deal [pattern, formation , news ]
    string pattern_usage_exit; //  why i exit deal [line breakout month, by stop etc.]
    string status; //for show status order [ win, loss, neutral]
    string mistakes; //optional [fomo, overtrade]
    };
private:
    ushort uid_trader;
};


   /**
    * @brief PNL, profit and loss;
    *
    */

  class AnalyzeTradeLogSummary final  {
       using string_view = std::string_view;
      using TradeLogSummary = std::vector<TradeLog>;
    public:
        explicit AnalyzeTradeLogSummary(TradeLogSummary &log);
        void calculate();
  protected:
      ushort total_number_winning_trades;
      ushort total_number_losing_trades;
      int  trade_win_rate;
      int average_win_trade;
      int average_losing_trade;
      ushort largest_lose_trade;
      ushort largest_losing_trade;
      double total_PL; //profit_loss;
      double average_PL; //profit_loss;
      double ROI;//return on investment;
      double average_risk_per_trade;
      double average_reward_per_trade;
      double average_risk_reward;
      // usage on Month analysis, on Summary All;
  };


#pragma push(pop)

#endif //SSM_TRADELOG_H
