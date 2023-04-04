#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow app;
    app.show();

    timer<ch::microseconds> timer;

    Strategy::dca res_test(10, 100, 90);

    std::vector<std::pair<int,int>> ranges = Strategy::make_sub_range(MAX_ASSETS);

    ThreadPool pool(4);
    pool.setJoin(true);
    timer.start();
    for(auto &item_range : ranges)
    {
//        std::cout<<"Try add task:  " << item_range.first <<" to " << item_range.second <<"\n";
        pool.addTask([&]() { Strategy::calculateDCA(res_test, item_range.first, item_range.second); });
//        pool.addTask( std::ref(Strategy::calculateDCA), std::ref(res_test) , std::ref(item_range.first), std::ref(item_range.second));
//        Strategy::calculateDCA(res_test, item_range.first, item_range.second);

    }
    timer.stop().print();

////    c++17 struct_binding
//    for(const auto [range_assets, range_price] : res_test.goal_range)
//    {
//        qDebug() <<"Range assets main" <<range_assets<<"Range price:" <<range_price;
//    }

//    qDebug() <<"Size ranges: " <<res_test.goal_range.size();



//    calculate SMA usage history data [json,  etc.] and period {example 20, 50, 100 }
//    std::vector<double> price_closing { 3.00, 5.10, 2.45, 1.33, 2.23, 3.22 };
//    int period = 5;//not usage
//    auto sma = [](const int period, std::vector<int> price_closing) {
//    return std::accumulate(price_closing.begin(), price_closing.end(), 0) / price_closing.size();
//    };

    //calculate esma

    return a.exec();
}
