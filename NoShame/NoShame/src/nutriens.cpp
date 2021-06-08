#include "nutriens.h"
#include "ui_nutriens.h"

#include <QFormLayout>

///TODO: q-ty stuff

Nutriens::Nutriens(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Nutriens)
{
    ui->setupUi(this);
    setFixedSize(geometry().width(), geometry().height());

    logo = new RenderArea(ui->Logo);

    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding); //size policy for logo to work properly
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(logo->sizePolicy().hasHeightForWidth());
    logo->setSizePolicy(sizePolicy);

    QHBoxLayout* horLayout = new QHBoxLayout(ui->Logo); // creating the logo in a given area
    horLayout->addWidget(logo);
}

Nutriens::~Nutriens()
{
    delete ui;
}

void Nutriens::setHeaders(const QList<QString>& h)
{
    headers = h;
}

void Nutriens::clearLayout()
{
    while (ui->Details->count() > 0)
    {
        QLayoutItem *item = ui->Details->takeAt(0);
        delete item->widget();
        delete item;
    }
//    int prevSize = ui->Details->rowCount();
//    for (int i = 0; i <= prevSize; ++i)
//    {
//        ui->Details->removeRow(0);
//    }
}

void Nutriens::displayItem(const QList<QVariant>& item)
{
   if (dataShown)
       clearLayout();

   for (int it = 0; it < item.size(); ++it)
   {
       if(headers[it] != "Item")
       {
           QLabel* left = new QLabel(headers[it] + ":");
           left->setFont(QFont("Calibri", 9));
           QLabel* right = new QLabel(item[it].toString());
           right->setFont(QFont("Calibri", 9));
           ui->Details->addRow(left, right);
       }
       else
       {
           ui->ItemName->setText(item[it].toString());
       }
   }
   dataShown = ShowingItem;
}

void Nutriens::displayMeal()
{
    if (dataShown)
        clearLayout();

    ui->ItemName->setText("Your Meal");

    for (int it = 0; it < headers.size(); ++it)
    {
        if(headers[it] != "Item" && headers[it] != "Serving Size" && headers[it] != "Category") //those simply don't aply when talkin to a vast group of items
        {
            QVariant sum = 0;
            for (int i = 0; i < meal.size(); ++i)
            {
                sum = sum.toDouble() + meal.at(i).at(it).toDouble() * quantities.at(i);
            }
            QLabel* left = new QLabel(headers[it] + ":");
            left->setFont(QFont("Calibri", 9));
            QLabel* right = new QLabel(sum.toString());
            right->setFont(QFont("Calibri", 9));
            ui->Details->addRow(left, right);
        }
    }
    dataShown = ShowingMeal;
}

void Nutriens::addMealItem(const QList<QVariant>& item)
{
   meal.append(item);
   quantities.append(1);

   if(this->isVisible() && (dataShown & ShowingMeal)) //if currently the window is open and user has been loking at data for meal update it with any change
      displayMeal();
}

void Nutriens::deleteMealItem(int index)
{
   meal.removeAt(index);
   quantities.removeAt(index);

   if(this->isVisible() && (dataShown & ShowingMeal)) //if currently the window is open and user has been loking at data for meal update it with any change
      displayMeal();
}

void Nutriens::changeQuantity(unsigned short index, unsigned short value)
{
   quantities[index] = value;

   if(this->isVisible() && (dataShown & ShowingMeal)) //if currently the window is open and user has been loking at data for meal update it with any change
      displayMeal();
}
