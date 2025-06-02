#include "chart.h"
QT_CHARTS_USE_NAMESPACE


Graph::Graph() {

    m_datetimeFormat = "yyyy-MM-dd HH:mm:ss";
}

void Graph::setChartView(QChart *chart)
{
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setRubberBand(QChartView::HorizontalRubberBand); //for zoom graph

    QDialog *dialog = new QDialog();
    dialog->setWindowTitle(m_title);
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
    chart->setTitle(m_title);
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QStringList categories;
    categories << m_titleAxisX;
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    auto param = min_max_range_element(data);
    axisY->setRange(0, param.first + param.second);

    axisY->setTickCount(data.size() * 1.5);

    axisY->setTitleText(m_titleAxisY);
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
        series->append(set[i]);
        i++;
    }

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle(m_title);
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
    [[maybe_unused]]auto param = min_max_range_element(data);

//    axisY->setRange(0, max->second + min->second);
//    axisY->setTickCount(data.size() * 1.5);

    axisY->setTitleText(m_titleAxisY);
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    setChartView(chart);

}

void Graph::buildLineChart(const std::map<int,int> &data) {

    QLineSeries *series = new QLineSeries();

    QVector<QPointF> points;

    for (auto&& [key, value] : data)
    points.append(QPointF(key, value));
    series->replace(points);

    // for (auto&& [key, value] : data)
        // series->append(key,value);

    QChart *chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->setTitle(m_titleAxisY);
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

void Graph::buildDateTimeAxes(const std::map<QString,int> &data) {

    QLineSeries *series = new QLineSeries();

    for (auto items : data) {
        QDateTime momentInTime = QDateTime::fromString(items.first, m_datetimeFormat);
        series->append(momentInTime.toMSecsSinceEpoch(), items.second);
    }

    QChart *chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(series);
    chart->setTitle(m_titleAxisY);


    QDateTimeAxis * axisX = new QDateTimeAxis;
    axisX->setTickCount(10);
//    axisX->setTickCount(data.size()/4);
    axisX->setFormat(m_datetimeFormat);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);


    auto param = min_max_range_element(data);

    QValueAxis * axisY = new QValueAxis;
    axisY->setLabelFormat(" %i");
    axisY->setRange(0, param.first + param.second);
    axisY->setTickCount(10);

    axisY->setTitleText(m_titleAxisY);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    setChartView(chart);
}

void Graph::buildPieChart(const std::map<QString,int> &data) {

    QPieSeries * series = new QPieSeries();
    for(auto &items: data)
        series->append(items.first, items.second);

    series->setLabelsVisible(true);
    series->setLabelsPosition(QPieSlice::LabelInsideHorizontal);

    QPieSlice * slice = series->slices().at(1);
    for( auto slice : series->slices())
        slice->setLabel(QString("%1% %2").arg(100  * slice->percentage(),  0, 'f', 1).arg(slice->label()));
    slice->setExploded();
    slice->setLabelVisible();
    slice->setPen(QPen(Qt::darkGreen,2));
    slice->setBrush(Qt::green);

    QChart *chart = new QChart();
//    chart->legend()->hide();
    chart->addSeries(series);
    setChartView(chart);
}
