#ifndef TABLEVIEW_H
#define TABLEVIEW_H

#include<QWidget>
#include <QMenu>
#include "menumodel.h"
#include "categoryfilterproxymodel.h"

class QTableView;

class TableView : public QWidget
{
    Q_OBJECT
public:
    explicit TableView(QWidget *parent = nullptr, CategoryFilterProxyModel *m = new CategoryFilterProxyModel());
    ~TableView();

    void setSortingEnabled(bool v);
    void setModel(CategoryFilterProxyModel *m);
    void setSortabilityForModel(bool v);

private slots:
    void customMenuRequested(const QPoint& pos);
    void customHeaderMenuRequested(int logicalIndex);
    void handleIndicatorChanged(int logicalIndex);

    void customMenuAdd();
    void customMenuRemove();
    void customMenuDetails() const;

    void setupCategoriesMenu();
    void eliminateCategory(const QVariant& catName);

signals:
    void addItemToMeal(QModelIndex);
    void removeItemFromModel(QModelIndex);
    void giveDetails(QModelIndex) const;
private:
    QMenu *tMenu;
    QMenu *hMenu;

    QTableView *table;
    CategoryFilterProxyModel *model;
    QModelIndex rightClicked;

    void setupCustomMenu();


    bool categoriesSetUp = false;
    bool isModelSortable = false;
};

#endif // TABLEVIEW_H
