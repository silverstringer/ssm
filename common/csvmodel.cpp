#include "csvmodel.h"
#include "csvreader.h"


CsvModel::CsvModel(QObject *parent) : QObject(parent) {

}


void CsvModel::build(const std::string filename, char delimeter) {
    CSVRead reader_csv(filename, delimeter);
    readCSV(reader_csv);
    if (m_titleWindow == " ")
        setTitleWindow(filename);

    auto column = reader_csv.getSizeGrid().first;
    auto rows = reader_csv.getSizeGrid().second;
    std::cout << "Counter column: " << column << "\n";
    std::cout << "Counter rows: " << rows << "\n";


    QDialog *dlg = new QDialog();
    dlg->setWindowTitle(QString::fromStdString(m_titleWindow));
    QVBoxLayout *VLayout = new QVBoxLayout(dlg);
    QHBoxLayout *HLayout = new QHBoxLayout();


    // Configure the table view
    auto tv = new QTableView(dlg);
    auto model = new QStandardItemModel();
    model->setColumnCount(column);
    tv->setModel(model);
    tv->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tv->resizeColumnsToContents();
    tv->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    if (m_header.size() > 0) {
        for (size_t item = 0; item < m_header.size(); item++)
            model->setHorizontalHeaderItem(item, new QStandardItem(m_header.at(item)));
    }


//    std::map<int , QString> header {{0, "DataTime"},{1,"Month"},{2,"Depo"}, {3,"Profit"},{4,"Percent"}, { 5, "Percent on Depo"}};
//    for(const auto &[item, head] : header)
//        model->setHorizontalHeaderItem(item, new QStandardItem(head));



    auto data = reader_csv.getData();
    auto grid_data = Common::Grid<std::string>(column, rows);
    grid_data.setDataGrid(data);

    QList<QStandardItem *> rowData;
    for (auto items = 0; items < rows; items++) {
        rowData.clear();
        for (auto subitems = 0; subitems < column; subitems++) {
            auto cell = grid_data.at(items, subitems);
            rowData << new QStandardItem(QString::fromStdString(cell));
        }
        model->appendRow(rowData);
    }

    VLayout->addWidget(tv);
    VLayout->addLayout(HLayout);
    dlg->setLayout(VLayout);
    dlg->show();

//    QRect parentRect = this->geometry();
//    dlg->setGeometry(p);
//    dlg->move(QPoint(parentRect.bottomLeft().x(), parentRect.bottomLeft().y() + 30));
}

CsvModel &CsvModel::setTitleWindow(const std::string titleWindow) {

    m_titleWindow = titleWindow;
    return *this;
}

CsvModel &CsvModel::setHeaderTable(std::vector<QString> header) {
    m_header = header;
    return *this;
}




