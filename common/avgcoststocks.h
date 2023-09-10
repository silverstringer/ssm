#ifndef AVGCOSTSTOCKS_H
#define AVGCOSTSTOCKS_H

#include <QWidget>
#include <QTableView>
#include <QHeaderView>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "Model/acsmodel.h"


/**
 * @brief  average resultDCA of buy assets
 */
class AvgCostStocks : public QWidget
{
    Q_OBJECT
public:
    explicit AvgCostStocks(QWidget *parent = nullptr);
    ~AvgCostStocks();
public slots:
    void addItem();
    void clearItems();
    void removeItem();
    void result();
protected:
    void makeGUI();
private:
    QPushButton * btnAdd;
    QPushButton * btnClearAll;
    QPushButton * btnDelete;
    QPushButton * btnResult;

    QTableView *  m_acsView { nullptr};
    ACSModel *    m_model {nullptr};
};

#endif // AVGCOSTSTOCKS_H
