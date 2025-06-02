#include "mainwindow.h"

#include <QApplication>

#include "future.h"

Common::Logger::ConsoleLogger * Common::Logger::ConsoleLogger::p_instanse = 0;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow app;
    app.show();

    timer<ch::microseconds> timer;

    Strategy::dca res_test(10, 100, 90);
    std::vector<std::pair<int,int>> ranges = Strategy::make_sub_range(MAX_ASSETS);

    //for thread pool
//    ThreadPool pool(4);
//    pool.setJoin(true);
//    timer.start();
//    for(auto &item_range : ranges)
//    {
////        std::cout<<"Try add task:  " << item_range.first <<" to " << item_range.second <<"\n";
//        pool.addTask([&]() { Strategy::calculateDCA(res_test, item_range.first, item_range.second); });
////        pool.addTask( std::ref(Strategy::calculateDCA), std::ref(res_test) , std::ref(item_range.first), std::ref(item_range.second));
////        Strategy::calculateDCA(res_test, item_range.first, item_range.second);
//
//    }
//    timer.stop().print();


    auto strategy = Market::Strategy::Type::Scalping;
    auto algo = Market::TraderAlgorithmFactory::createAlgo(strategy);
    algo->init();

    algo = Market::TraderAlgorithmFactory::createAlgo(Market::Strategy::Type::DayTrade);
    algo->init();

    auto algo_3 = algo->clone();
    algo_3->init();



    return a.exec();
}
