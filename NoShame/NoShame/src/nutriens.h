#ifndef NUTRIENS_H
#define NUTRIENS_H

#include <QDialog>
#include "menumodel.h"
#include "renderarea.h"

enum DataShown
{
    IsEmpty,
    ShowingItem,
    ShowingMeal
};


namespace Ui {
class Nutriens;
}

class Nutriens : public QDialog
{
    Q_OBJECT

public:
    explicit Nutriens(QWidget *parent = nullptr);
    ~Nutriens();

    void setHeaders(const QList<QString>& h);

    void displayItem(const QList<QVariant>& item);
    void displayMeal();

    void addMealItem(const QList<QVariant>& item);
    void deleteMealItem(int index);

    void changeQuantity(unsigned short index, unsigned short value);
private:
    Ui::Nutriens *ui;
    RenderArea *logo;

    QList<QList<QVariant>> meal; //it is easy enough so it doesn't require a model
    QList<unsigned short> quantities;

    QList<QString> headers;
    int dataShown = IsEmpty;

    void clearLayout();
};

#endif // NUTRIENS_H
