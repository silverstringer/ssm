
#include "avgcoststocks.h"
#include <QDebug>

AvgCostStocks::AvgCostStocks(QWidget *parent) : QWidget(parent)
{

    m_acsView = new QTableView (this);
    m_acsView->setSortingEnabled(false);
    m_acsView->resizeColumnsToContents();
    m_acsView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    m_model = new ACSModel(this);

    makeGUI();
    connect(btnDelete,    SIGNAL(clicked()), SLOT(removeItem()));
    connect(btnClearAll,  SIGNAL(clicked()), SLOT(clearItems()));
    connect(btnAdd,       SIGNAL(clicked()), SLOT(addItem()));
    connect(btnResult,    SIGNAL(clicked()), SLOT(result()));
}

AvgCostStocks::~AvgCostStocks()
{
    delete m_acsView;
}

void AvgCostStocks::addItem()
{
    m_model->insertItem(m_model->rowCount() + 1, 0);
}



void AvgCostStocks::removeItem()
{
    QItemSelectionModel * selectionModel =  m_acsView->selectionModel();

    QModelIndexList indexes = selectionModel->selectedRows();
    QModelIndex index;

    foreach (index, indexes) {
        int row = index.row();
        m_model->removeRows(row, 1, QModelIndex());
    }

}

void AvgCostStocks::result()
{
    auto data = m_model->getData();
    double all_assets   {0};
    double total_amount {0};
    double total_price {0};
    for(const auto key: data.toStdMap()) {
        all_assets += key.first;
        total_amount += key.second;
        total_price += key.first * key.second;
    }
    qDebug() <<"Total Assets" <<all_assets <<"Total Price" <<total_price <<"Avg price" <<total_price/all_assets;
    m_model->insertItem(all_assets, total_price/all_assets);
}

void AvgCostStocks::clearItems()
{
    QItemSelectionModel * selectionModel =  m_acsView->selectionModel();

    QModelIndexList indexes = selectionModel->selectedRows();
    QModelIndex index;

    foreach (index, indexes) {
        int row = index.row();
        m_model->removeRows(1, row, QModelIndex());
    }
}

void AvgCostStocks::makeGUI()
{

    QVBoxLayout * vlayout = new QVBoxLayout;

    QHBoxLayout * hlayout = new QHBoxLayout;

    btnAdd = new QPushButton(tr("Add"));
    btnClearAll = new QPushButton(tr("Clear"));
    btnDelete = new QPushButton(tr("Delete"));
    btnResult = new QPushButton(tr("Result"));

    m_acsView->setModel(m_model);

    vlayout ->addWidget(m_acsView);
    vlayout->addLayout(hlayout);

    hlayout->addWidget(btnAdd);
    hlayout->addWidget(btnClearAll);
    hlayout->addWidget(btnDelete);
    hlayout->addWidget(btnResult);
    setLayout(vlayout);

}
