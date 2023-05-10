#include "chart.h"
QT_CHARTS_USE_NAMESPACE

void Graph::setChartView(QChart *chart)
{
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setRubberBand(QChartView::HorizontalRubberBand); //for zoom graph

    QDialog *dialog = new QDialog();
    dialog->setWindowTitle(title);
    QVBoxLayout *mainlayout = new QVBoxLayout();
    mainlayout->addWidget(chartView);

    dialog->setWindowFlags(Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint);
    dialog->setWindowFlags(
            Qt::Window | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint);
    dialog->setLayout(mainlayout);
    dialog->resize(800, 600);
    dialog->exec();
}

void Graph::buildBarChart(const std::map<QString,int> &data) {
    QBarSet *set[data.size()];
    QBarSeries *series = new QBarSeries();
    int i=0;
    for (auto&& [key, value] : data) {
        set[i] = new QBarSet(key);
        *set[i] << value;
        series->append(set[i]);
        i++;
    }

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle(title);
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QStringList categories;
    categories << titleAxisX;
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    auto max = std::max_element(data.begin(), data.end(),
                                [](const std::pair<QString, int> &p1, const std::pair<QString, int> &p2) {
                                    return p1.second < p2.second;
                                });
    auto min = std::min_element(data.begin(), data.end(),
                                [](const std::pair<QString, int> &p1, const std::pair<QString, int> &p2) {
                                    return p1.second < p2.second;
                                });

    axisY->setRange(0, max->second + min->second);
    axisY->setTickCount(data.size() * 1.5);

    axisY->setTitleText(titleAxisY);
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    setChartView(chart);

}


void Graph::buildBarChartDiffDepo(const std::map<QString,int> &data)
{

    QBarSet *set[data.size()];
    auto depo = data.begin()->second;
    QStackedBarSeries *series = new QStackedBarSeries();
    int i = 0;
    for (auto&& [key, value] : data) {
        set[i] = new QBarSet(key);
        *set[i] <<depo<<value;
//        *set[i] <<0<<3<<4<<8<<45<<90<<123<<450 <<600<<900<<0<<0;
//        *set[i] <<depo<<value<<0<<0<<0<<0<<0<<0<<0<<0<<0<<0;
        series->append(set[i]);
        i++;
    }

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle(title);
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QStringList categories;
    for (auto&& [key, value] : data) {
        categories << key;
    }

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    auto max = std::max_element(data.begin(), data.end(),
                                [](const std::pair<QString, int> &p1, const std::pair<QString, int> &p2) {
                                    return p1.second < p2.second;
                                });
    auto min = std::min_element(data.begin(), data.end(),
                                [](const std::pair<QString, int> &p1, const std::pair<QString, int> &p2) {
                                    return p1.second < p2.second;
                                });

//    axisY->setRange(0, max->second + min->second);
//    axisY->setTickCount(data.size() * 1.5);

    axisY->setTitleText(titleAxisY);
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    setChartView(chart);

}

void Graph::buildLineChart(const std::map<int,int> &data) {

    QLineSeries *series = new QLineSeries();

    for (auto&& [key, value] : data)
        series->append(key,value);

    QChart *chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->setTitle(titleAxisY);
//    QValueAxis *axisY = new QValueAxis();
//
//    axisY->setTickCount(10);
//
//    axisY->setTitleText(titleAxisY);
//    chart->legend()->setVisible(true);
//    chart->legend()->setAlignment(Qt::AlignBottom);
//    chart->addAxis(axisY, Qt::AlignLeft);
//    series->attachAxis(axisY);


    setChartView(chart);
}
