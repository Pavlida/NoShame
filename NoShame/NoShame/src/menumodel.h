#ifndef MENUMODEL_H
#define MENUMODEL_H

#include <QAbstractTableModel>

class MenuModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit MenuModel(QObject *parent = nullptr);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QList<QString> getHeaders() const;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    QList<QVariant> getItem(int row) const;

    void appendRow(const QList<QVariant>& newRow);
    void removeRow(int segment);

    void importData();
    void refreshData();
    void setOverrideTo(bool v);

    bool isEmpty() const;
    // Information for custom proxyModel
    QMap<QVariant, size_t> categories; //categories, q-ty of items in that category
    int categoryColumn = -1;

    int nameColumn = -1;

    // Information for meal list
    int caloryColumn = -1;
private:
    bool overrideOnImport = true;
    void clear();

    QString FilePath;
    QList<QList<QVariant>> McData;
    QList<QString> headers;    
};

#endif // MENUMODEL_H
