#include "tableview.h"
#include <QHeaderView>
#include <QLayout>
#include <QTableView>
#include <QWidget>
#include <QCheckBox>
#include <QWidgetAction>
#include <QLabel>

TableView::TableView(QWidget *parent, CategoryFilterProxyModel* m) : QWidget(parent)
{
    model = m;
    QVBoxLayout *l = new QVBoxLayout(this);
    table = new QTableView(this);

    table->setModel(model);
    table->setFont(QFont("Calibri", 9));

    setupCustomMenu();
    connect(model, SIGNAL(categoriesChanged()), SLOT(setupCategoriesMenu()));

    table->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(table, SIGNAL(customContextMenuRequested(QPoint)), this,  SLOT(customMenuRequested(QPoint)));

    table->horizontalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(table->horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(customHeaderMenuRequested(int)));

    connect(model, SIGNAL(categoryNoLongerNeeded(const QVariant&)), this, SLOT(eliminateCategory(const QVariant&)));

    connect(table->horizontalHeader(), SIGNAL(sortIndicatorChanged(int, Qt::SortOrder)), this, SLOT(handleIndicatorChanged(int))); //basically for design purposes

    l->addWidget(table);
}

void TableView::setupCustomMenu()
{
    tMenu = new QMenu(this);
    tMenu->addAction("Add product to your meal", this, SLOT(customMenuAdd()));
    tMenu->addAction("Remove product from the menu", this, SLOT(customMenuRemove()));
    tMenu->addAction("View product details", this, SLOT(customMenuDetails()));

}

void TableView::setupCategoriesMenu()
{
    if(categoriesSetUp)
        delete hMenu; // not to leak data
    hMenu = new QMenu(this);
    for (QVariant &category : model->getCategories())
    {
        QWidget *cWidget = new QWidget(hMenu); // create labels with checkable boxes

        QHBoxLayout *cL = new QHBoxLayout(cWidget);

        QLabel *catName = new QLabel(category.toString(), cWidget);
        QCheckBox *checkBox = new QCheckBox(cWidget);
        checkBox->setChecked(true);

        cL->addWidget(catName);
        cL->addWidget(checkBox, Qt::AlignRight);

        QWidgetAction *cat = new QWidgetAction(hMenu);
        cat->setDefaultWidget(cWidget);

        cWidget->setObjectName(category.toString()); //for navigation
        cat->setObjectName(category.toString() + "Action");

        hMenu->addAction(cat);

        connect(checkBox, &QCheckBox::stateChanged, model, [catName, this](int checked)
        {
            if(checked & Qt::Checked)
            {
                model->addCategory(catName->text());
            }
            else
            {
                model->removeCategory(catName->text());
            }
            model->addMode(SortFilterMode::CategoriesFilter);
        });
    }
    categoriesSetUp = true;
}

void TableView::eliminateCategory(const QVariant& catName)
{
    QWidget *cWidget = hMenu->findChild<QWidget *>(catName.toString()); //widget and widgetaction have a weir connection so we have to find and delte them separately
    QWidgetAction *cat = hMenu->findChild<QWidgetAction *>(catName.toString() + "Action");
    delete cWidget;
    delete cat;
}

void TableView::setSortingEnabled(bool v)
{
    table->setSortingEnabled(v);
}

void TableView::setSortabilityForModel(bool v) // Making it possible to change the sortability of the model without it being emidiately sorted after status has been changed
{
    isModelSortable = v;
    if(!v)
        setSortingEnabled(false);
}

void TableView::customMenuRequested(const QPoint& pos)
{
    if(!model->isEmpty())
    {
        rightClicked = table->indexAt(pos);
        table->selectRow(rightClicked.row());
        tMenu->popup(table->viewport()->mapToGlobal(pos));
    }
}

void TableView::customMenuAdd()
{
    emit addItemToMeal(rightClicked);
}

void TableView::customMenuRemove()
{
    emit removeItemFromModel(rightClicked);
}

void TableView::customMenuDetails() const
{
    emit giveDetails(rightClicked);
}


void TableView::customHeaderMenuRequested(int logicalIndex)
{
    table->clearSelection();
    if (model->headerData(logicalIndex, Qt::Horizontal) == "Category") //Not to trigger sorting after the first startup
    {
        QPoint p(table->horizontalHeader()->sectionPosition(logicalIndex), 0);
        hMenu->popup(table->horizontalHeader()->viewport()->mapToGlobal(p));
    }
    else if (isModelSortable  && !table->isSortingEnabled())
    {
        setSortingEnabled(true);
    } 
}

void TableView::handleIndicatorChanged(int logicalIndex)
{
    if (logicalIndex == 0)
    {
        setSortingEnabled(false);
    }
}

TableView::~TableView()
{
    delete tMenu;
    delete hMenu;
    delete table;
}
