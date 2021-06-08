#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include "renderarea.h"
#include "menumodel.h"
#include "tableview.h"
#include "categoryfilterproxymodel.h"
#include "aboutwindow.h"
#include "Nutrients.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:    
    Ui::MainWindow *ui;
    RenderArea *logo;
    MenuModel *McMenu;
    CategoryFilterProxyModel *userMenu;

    TableView *rcTable;
    void connectTableSignals() const;

    QMessageBox* cantOpen;
    QMessageBox* sizeError;
    QMessageBox* headerError;
    QMessageBox* dataError;

    QMessageBox* mealLimitReached;

    QMap<QMessageBox*, bool> showError;

    const int mealLimit = 11; //based on fullscreen data
    QList<QString> inMeal; //name

    void setUpError(QMessageBox* error);
    void executeError(QMessageBox* error, const QString& message);

    void setUpSearchShortcut();

    QString properTime(int min);

    AboutWindow *about;
    Nutrients *nutrients;
private slots:
    void on_actionLoad_another_menu_triggered();
    void on_actionAbout_triggered();
    void on_actionChange_Sorting_triggered();
    void on_SearchButton_clicked();
    void on_actionUndo_all_changes_triggered();

    void addMealItem(QModelIndex i);
    void removeMenuItem(QModelIndex i);
    void viewItemNutrients(QModelIndex i) const;

    void on_toolButton_clicked();

signals:
    void itemAlreadyInMenu(QString);
};
#endif // MAINWINDOW_H

