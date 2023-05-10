#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "dca.h"
#include "avgcoststocks.h"
#include "timer.h"
#include "threadpool.h"
#include "logger.h"
#include "grid.h"
#include "chart.h"
#include "csvreader.h"
#include  "csvmodel.h"

#include <QMainWindow>
#include <QDebug>

#define MAX_ASSETS 10000000 //max value of assets on calculate

namespace Ui {
class MainWindow;
}

/**
 * @brief
 * @todo загрузка с csv файла  и  отрисовка графика по выбранным колонкам
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    using dca = Strategy::dca;
 private slots:
     void detailDCA();
     void setResult(dca &res);
     void getDiffPercentDetails();
signals:
    void totalSumFirstOrder();
    void calcDone();
protected:
    dca result;
private:
    void setDefaultValue();
    void calculate(double assets, double price, double goal_price);
    void calculate(dca &res, int max_range = MAX_ASSETS);
private:
    bool isMoonTheme { false };
    std::map<double, double> resultDiffPercent;
    AvgCostStocks * m_acs { nullptr };
    Ui::MainWindow * ui;

};

 template<typename T1, typename T2>
 std::map<T2,T1> invert(const std::map<T1,T2> data)
 {
  std::map<T2,T1> invert_map;
  for(const auto &item:data)
      invert_map[item.second] = item.first;
  return invert_map;
 }


#endif // MAINWINDOW_H
