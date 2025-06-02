#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDialog>
#include <QTimer>
#include <QTableView>
#include <QHeaderView>
#include <QStandardItemModel>
#include <QHBoxLayout>
#include <QClipboard>
#include <QDateTime>
#include <QShortcut>
#include <QMap>

#include <QFuture>
#include <QtConcurrent/qtconcurrentrun.h>
#include <QFutureWatcher>


#include <iostream>


MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow) {
    ui->setupUi(this);

    setDefaultValue();
    m_acs = new AvgCostStocks();

    setHotKey();

    connect(ui->spnFirstAssest, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [this]() {
        emit totalSumFirstOrder();
    });

    connect(ui->btnDiffPercent, &QPushButton::clicked, [this]() {
        calculateDiffPercentage();
    });

    connect(ui->spnFirstPrice, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [this]() {
        emit totalSumFirstOrder();
    });

    connect(ui->spnDeposit, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [this]() {

        std::unique_ptr<Graph>  graph = std::make_unique<Graph>();
        graph->setType(Graph::TypeChart::PieChart);
        graph->setTitleGraph("Stock portfolio", "Month", "Depo");

        std::map<QString, int>  test_data = {{"EUR", 3}, {"BLR", 6},
                                             {"USD", 5},  {"TRY", 12},
                                             {"GBR", 2}, {"CNY", 7}};

//        graph->buildPieChart(test_data);
    });

    connect(ui->btnAvgCostStock, &QPushButton::clicked, [this]() {
        m_acs->show();
    });


    connect(ui->btnDetail, &QPushButton::clicked, [this]() {
        detailDCA();
    });

    connect(ui->btnBackgrColor, &QPushButton::clicked, [this]() {
        setBackgroundMainWindow();
    });


    connect(ui->spnGoalPrice, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [this]() {

        if (ui->spnFirstPrice->value() < ui->spnGoalPrice->value())
            ui->spnGoalPrice->setValue(ui->spnFirstPrice->value());
    });

    connect(ui->btnFuturesCalc, &QPushButton::clicked, []() {

        try {
            QtConcurrent::run([]() {
                FuturesCalculator calc;
                calc.run();
            });
        }
        catch(std::exception& e){
            qDebug() <<e.what();
        }
    });

    connect(ui->btnInvestmentCalc, &QPushButton::clicked, []() {

        try {
            QtConcurrent::run([]() {

                int currentLots;
                double currentAvgPrice, targetAvgPrice, budget;

                std::cout << "\n Введите текущее количество лотов: ";
                std::cin >> currentLots;
                std::cout << "Введите текущую среднюю цену (руб.): ";
                std::cin >> currentAvgPrice;
                std::cout << "Введите целевую среднюю цену (руб.): ";
                std::cin >> targetAvgPrice;
                std::cout << "Введите бюджет (руб., или 0 для неограниченного): ";
                std::cin >> budget;
                budget = (budget <= 0) ? std::numeric_limits<double>::max() : budget;

                InvestmentCalculator::compareStrategies(currentLots, currentAvgPrice, targetAvgPrice, budget);

            });
        }
        catch(std::exception& e){
            qDebug() <<e.what();
        }
    });

    connect(ui->btnCalculateDCA, &QPushButton::clicked, [this]() {

        QFuture <void > future = QtConcurrent::run(this, &MainWindow::calculateDCA);
        QFutureWatcher <void>  * watcher_calculate =  new QFutureWatcher <void >(this);
        connect(watcher_calculate, &QFutureWatcher<void>::finished,this,[&](){
            emit calcDone();
            ui->btnCalculateDCA->setText("calculate");

//        ui->spnGoalPrice->setStyleSheet("QDoubleSpinBox {color : red;}");
//
//        ui->spnRangeAsset->setStyleSheet("QDoubleSpinBox {color : green; }");
//        ui->spnRangePrice->setStyleSheet("QDoubleSpinBox {color : green; }");
//
//        ui->lblRangeSum->setStyleSheet("QLabel {color : green; }");
        });
        watcher_calculate->setFuture(future);
        ui->btnCalculateDCA->setText("calculate...");


//        calculateDCA();
    });
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::detailDCA() {

    QDialog *dlg = new QDialog(this);

    dlg->setWindowTitle("Strategy DCA");

    QHBoxLayout *HLayout = new QHBoxLayout(dlg);

    // Configure the table view
    auto tv = new QTableView(dlg);
    auto model = new QStandardItemModel();
    tv->setModel(model);
    tv->setEditTriggers(QAbstractItemView::NoEditTriggers);

    tv->resizeColumnsToContents();
    tv->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

// this->setCentralWidget(tv);

    // Configure column titles
    std::vector<QString> headerDCA = { "Amount", "Price", "Sum",  " Down price , %", "Percentage from m_depo"};
    for( size_t  items = 0; items < headerDCA.size(); items++)
    model->setHorizontalHeaderItem(items, new QStandardItem(headerDCA.at(items)));

    // Add rows to the model
    QList<QStandardItem *> rowData;
    for (const auto &item:resultDCA.goal_range) {
        rowData.clear();
        auto partofdepo = (item.second * item.first * 100) / m_depo;
        auto dd =  new QStandardItem(QString("%1").arg(partofdepo));
        rowData << new QStandardItem(QString("%1").arg(item.first));
        rowData << new QStandardItem(QString("%1").arg(item.second));
        rowData << new QStandardItem(QString("%1").arg(item.second * item.first));
        rowData << new QStandardItem(QString("%1").arg((item.second * 100 / resultDCA.price) - 100));
        rowData << dd;
        (partofdepo > 100) ? (dd->setForeground(QColor(Qt::red))):( dd->setForeground(QColor(Qt::green)));

        model->appendRow(rowData);
    }

    HLayout->addWidget(tv);
    dlg->setLayout(HLayout);
    dlg->show();

    //position child widget to right from parent window
    QRect parentRect = this->geometry();
    dlg->setGeometry(parentRect);
    dlg->move(QPoint(parentRect.topRight().x() + 10, parentRect.topRight().y()));



//selected model data to  clipboard

    if (tv->selectionModel()->hasSelection()) {
//     QAbstractItemModel * model1 = tv->model();
        QItemSelectionModel *selection = tv->selectionModel();
        QModelIndexList indexes = selection->selectedIndexes();

        QString selected_text;
        // You need a pair of indexes to find the row changes
        QModelIndex previous = indexes.first();
        indexes.removeFirst();
                foreach(const QModelIndex &current, indexes) {
                QVariant data = model->data(current);
                QString text = data.toString();
                // At this point `text` contains the text in one cell
                selected_text.append(text);
                // If you are at the start of the row the row number of the previous index
                // isn't the same.  Text is followed by a row separator, which is a newline.
                if (current.row() != previous.row()) {
                    selected_text.append('\n');
                }
                    // Otherwise it's the same row, so append a column separator, which is a tab.
                else {
                    selected_text.append('\t');
                }
                previous = current;
            }
        QApplication::clipboard()->setText(selected_text);
    }


}

void MainWindow::setResult(dca &res) {
    ui->lblTotalFirstSum->setText(QString::number(res.assets * res.price));

    for (const auto value:res.goal_range) {
        auto range_assets = value.first;
        auto range_price = value.second;

        ui->spnRangeAsset->setValue(range_assets);
        ui->spnRangePrice->setValue(range_price);


        ui->spnTotalAsset->setValue(range_assets + res.assets);
        ui->lblTotalSum->setText(QString::number(range_price * range_assets + res.assets * res.price));

        ui->spnGoalPrice->setStyleSheet("QDoubleSpinBox {color : red;}");

        ui->spnRangeAsset->setStyleSheet("QDoubleSpinBox {color : green; }");
        ui->spnRangePrice->setStyleSheet("QDoubleSpinBox {color : green; }");

        ui->lblRangeSum->setText(QString::number(range_assets * range_price));
        ui->lblRangeSum->setStyleSheet("QLabel {color : green; }");
    }
}


void MainWindow::setDefaultValue() {
    //Set Range [Limit Value]
    ui->spnFirstAssest->setRange(0, 1600000);
    ui->spnFirstPrice->setRange(0, 1600000);

    ui->spnFirstPrice->setDecimals(4);

    ui->spnDeposit->setRange(0,10000000);
    ui->spnDeposit->setDecimals(4);


    ui->spnTotalAsset->setRange(0, 1600000);
    ui->spnGoalPrice->setRange(0, 1600000);

    ui->spnRangeAsset->setRange(0, 16000000);
    ui->spnRangePrice->setRange(0, 1600000);
    ui->spnGoalPrice->setDecimals(4);

    //UX diff percentage
    ui->spnDiffPercentInMonth->setRange(0, 999);
    ui->spnDepoDiffPerc->setRange(0, 10000000);

    ui->spnDiffPercentDuration->setRange(0, 10000);
    ui->spnDeposit->setRange(0, 100000000);

    ui->spnFirstAssest->setValue(10);
    ui->spnFirstPrice->setValue(100);
    ui->spnGoalPrice->setValue(90);
    ui->spnDeposit->setValue(800);
    ui->spnRangeAsset->setReadOnly(true);
    ui->spnRangePrice->setReadOnly(true);
    ui->spnDepoDiffPerc->setValue(100); //default value 100
    ui->spnDiffPercentDuration->setValue(12); //default value month
    ui->spnDiffPercentInMonth->setValue(15);

}

[[maybe_unused]]
void MainWindow::calculate(double assets, double price, double goal_price) {
    auto first_total_sum = assets * price;

    qDebug() << "Total first sum" << first_total_sum;
    qDebug() << "Goal price" << goal_price;

    for (auto range_assets = 0; range_assets < 1000000; range_assets++) {
        for (auto range_price = 0; range_price < price; range_price++) {
//         auto range_sum = range_assets * range_price;

//         auto total_sum = range_sum + first_total_sum;
        }
    }

}

void MainWindow::calculate(dca &res, int max_range) {

    auto first_total_sum = res.assets * res.price;
    res.goal_range.clear();

    for (auto range_assets = 0; range_assets < max_range; range_assets++) {
        for (auto range_price = 0; range_price <= res.goal_price; range_price++) {
            auto range_sum = range_assets * range_price;

            auto total_sum = range_sum + first_total_sum;
            auto find_down_price = total_sum / (res.assets + range_assets);

            auto diff_price = res.goal_price - find_down_price;

            if (diff_price == 0) {
                res.goal_range[range_assets] = range_price;
            }
        }
    }

    //set GUI
    setGuiCalculateDca(res);

//   emit calcDone();
}

void MainWindow::getDiffPercentDetails() {

    QDialog *dlg = new QDialog(this);
    dlg->setWindowTitle("Strategy Difference Percentage");
    QPushButton *btnSaveCSV = new QPushButton(tr("Save CSV"));
    QPushButton *btnGetGraph = new QPushButton(tr("Graph"));
//    btnSaveCSV->setMinimumSize(100,30);
    QVBoxLayout *VLayout = new QVBoxLayout(dlg);
    QHBoxLayout  * HLayout = new QHBoxLayout();
    HLayout->addWidget(btnSaveCSV);
    HLayout->addWidget(btnGetGraph);


    // Configure the table view
    auto tv = new QTableView(dlg);
    auto model = new QStandardItemModel();
    tv->setModel(model);
    tv->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tv->resizeColumnsToContents();
    tv->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

//    std::vector<QString> headerSDF = {"Month","Depo", "Profit", "Percent", "Percent on Depo"};
//    for(size_t item = 0; item < headerSDF.size(); item++ )
//        model->setHorizontalHeaderItem(item, new QStandardItem(headerSDF.at(item)));

    std::map<int , QString> header {{0,"Month"},{1,"Depo"}, {2,"Profit"},{3,"Percent"}, { 4, "Percent on Depo"}};
    for(const auto &[item, head] : header)
        model->setHorizontalHeaderItem(item, new QStandardItem(head));

    QList<QStandardItem *> rowData;

    auto firstDepo = resultDiffPercent.begin()->second;
    std::vector<std::vector <std::string>> data{};
    data.clear();

    for(auto it = resultDiffPercent.begin(); it != resultDiffPercent.end(); ++it) {
        rowData.clear();
        std::vector<std::string> subvector;
        rowData << new QStandardItem(QString("%1").arg((*it).first));
        rowData << new QStandardItem(QString("%1").arg(((*it).second)));
        subvector.push_back(QString("%1").arg((*it).first).toStdString());
        subvector.push_back(QString("%1").arg(((*it).second)).toStdString());

        if ((*it).first == 1) {
            rowData << new QStandardItem(QString::number(0));
            rowData << new QStandardItem(QString("%1").arg(QString::number(0)));
            rowData << new QStandardItem(QString("%1").arg(QString::number(0)));

            subvector.push_back(QString::number(0).toStdString());
            subvector.push_back(QString("%1").arg(QString::number(0)).toStdString());
            subvector.push_back(QString("%1").arg(QString::number(0)).toStdString());

        } else {
            auto first = std::prev(it)->second;
            auto next = (*it).second;
            rowData << new QStandardItem(QString("%1").arg(next - first));
            rowData << new QStandardItem(QString("%1 %2").arg(
                    QString::number(ui->spnDiffPercentInMonth->value() * std::prev(it)->first)).arg(QString("%")));
            rowData << new QStandardItem(
                    QString("%1 %2").arg(QString::number(next * 100 / firstDepo - 100)).arg(QString("%")));


            subvector.push_back(QString("%1").arg(next - first).toStdString());
            subvector.push_back(QString("%1 %2").arg(
                    QString::number(ui->spnDiffPercentInMonth->value() * std::prev(it)->first)).arg(QString("%")).toStdString());
            subvector.push_back(QString("%1 %2").arg(QString::number(next * 100 / firstDepo - 100)).arg(QString("%")).toStdString());
        }
        data.push_back(subvector);
        model->appendRow(rowData);
    }

    VLayout->addWidget(tv);
    VLayout->addLayout(HLayout);
    dlg->setLayout(VLayout);
    dlg->show();

    QRect parentRect = this->geometry();
    dlg->setGeometry(parentRect);
    dlg->move(QPoint(parentRect.bottomLeft().x(), parentRect.bottomLeft().y() + 30));


    //Save csv to file
    connect(btnSaveCSV, &QPushButton::clicked, [=]() {

        std::unique_ptr<Common::Logger::CSVLogger> logger(new Common::Logger::CSVLogger);

        std::string current_filename = RESULT_DCA_FILE;
        std::string dump_dir = (getenv("HOME") != nullptr ? std::string(getenv("HOME")) : "/tmp") + std::string("/.ssm/");
        auto current_data = QDateTime::currentDateTime().toString("dd-MM-yyyy");
        auto filename_with_date = current_data.toStdString() + "_" + current_filename;
        logger->settings(dump_dir, filename_with_date);
//        logger->Log(std::string ("Month"),std::string("Depo"), std::string("Profit") ,std::string("Percent"), std::string("Percent on Depo"));
        logger->add("Month","Depo","Profit" ,"Percent","Percent on Depo");
        for(auto items:data)
        logger->add(items);
    });

    //Save csv to file
    connect(btnGetGraph, &QPushButton::clicked, [=]() {

        auto grid = Common::Grid<std::string>(5, data.size());
        std::vector<std::string> container{};
        for (size_t rows = 0; rows < data.size(); rows++)
            for (std::size_t items = 0; items < data.at(rows).size(); items++)
                container.push_back(data.at(rows).at(items));

       grid.setDataGrid(container);
//        for(const auto &[range_assets, range_price] :grid.byColumns(0,1))
//        {
//            qDebug() << "X: " <<QString::fromStdString(range_assets) <<"\t Y: " <<stoi(range_price);
//
//        }

       std::map<std::string, std::string> need_data1 = grid.byColumns(0,1);
//       std::map<std::string, std::string, std::greater<std::string>> need_data {need_data1};

//       std::map<std::string, std::string, std::greater<std::string>> need_data {need_data1};
         auto need_data2 = invert(need_data1);
//         qDebug() <<"Invert data";
//        for(const auto &[range_assets, range_price] :need_data2)
//        {
//            qDebug() << "X: " <<QString::fromStdString(range_assets) <<"\t Y: " <<stoi(range_price);
//
//        }

        auto need_data = invert(need_data2);
//        qDebug() <<"Invert data 2:";
//        for(const auto &[range_assets, range_price] : need_data)
//        {
//            qDebug() << "X: " <<QString::fromStdString(range_assets) <<"\t Y: " <<stoi(range_price);
//
//        }
//      std::sort(need_data.begin(), need_data.end(),[](std::string &a,std::string&b)
//      {
//            return a.data()>b.data();
//      }
//      );

      std::map<QString,int> data_convert;
      std::map<int,int> data_convert1;
      for(const auto &[range_assets, range_price] : need_data)
      {
          data_convert.emplace(QString::fromStdString(range_assets), stoi(range_price));
          data_convert1.emplace(stoi(range_assets), stoi(range_price));
      }

      std::unique_ptr<Graph>  graph = std::make_unique<Graph>();
      graph->setType(Graph::TypeChart::LineChart);
      graph->setTitleGraph("Diff Percentage", "Month", "Depo");

//      graph->buildBarChart(data_convert);
//      graph->buildBarChartDiffDepo(data_convert);
     graph->buildLineChart(data_convert1);

     //View csv data from file
     char delim = ';';
     std::vector<QString> headerSDF = {"Data","Month","Depo", "Profit", "Percent", "Percent on Depo"};
    CsvModel model;
    model.setTitleWindow("Strategy Difference Percentage").setHeaderTable(headerSDF).build("/tmp/log.csv", delim);
    });
}

void MainWindow::calculateDiffPercentage() {

    resultDiffPercent.clear();
    double depo = ui->spnDepoDiffPerc->value();
    double percentage = ui->spnDiffPercentInMonth->value();
    int period = ui->spnDiffPercentDuration->value();
    resultDiffPercent = Strategy::calculateDiffPercentPeriod(depo, percentage, period);
//        resultDiffPercent = Strategy::calculateDiffPercentPeriodAddStock(m_depo,50,percentage,period);
    ui->lblResultDiffPercent->setText(QString::number(std::prev(resultDiffPercent.end())->second, 'f', 2));
    getDiffPercentDetails();
}

void MainWindow::calculateDCA() {
    resultDCA.assets = ui->spnFirstAssest->value();
    resultDCA.price = ui->spnFirstPrice->value();
    resultDCA.goal_price = ui->spnGoalPrice->value();
    m_depo = ui->spnDeposit->value();

    timer<ch::microseconds> timer;
    timer.start();
    calculate(resultDCA);
    timer.stop().print();
}

void MainWindow::setHotKey() {

    QShortcut * shortcutDCAcalc = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_S), this);
    connect(shortcutDCAcalc, &QShortcut::activated, this, [this](){
        calculateDCA();
    });

    QShortcut * shortcutDCADetail = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_D), this);
    connect(shortcutDCADetail, &QShortcut::activated, this, [this](){
        detailDCA();
    });


    QShortcut * shortcutMoonBackground = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_B), this);
    connect(shortcutMoonBackground, &QShortcut::activated, this, [this](){
    setBackgroundMainWindow();
    });


    QShortcut * shortcutACSCalculate = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_A), this);
    connect(shortcutACSCalculate, &QShortcut::activated, this, [this](){
        m_acs->show();
    });

    QShortcut * shortcutDiffPercentage = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_X), this);
    connect(shortcutDiffPercentage, &QShortcut::activated, this, [this](){
        calculateDiffPercentage();
    });

}

void MainWindow::setBackgroundMainWindow() {

    QList<QDoubleSpinBox *> spinBox = this->findChildren<QDoubleSpinBox *>();

    if (isMoonTheme == true) {
        setStyleSheet("background-color:#393C39;color: #F5F5F5;");
    } else {
        setStyleSheet("background-color:#E0E0E0; color: black;");
        for (auto *items :spinBox)
            items->setStyleSheet("background-color:white;color: black;");
    }

    (isMoonTheme == false) ? (isMoonTheme = true) : (isMoonTheme = false);

}

void MainWindow::setGuiCalculateDca(dca &res) {

    ui->lblTotalFirstSum->setText(QString::number(res.assets * res.price));

    for (const auto value:res.goal_range) {
        auto range_assets = value.first;
        auto range_price = value.second;

        ui->spnRangeAsset->setValue(range_assets);
        ui->spnRangePrice->setValue(range_price);

        ui->spnTotalAsset->setValue(range_assets + res.assets);
        ui->lblTotalSum->setText(QString::number(range_price * range_assets + res.assets * res.price));
//
//        ui->spnGoalPrice->setStyleSheet("QDoubleSpinBox {color : red;}");
//
//        ui->spnRangeAsset->setStyleSheet("QDoubleSpinBox {color : green; }");
//        ui->spnRangePrice->setStyleSheet("QDoubleSpinBox {color : green; }");
//
        ui->lblRangeSum->setText(QString::number(range_assets * range_price));
//        ui->lblRangeSum->setStyleSheet("QLabel {color : green; }");
    }
}
