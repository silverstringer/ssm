#ifndef CHART_H
#define CHART_H
#include <QDialog>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QStackedBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QtCharts/QLineSeries>
#include <QtCharts/QPieSeries>
#include <QtCharts>
#include <QVBoxLayout>

#include <map>
#include <utility>
#include <type_traits>


template<typename Container>
auto min_max_range_element (Container &data) {

    auto max = std::max_element(data.begin(), data.end(),
                                [](const std::pair<QString, int> &p1, const std::pair<QString, int> &p2) {
                                    return p1.second < p2.second;
                                });

    auto min = std::min_element(data.begin(), data.end(),
                                [](const std::pair<QString, int> &p1, const std::pair<QString, int> &p2) {
                                    return p1.second < p2.second;
                                });
    return std::make_pair(min->second, max->second);
}

class Graph {
public:
    enum class TypeChart {
        BarChart,
        LineChart,
        DateTimeAxesChart,
        PieChart,
    };

    Graph();

    void setTitleGraph(const QString title, const QString titleX, const QString titleY)
    {
        this->m_title = title;
        m_titleAxisX = titleX;
        m_titleAxisY = titleY;
    }

    void setType(TypeChart type ) {
        m_type = type;
    }

    void buildBarChart(const std::map<QString,int> &data);
    void buildBarChartDiffDepo(const std::map<QString,int> &data);
    void buildLineChart(const std::map<int,int> &data);
    void buildDateTimeAxes(const std::map<QString, int> &data); //  build graph by timelineseries
    void buildPieChart(const std::map<QString,int> &data);

    /**
     *
     * @brief общяя функция построения
     * @param dataset
     * @todo  make one type data
     */
//    template <typename T>
//    void build( T& dataset)
//    {
//        if(m_type == TypeChart::BarChart)
//            buildBarChart(dataset);
//        if(m_type == TypeChart::LineChart)
//            buildLineChart(dataset);
//    }


//    template <typename T>
//    void build( T& dataset)
//    {
//        if(m_type == TypeChart::BarChart)
//            buildBarChart(dataset);
//        if(m_type == TypeChart::LineChart)
//            buildLineChart(dataset);
//    }

//    template<>
//    void build<std::map<QString,int>>(std::map<QString,int>& dataset)
//    {
//        if(m_type == TypeChart::BarChart)
//            buildBarChart(dataset);
//    }



protected:
    void setChartView(QChart *chart);

private:
    QString m_title;
    QString m_titleAxisX;
    QString m_titleAxisY;
    QString m_datetimeFormat;
    TypeChart m_type;

};

#endif // CHART_H
