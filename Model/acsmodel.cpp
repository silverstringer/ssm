
#include "acsmodel.h"
#include <QColor>

ACSModel::ACSModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

int ACSModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_avgCostStock.size();
}

int ACSModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 3;
}

QVariant ACSModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    int row = index.row();
    if(row >= m_avgCostStock.size())
        return QVariant();



    QList<double> values = m_avgCostStock.keys();
    QList<double> val = m_avgCostStock.values();

    if(role == Qt::DisplayRole or role==Qt::EditRole)
    {
        switch (index.column())
        {
        case 0:
            return values[row];
        case 1:
        {
             return val[row];
        }
        case 2:
                return (double) (values[row] * val[row]);

        }
    }

    if(role == Qt::TextColorRole && index.column() == 1 && index.row() == m_avgCostStock.size())
        return qVariantFromValue(QColor(Qt::green));

    return QVariant();
}

QVariant ACSModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
        case 0:
            return QString("Assets");
        case 1:
            return QString("Price");
        case 2:
            return QString("TotalAmount");
        }
    }

    if (role == Qt::DisplayRole && orientation == Qt::Vertical)
        return section + 1;

    return QVariant();
}

bool ACSModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role == Qt::EditRole) {
//        if (!checkIndex(index) or index.column() > 2)
//            return false;

        QList <double> keys = m_avgCostStock.keys();
        double item = keys[index.row()];

        if(index.column() == 0)
        {
            if(m_avgCostStock.contains(value.toDouble()))
                return false;

            m_avgCostStock.remove(item);
            m_avgCostStock.insert(value.toDouble(), 0);
//            emit editKitNumber(item, value.toInt());
        }

        if(index.column() == 1)
        {
               m_avgCostStock.remove(item);
               m_avgCostStock.insert(item, value.toDouble());
        }

//        if(index.column() == 2)
//            return value.toDouble() *item;



        return true;
    }
    return false;
}

Qt::ItemFlags ACSModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return Qt::ItemIsEnabled;

    return Qt::ItemIsEnabled  | Qt::ItemIsSelectable|Qt::ItemIsUserCheckable|Qt::ItemIsEditable;
}

bool ACSModel::insertRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent);
    beginInsertRows(QModelIndex(), row, row + count - 1);

    endInsertRows();
    return true;
}

bool ACSModel::removeRows(int position, int rows, const QModelIndex &index)
{

    Q_UNUSED(index);
    beginRemoveRows(QModelIndex(), position, position + rows -1);
    auto beg = m_avgCostStock.begin();
    for( auto row = 0; row < rows; row++)
        m_avgCostStock.erase(beg + position);
    endRemoveRows();
    return true;

}

QHash<int, QByteArray> ACSModel::roleNames() const
{

 QHash <int, QByteArray> roles;
 roles[ACSROLES::Assets] = "assets";
 roles[ACSROLES::Price] = "price";
 roles[ACSROLES::TotalAmount] = "totalamount";
 return roles;
}

void ACSModel::insertItem(double assets, double price)
{

        insertRows(rowCount(), 1, QModelIndex());
        m_avgCostStock.insert(assets, price);

}

QMultiMap<double, double> ACSModel::getData()
{
    return m_avgCostStock;
}


