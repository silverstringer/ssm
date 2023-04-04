#ifndef CHART_H
#define CHART_H
#include <QDialog>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QtCharts/QLineSeries>
#include <QtCharts>
#include <QVBoxLayout>

#include <map>
#include <utility>

class Graph {
public:
    enum class TypeChart {
        BarChart,
        LineChart
    };

    Graph() = default;

    void setTitleGraph(const QString title, const QString titleX, const QString titleY)
    {
        this->title = title;
        titleAxisX = titleX;
        titleAxisY = titleY;
    }

    void setType(TypeChart type ) {
        m_type = type;
    }

    void buildBarChart(const std::map<QString,int> &data);
    void buildLineChart(const std::map<int,int> &data);

protected:
    void setChartView(QChart *chart);

private:
    QString title;
    QString titleAxisX;
    QString titleAxisY;
    TypeChart m_type;
};


#endif // CHART_H
