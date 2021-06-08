#ifndef CATEGORYFILTERPROXYMODEL_H
#define CATEGORYFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>

enum SortFilterMode
{
    InitialState,
    CategoriesFilter,
    WordSearch,
    BasicSorting = 4
};

class CategoryFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit CategoryFilterProxyModel(QObject *parent = nullptr);

    void recieveSetupData(int cc, int sc, const QList<QVariant>& c);
    QList<QVariant> getCategories() const;
    QString getName(const QModelIndex& i) const; // It will be of use when adding items to meal and searching by name

    void addMode(int m);
    void removeMode(int m);
    void resetMode();

    void setSearchWord(const QString& searchRequest);

    void setNewCategories(const QList<QVariant>& c);
    void removeCategory(const QVariant& category);
    void addCategory(const QVariant& category);

    void eliminateCategory(const QVariant& category);

    bool isEmpty() const;
protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;

private:
    int mode = BasicSorting;

    QList<QVariant> categories;
    int categorieColumn;

    QString searchWord;
    int searchColumn;

signals:
    void categoriesChanged();
    void categoryNoLongerNeeded(QVariant);
};

#endif // CATEGORYFILTERPROXYMODEL_H
