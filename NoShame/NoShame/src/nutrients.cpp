#include "Nutrients.h"
#include "ui_Nutrients.h"

#include <QFormLayout>

///TODO: q-ty stuff

Nutrients::Nutrients(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Nutrients)
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

Nutrients::~Nutrients()
{
    delete ui;
}

void Nutrients::setHeaders(const QList<QString>& h)
{
    headers = h;
}

void Nutrients::clearLayout()
{
    while (ui->Details->count() > 0)
    {
        QLayoutItem *item = ui->Details->takeAt(0);
        delete item->widget();
        delete item;
    }
}

void Nutrients::displayItem(const QList<QVariant>& item)
{
   if (dataShown)
       clearLayout();

   for (int it = 0; it < item.size(); ++it)
   {
       if(headers[it] != "Item")
       {
           QLabel* left = new QLabel(headers[it] + ":");
           left->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding)); // to fully fit the layout
           left->setFont(QFont("Calibri", 9));
           left->setStyleSheet("color: rgb(255, 250, 244);");
           QLabel* right = new QLabel(item[it].toString());
           right->setFont(QFont("Calibri", 9));
           right->setStyleSheet("color: rgb(255, 250, 244);");
           ui->Details->addRow(left, right);
       }
       else
       {
           ui->ItemName->setText(item[it].toString());
       }
   }
   dataShown = ShowingItem;
}

void Nutrients::displayMeal()
{
    if (dataShown)
        clearLayout();

    ui->ItemName->setText("Your Meal");

    for (int it = 0; it < headers.size(); ++it)
    {
        if(headers.at(it) != "Item" && headers.at(it) != "Category" && meal.at(0).at(it) == meal.at(0).at(it).toDouble()) //strings, names and categories don't add properly
        {
            QVariant sum = 0;
            for (int i = 0; i < meal.size(); ++i)
            {
                sum = sum.toDouble() + meal.at(i).at(it).toDouble() * quantities.at(i);
            }
            QLabel* left = new QLabel(headers[it] + ":");
            left->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding)); // to fully fit the layout
            left->setFont(QFont("Calibri", 9));
            left->setStyleSheet("color: rgb(255, 250, 244);");
            QLabel* right = new QLabel(sum.toString());
            right->setFont(QFont("Calibri", 9));
            right->setStyleSheet("color: rgb(255, 250, 244);");
            ui->Details->addRow(left, right);
        }
    }
    dataShown = ShowingMeal;
}

void Nutrients::addMealItem(const QList<QVariant>& item)
{
   meal.append(item);
   quantities.append(1);

   if(this->isVisible() && (dataShown & ShowingMeal)) //if currently the window is open and user has been loking at data for meal update it with any change
      displayMeal();
}

void Nutrients::deleteMealItem(int index)
{
   meal.removeAt(index);
   quantities.removeAt(index);

   if(this->isVisible() && (dataShown & ShowingMeal)) //if currently the window is open and user has been loking at data for meal update it with any change
      displayMeal();
}

void Nutrients::changeQuantity(unsigned short index, unsigned short value)
{
   quantities[index] = value;

   if(this->isVisible() && (dataShown & ShowingMeal)) //if currently the window is open and user has been loking at data for meal update it with any change
      displayMeal();
}
