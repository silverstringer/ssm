#ifndef SSM_CSVMODEL_H
#define SSM_CSVMODEL_H

#include <QStandardItem>
#include <QStandardItemModel>
#include <QTableView>
#include <QDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QObject>


#include <iostream>
#include <string>

#include <grid.h>

/**
 *@brief load date from csv and show csvtable
 *@todo paint graph by model
 */
class CsvModel : public QObject {
    Q_OBJECT
public:
    CsvModel(QObject *parent = 0);
    ~CsvModel() = default;
    void build(const std::string filename, char delimeter );
    CsvModel& setTitleWindow(const std::string titleWindow );
    CsvModel& setHeaderTable(std::vector<QString> header);
private:
    std::string m_titleWindow;
    std::vector<QString> m_header;

};

#endif //SSM_CSVMODEL_H
