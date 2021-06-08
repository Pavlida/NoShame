#include "categoryfilterproxymodel.h"

CategoryFilterProxyModel::CategoryFilterProxyModel(QObject *parent) : QSortFilterProxyModel(parent)
{
}

void CategoryFilterProxyModel::recieveSetupData(int cc, int sc, const QList<QVariant>& c)
{
    categorieColumn = cc;
    searchColumn = sc;
    categories = c;

    emit categoriesChanged();
}

QList<QVariant> CategoryFilterProxyModel::getCategories() const
{
    return categories;
}
QString CategoryFilterProxyModel::getName(const QModelIndex& i) const
{
    return data(index(i.row(), searchColumn)).toString();
}

void CategoryFilterProxyModel::addMode(int m)
{
    if (!(mode & m)) //if the mode is new we refresh our sorting/filtering
    {
        mode += m;
        invalidate();
    }
}

void CategoryFilterProxyModel::removeMode(int m)
{
    if (mode & m) //if the mode is new we refresh our sorting/filtering
    {
        mode -= m;
        invalidate();
    }
}

void CategoryFilterProxyModel::resetMode()
{
    mode = InitialState;
    invalidate();
}

void CategoryFilterProxyModel::setSearchWord(const QString& searchRequest)
{
    searchWord = searchRequest;
    invalidateFilter();
}

void CategoryFilterProxyModel::setNewCategories(const QList<QVariant>& c)
{
    categories = c;
    invalidateFilter();

    emit categoriesChanged();
}

void CategoryFilterProxyModel::removeCategory(const QVariant& category)
{
    categories.removeOne(category);
    invalidateFilter();
}

void CategoryFilterProxyModel::addCategory(const QVariant& category)
{
    categories.append(category);
    invalidateFilter();
}

void CategoryFilterProxyModel::eliminateCategory(const QVariant& category)
{
    categories.removeOne(category);

    emit categoryNoLongerNeeded(category);
}

bool CategoryFilterProxyModel::filterAcceptsRow(int sourceRow,const QModelIndex &sourceParent) const
{
    if (mode & CategoriesFilter) //firstly try the categorie filter
    {
        QModelIndex categorie = sourceModel()->index(sourceRow, categorieColumn, sourceParent);

        if (!categories.contains(sourceModel()->data(categorie)))
            return false;
    }
    if (mode & WordSearch) //then the word search
    {
        QModelIndex word = sourceModel()->index(sourceRow, searchColumn, sourceParent);

        if (!sourceModel()->data(word).toString().toLower().contains(searchWord.toLower()))
            return false;
    }
    return true;
}

bool CategoryFilterProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    if(mode & BasicSorting)
        return QSortFilterProxyModel::lessThan(left, right);
    return false; //For not bothering with ascending/descending order and just sorting in ascending if initial
}

bool CategoryFilterProxyModel::isEmpty() const
{
    return categories.isEmpty();
}
