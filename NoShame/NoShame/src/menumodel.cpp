#include "menumodel.h"
#include <QtWidgets>
#include <QErrorMessage>
//#include<iostream>

MenuModel::MenuModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

QVariant MenuModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole)
         return QVariant();

    if (headers.isEmpty() || orientation==Qt::Vertical)
        return section + 1;

    if (orientation==Qt::Horizontal)
        return headers.at(section);

    return QVariant();
}

int MenuModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return McData.size();
}

int MenuModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid() || McData.isEmpty())
        return 0;

    return McData[0].size();
}

QVariant MenuModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    int row = index.row();
    int column = index.column();

    switch (role)
    {
    case Qt::DisplayRole:
    case Qt::EditRole:
        return McData.at(row).at(column);

    case Qt::BackgroundColorRole:
    {
        QColor yellow(255, 221, 66);
        return QBrush(yellow);
    }
    default:
        break;
    }

    return QVariant();
}

void MenuModel::appendRow(const QList<QVariant>& newRow)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    McData.append(newRow);
    endInsertRows();
}

void MenuModel::removeRow(int segment)
{
    beginRemoveRows(QModelIndex(), segment, segment); //no checking because only going to recieve signal from tableview so there will be no indexses out of range
    McData.removeAt(segment);
    endRemoveRows();
}

QList<QVariant> MenuModel::getItem(int row) const
{
    return McData.at(row);
}

QList<QString> MenuModel::getHeaders() const
{
    return headers;
}

void MenuModel::setOverrideTo(bool v)
{
    overrideOnImport = v;
}

void MenuModel::importData()
{
    QString path = QFileDialog::getOpenFileName(); //Standart csv parcing
    QFile inputFile(path);
    if(!inputFile.open(QFile::ReadOnly | QFile::Text))
        throw std::runtime_error("Not able to open the file from the path");

    if (overrideOnImport)
        clear();

    QTextStream inputStream(&inputFile);

    QString labelLine = inputStream.readLine();
    QList<QString> headersHolder = labelLine.split(';');

    if(headers.isEmpty()) //if those are undefined we need their indices
    {
        categoryColumn = headersHolder.indexOf("Category");
        nameColumn = headersHolder.indexOf("Item");
        caloryColumn = headersHolder.indexOf("Calories");
    }

    if (categoryColumn == -1 || nameColumn == -1 || caloryColumn == -1) //if unseccsesful throw error and refresh
    {
        categoryColumn = -1;
        nameColumn = -1;
        caloryColumn = -1;
        throw std::domain_error("The Menu dosn't have the necessary columns to properly operate (Category, Item, Calories)");
    }

    if (!headers.isEmpty() && headersHolder.size() != headers.size())
        throw std::out_of_range("The sizes of the menues don't match");
    else if(!headers.isEmpty() && headers != headersHolder)
        throw std::invalid_argument("The headers of the menues don't match or are in different order");
    else if (headers.isEmpty())
        headers = headersHolder;

    emit headerDataChanged(Qt::Horizontal, 0, headers.size()); // whe we firstly import the model just in case


    while (!inputStream.atEnd())
    {
        QString line = inputStream.readLine();
        if (line.isEmpty())
            break;

        QList<QVariant> row;
        for (QString& item : line.split(';'))
        {
            bool ok = false;
            QVariant num = item.toInt(&ok);
            if (ok)
            {
                row.append(num);
                continue;
            }
            num = item.toDouble(&ok);
            if (ok)
            {
                row.append(num);
                continue;
            }
            row.append(item);
        }
        appendRow(row);

        QVariant category = row.at(categoryColumn); // Get the categories and number of items while constructing the model
        if (categories.contains(category))
            categories[category] += 1;
        else
            categories[category] = 1;
    }

    if(overrideOnImport)
        FilePath = path; //if everything was a success we save path for reverting sata to original state

}

void MenuModel::refreshData()
{
    clear();
    QFile inputFile(FilePath); //still have to reenstate headers because we cleared them in clear(), but we know they are good for sure
    inputFile.open(QFile::ReadOnly | QFile::Text);
    QTextStream inputStream(&inputFile);

    QString labelLine = inputStream.readLine();
    headers = labelLine.split(';');

    emit headerDataChanged(Qt::Horizontal, 0, headers.size());

    categoryColumn = headers.indexOf("Category");   // know for a fact they are good
    nameColumn = headers.indexOf("Item");
    caloryColumn = headers.indexOf("Calories");

    while (!inputStream.atEnd())
    {
        QString line = inputStream.readLine();
        if (line.isEmpty())
            break;

        QList<QVariant> row;
        for (QString& item : line.split(';'))
        {
            bool ok = false;
            QVariant num = item.toInt(&ok);
            if (ok)
            {
                row.append(num);
                continue;
            }
            num = item.toDouble(&ok);
            if (ok)
            {
                row.append(num);
                continue;
            }
            row.append(item);
        }
        appendRow(row);

        QVariant category = row.at(categoryColumn); // Get the categories and number of items while constructing the model
        if (categories.contains(category))
            categories[category] += 1;
        else
            categories[category] = 1;

    }
}

Qt::ItemFlags MenuModel::flags(const QModelIndex &index) const
{
    if (index.column() == nameColumn || index.column() == categoryColumn) //names and categories shouldn't be changable
        return Qt::ItemIsSelectable | QAbstractTableModel::flags(index);
    return Qt::ItemIsEditable | QAbstractTableModel::flags(index);
}

bool MenuModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role == Qt::EditRole) {
        if (index.row() >= McData.size() || index.column() >= McData.at(0).size())
            return false;
        McData[index.row()][index.column()] = value.toString();
        emit dataChanged(index, index);
        return true;
        }
    return false;
}

void MenuModel::clear()
{
    if (!McData.isEmpty())
    {
        int hSize = headers.size();

        beginRemoveRows(QModelIndex(), 0 , McData.size() - 1);
        beginRemoveColumns(QModelIndex(), 0, McData.at(0).size() - 1);

        McData.clear();

        endRemoveRows();
        endRemoveColumns();

        headers.clear();

        emit headerDataChanged(Qt::Horizontal, 0, hSize);

        categories.clear();

        categoryColumn = -1;
        nameColumn = -1;
        caloryColumn = -1;
    }
}

bool MenuModel::isEmpty() const
{
    return McData.isEmpty();
}




