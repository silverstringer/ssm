#ifndef ACSMODEL_H
#define ACSMODEL_H

#include <QAbstractTableModel>
#include <QMap>
#include <QMultiMap>
#include <QObject>


class ACSModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit ACSModel(QObject *parent = nullptr);
    ~ACSModel() =  default;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    virtual bool insertRows(int row, int count, const QModelIndex &parent) override;
    bool removeRows(int position, int rows, const QModelIndex & index = QModelIndex()) override;
    QHash<int, QByteArray> roleNames() const override;

    void insertItem(double assets, double price = 0);

    QMultiMap<double, double> getData();

signals:
//    void editKitNumber(quint16 key, quint16 newKey);

private:
    QMultiMap<double, double> m_avgCostStock;

    enum ACSROLES {
        Assets = Qt::UserRole + 1,
        Price,
        TotalAmount,

    };


};

#endif // ACSMODEL_H
