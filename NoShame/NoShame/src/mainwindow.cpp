#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->actionUndo_all_changes->setVisible(false); //Can't undo changes or change sorting while there in no model
    ui->actionChange_Sorting->setVisible(false);

    about = new AboutWindow(this); //setting up all the additional windows
    about->setWindowTitle("About");

    nutrients = new Nutrients(this);
    nutrients->setWindowTitle("Nutrients");
    cantOpen = new QMessageBox(this);       // setting up errors
    sizeError = new QMessageBox(this);
    headerError = new QMessageBox(this);
    dataError = new QMessageBox(this);
    mealLimitReached = new QMessageBox(this);

    setUpError(cantOpen);
    setUpError(sizeError);
    setUpError(headerError);
    setUpError(dataError);
    setUpError(mealLimitReached);

    setUpSearchShortcut();

    McMenu = new MenuModel(this);

    userMenu = new CategoryFilterProxyModel(this);
    userMenu->setSourceModel(McMenu);

    rcTable = new TableView(this, userMenu); // custom table view that shows contextMenu on right click
    rcTable->setSortabilityForModel(true);
    connectTableSignals();


    ui->Grid->addWidget(rcTable, 5, 0);
    rcTable->setStyleSheet("background-color: rgb(255, 221, 66);");

    logo = new RenderArea(ui->LogoZone);

    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding); //size policy for logo to work properly
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(logo->sizePolicy().hasHeightForWidth());
    logo->setSizePolicy(sizePolicy);

    QHBoxLayout* horLayout = new QHBoxLayout(ui->LogoZone); // creating the logo in a given area
    horLayout->addWidget(logo);
}

void MainWindow::connectTableSignals() const
{
    connect(rcTable, SIGNAL(addItemToMeal(QModelIndex)), this, SLOT(addMealItem(QModelIndex))); // complex task so we send the signal to mainwindow
    connect(rcTable, SIGNAL(removeItemFromModel(QModelIndex)), this, SLOT(removeMenuItem(QModelIndex))); // no to overcomplicate we use mainwindow slot rather then lambda functions
    connect(rcTable, SIGNAL(giveDetails(QModelIndex)), this, SLOT(viewItemNutrients(QModelIndex)));
}

void MainWindow::setUpSearchShortcut()
{
    QShortcut *shortcut = new QShortcut(QKeySequence(Qt::Key_Return), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(on_SearchButton_clicked()));
}

void MainWindow::setUpError(QMessageBox* error)
{
    showError.insert(error, true); // creating a show criteria

    error->setWindowTitle("Error!"); // basic design elements
    error->setIcon(QMessageBox::Warning);

    QCheckBox *showAgain = new QCheckBox("Don't show me this message again"); // creating a checkbox
    error->setCheckBox(showAgain);

    connect(showAgain, &QCheckBox::stateChanged, [this, error](int state)  // connecting it to the show criteria of errors
    {
            if (static_cast<Qt::CheckState>(state) == Qt::CheckState::Checked)
                    showError[error] = false;
    });
}

void MainWindow::executeError(QMessageBox* error, const QString& message)
{
        if(showError[error]) // check is user has ticked the "Don't show again" box
        {
            error->setText(message);
            error->exec();
        }
}


void MainWindow::on_actionLoad_another_menu_triggered()
{
    if (!McMenu->isEmpty())
    {
        QMessageBox override(this); // ask the user through messageBox if they want to override current dataset
        override.setText("Do you want to override the menu or just add new items to it?");
        QPushButton *overrideButton = override.addButton(tr("Override"), QMessageBox::ActionRole);
        QPushButton *addNewButton = override.addButton(tr("Add new items"), QMessageBox::ActionRole);
        override.setDefaultButton(overrideButton);
        override.exec();

        if (override.clickedButton() == overrideButton) // change the parameter of import according to their answer
            McMenu->setOverrideTo(true);
        if (override.clickedButton() == addNewButton)
            McMenu->setOverrideTo(false);
    }

    try
    {
        McMenu->importData();
        rcTable->setSortabilityForModel(true);
        ui->actionUndo_all_changes->setVisible(true); //whenever the model first appears we make all the menu interactions possible
        ui->actionChange_Sorting->setVisible(true);
        userMenu->recieveSetupData(McMenu->categoryColumn, McMenu->nameColumn, McMenu->categories.keys()); // give all the necessay info for the filterModel
        nutrients->setHeaders(McMenu->getHeaders()); //give the nutrien window headers for constructing  properly
    }
    catch (std::runtime_error e) // catch and execute different errors
    {
        executeError(cantOpen, e.what());
    }
    catch (std::out_of_range e)
    {

        executeError(sizeError, e.what());
    }
    catch (std::invalid_argument e)
    {

        executeError(headerError, e.what());
    }
    catch (std::domain_error e)
    {

        executeError(headerError, e.what());
    }
}

void MainWindow::on_actionUndo_all_changes_triggered()
{
    McMenu->refreshData();
    userMenu->resetMode();
    userMenu->addMode(SortFilterMode::BasicSorting);
    userMenu->setNewCategories(McMenu->categories.keys());
    if(ui->actionChange_Sorting->text() == "Enable Sorting")
    {
        ui->actionChange_Sorting->setText("Disable Sorting");
        rcTable->setSortabilityForModel(true);
    }
}

void MainWindow::on_actionAbout_triggered()
{
    about->show(); // open the about window
}

void MainWindow::on_actionChange_Sorting_triggered()
{
    if(ui->actionChange_Sorting->text() == "Disable Sorting")
    {
        ui->actionChange_Sorting->setText("Enable Sorting");
        userMenu->removeMode(SortFilterMode::BasicSorting);
        rcTable->setSortabilityForModel(false);
    }
    else
    {
        ui->actionChange_Sorting->setText("Disable Sorting");
        userMenu->addMode(SortFilterMode::BasicSorting);
        rcTable->setSortabilityForModel(true);
    }
}

void MainWindow::on_SearchButton_clicked()
{
    QString searchRequest = ui->SearchBar->text();
    if (searchRequest.isEmpty())
        userMenu->removeMode(SortFilterMode::WordSearch);
    else
    {
        userMenu->setSearchWord(searchRequest);
        userMenu->addMode(SortFilterMode::WordSearch);
    }
}

QString MainWindow::properTime(int min)
{
    QString result;

    if (min > 1440)
    {
        int days = min / 1440;
        result += QString::number(days);
        if (days == 1 || (days % 10 == 1 && days % 100 / 10 != 1)) //english grammar if  i remember it correctly)
            result += " Day, ";
        else
            result += " Days, ";

        min -= 1440 * days;
    }

    if (min > 60)
    {
        int hours = min / 60;
        result += QString::number(hours);
        if (hours == 1 || hours == 21)
            result += " Hour, ";
        else
            result += " Hours, ";

        min -= 60 * hours;
    }

    result += QString::number(min);
    if (min == 1 || (min % 10 == 1 && min % 100 / 10 != 1))
        result += " Minute";
    else
        result += " Minutes";

    return result;
}

void MainWindow::addMealItem(QModelIndex i)
{
    QString itemName = userMenu->getName(i);
    if(inMeal.contains(itemName))
    {
        QFrame *item = ui->Meal->findChild<QFrame*>(itemName);
        QSpinBox* Qty = item->findChild<QSpinBox*>(itemName + "Qty");

        if (Qty->value() < Qty->maximum())
        {
            Qty->setValue(Qty->value()+ 1);
        }
    }
    else
    {
        int index = ui->MealLayout->count() - 3;
        if(index == mealLimit - 1)
        {
            executeError(mealLimitReached, "Sorry, but you have reached the item limit of a single meal. "
                                        "You can't add any more items without deleting some of the previous ones");
        }
        else
        {
            QFrame* frame = new QFrame(ui->Meal);
            frame->setStyleSheet("background-color: rgb(106, 4, 15);");
            frame->setFixedHeight(54); // to fit fullscreen properly

            QHBoxLayout* itemL = new QHBoxLayout(frame);

            QLabel* name = new QLabel(itemName, frame); //name of item
            name->setMinimumWidth(160);
            name->setMaximumWidth(160);
            name->setWordWrap(true);
            name->setFont(QFont("Calibri", 8));
            name->setStyleSheet("color: rgb(255, 250, 244);");

            itemL->addWidget(name);

            QSpinBox* Qty = new QSpinBox(frame); //spinbox for quantity
            Qty->setValue(1);
            Qty->setMinimum(1);
            Qty->setMaximumWidth(36);
            Qty->setStyleSheet("color: rgb(255, 250, 244);");

            frame->setObjectName(itemName); // for navigational purposes
            Qty->setObjectName(itemName + "Qty");

            QVariant defaultCalories = userMenu->data(userMenu->index(i.row(), McMenu->caloryColumn));
            QLabel* calories = new QLabel(defaultCalories.toString(), frame);
            calories->setFont(QFont("Calibri", 8));
            calories->setStyleSheet("color: rgb(255, 250, 244);");

            // Qlabel doesn't emit any signal so we have to do everything by hand
            ui->TotalCalories->setText(QString::number(ui->TotalCalories->text().toInt() + calories->text().toInt()));
            ui->Answer1->setText(properTime(ui->TotalCalories->text().toInt() / 4.5));
            ui->Answer2->setText(properTime(ui->TotalCalories->text().toInt() / 12));
            ui->Answer3->setText(properTime(ui->TotalCalories->text().toInt() / 30));

            calories->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

            connect(Qty, QOverload<int>::of(&QSpinBox::valueChanged), [this, calories, defaultCalories, index](int value) //change the calorie number if user adds more of the same item
            {
                ui->TotalCalories->setText(QString::number(ui->TotalCalories->text().toInt() - calories->text().toInt())); // first substract the old value
                calories->setText(QString::number(value * defaultCalories.toInt()));
                ui->TotalCalories->setText(QString::number(ui->TotalCalories->text().toInt() + calories->text().toInt())); // then add new ones

                ui->Answer1->setText(properTime(ui->TotalCalories->text().toInt() / 4.5));  //adjust the workout times based on total calories
                ui->Answer2->setText(properTime(ui->TotalCalories->text().toInt() / 12));
                ui->Answer3->setText(properTime(ui->TotalCalories->text().toInt() / 30));

                nutrients->changeQuantity(index, value); //change the Q-ty for counting Nutrients
            });


            itemL->addSpacerItem(new QSpacerItem(32, 20, QSizePolicy::Fixed)); //properly allign all the widgets
            itemL->addWidget(Qty, Qt::AlignLeft);
            itemL->addSpacerItem(new QSpacerItem(6, 20, QSizePolicy::Fixed));
            itemL->addWidget(calories);

            QToolButton* deleteItem = new QToolButton(frame); // button to make the item dissapear from your meal
            deleteItem->setStyleSheet("color: rgb(255, 250, 244);");
            deleteItem->setText("X");

            connect(deleteItem, &QToolButton::clicked, [this, calories, frame, itemName]()
            {
                inMeal.removeOne(itemName);
                nutrients->deleteMealItem(ui->MealLayout->indexOf(frame) - 1); // actual number of element in meal

                ui->TotalCalories->setText(QString::number(ui->TotalCalories->text().toInt() - calories->text().toInt())); // delete calories upon deletion of item from a meal
                if (ui->MealLayout->count() == 4) //together with the last layout there should be 4 items. If there are more, we shouldn't empty the labels
                {
                    ui->TotalCalories->setText("");
                    ui->Answer1->setText("");
                    ui->Answer2->setText("");
                    ui->Answer3->setText("");
                }
                delete frame;
            });

            itemL->addWidget(deleteItem); //properly insert the widget
            frame->setLayout(itemL);
            ui->MealLayout->insertWidget(ui->MealLayout->count() - 2, frame);

            inMeal.append(itemName);
            nutrients->addMealItem(McMenu->getItem(userMenu->mapToSource(i).row()));
        }
    }
}

void MainWindow::removeMenuItem(QModelIndex i)
{
    QVariant category = userMenu->data(userMenu->index(i.row(), McMenu->categoryColumn));
    McMenu->categories[category] -= 1;
    if (McMenu->categories[category] == 0) //if item category is no longer there, delete it
    {
        McMenu->categories.remove(category);

        userMenu->eliminateCategory(category);
    }
    McMenu->removeRow(userMenu->mapToSource(i).row());
}

void MainWindow::viewItemNutrients(QModelIndex i) const
{
    nutrients->displayItem(McMenu->getItem(userMenu->mapToSource(i).row()));
    nutrients->show();
}


void MainWindow::on_toolButton_clicked()
{
    if(!inMeal.isEmpty())
    {
        nutrients->displayMeal();
        nutrients->show();
    }
}

void MainWindow::on_actionSave_Meal_data_triggered()
{
    if (inMeal.isEmpty())
        return;
    QString path = QFileDialog::getSaveFileName();
    QFile saveFile(path);
    if(!saveFile.open(QFile::WriteOnly | QFile::Text))
        executeError(cantOpen, "Not able to open the file from the path");

    QTextStream out(&saveFile);
    out << "Qty;";
    int columns = McMenu->columnCount();
    for (int j = 0; j < columns; ++j)
    {
        out << McMenu->headerData(j, Qt::Horizontal).toString();
        if (j != columns - 1)
            out << ';';
    }
    out << '\n';

    const QList<QList<QVariant>>* meal = nutrients->getMealItems();
    for (const QList<QVariant>& item : *meal)
    {
        QFrame *it = ui->Meal->findChild<QFrame*>(item.at(McMenu->nameColumn).toString());
        QSpinBox* Qty = it->findChild<QSpinBox*>(item.at(McMenu->nameColumn).toString() + "Qty");
        out << Qty->value() <<';';
        for(int i =0; i < item.size(); ++i)
        {
            out << item.at(i).toString();
            if(i != item.size() - 1)
                out << ';';
        }
        out <<'\n';
    }
}

MainWindow::~MainWindow()
{
    delete nutrients;
    delete about;

    delete McMenu;
    delete userMenu;
    delete logo;
    delete rcTable;

    delete cantOpen;
    delete sizeError;
    delete headerError;
    delete dataError;

    delete ui;
}
