#ifndef SSM_TRADELOG_H
#define SSM_TRADELOG_H

#include <string>
#include <string_view>
#include <iostream>

/**
 * @brief файл журнала сделок
 * @note  usage in future  struct from future
 *  //скорее всего выгрузка из таблицы с id trader [ или  id  можно внести в общий конфиг]
 */

class TradeLog final {
    using  string = std::string;
public:
    explicit TradeLog(ushort uid_trader) {};
protected:
    struct Config {
    string timestampOpen; //dd-mm-yy
    string timestampClose; //dd-mm-yy
    string orderType;
    string typePosition; //[long, short]
    string ticker;
    string account_balance;
    string entry_price;
    long long  amount;
    string SL; //stop loss price
    string TP; //take profit price;
    string planned_risk_reward;
    string actual_exit_price;
    string trade_cost;//издержки сделки, стоимость сделки,  [taxes, transaction_fee]
    double percentage_account_risked; //risk_level;
    string closed_position_PL; // [profit - loss]
    string account_change_in_percentage;
    string pattern_usage; //  why i entered deal
    };
private:
    ushort uid_trader;
};

  //PNL, profit and loss;

  class AnalyzeTradeLogSummary final   {
       using string_view = std::string_view;
    public:
        explicit AnalyzeTradeLogSummary();
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

#endif //SSM_TRADELOG_H
