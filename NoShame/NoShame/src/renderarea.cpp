#include "renderarea.h"

#include <QPainter>

RenderArea::RenderArea(QWidget* parent)
    : QWidget(parent)
{

}


void RenderArea::paintEvent(QPaintEvent* /* event */)
{
    QPainter painter(this);

    int side = std::min(width(), height());

    QColor niceWhite(255, 250, 244); //our palette
    QColor yellow(255, 186, 8);

    qreal xc = width() * 0.5;
    qreal yc = height() * 0.5;

    painter.translate(xc, yc);

    painter.setPen(Qt::GlobalColor::transparent);

    painter.setBrush(niceWhite);
    painter.drawEllipse(- side * 5 / 12, - side * 5 / 12, side * 5 /6., side * 5/6.);

    painter.rotate(-15); //start tilted dumbell

    painter.setBrush(Qt::GlobalColor::black);
    painter.drawRect(- side / 4 - side / 80, - side / 40, side / 2, side / 20);

    painter.drawRect(- side / 4 - side / 40 - side / 100 - side / 80, - 5 * side / 40, side / 40, 5 * side / 20);
    painter.drawRect(- side / 4 - side / 20 - side / 50 - side / 80, - 3 * side / 40, side / 40, 3 * side / 20);
    painter.drawRect(- side / 4 - 3 * side / 40 - 3 * side / 100 - side / 80, - side / 40, side / 40, side / 20);

    painter.drawRect(side / 4 + side / 100 - side / 80, - 5 * side / 40, side / 40, 5 * side / 20);
    painter.drawRect(side / 4 + side / 40 + side / 50 - side / 80, - 3 * side / 40, side / 40, 3 * side / 20);
    painter.drawRect(side / 4 + side / 20 + 3* side / 100 - side / 80, - side / 40, side / 40, side / 20);

    painter.rotate(15); //end tilted dumbell

//    painter.setPen(Qt::GlobalColor::black);       /// used for cenralizing everything when making
//    painter.drawLine(-side / 2, 0, side / 2, 0);
//    painter.drawLine(0, -side / 2, 0 , side / 2);

    int penWidth = width() / 20;
    QPen mPen(painter.brush(), penWidth);
    mPen.setColor(yellow);
    painter.setPen(mPen); //m

    QRectF left(-side / 5, - 5 * side / 32, side / 5, side / 2);
    QRectF right(0, - 5 * side / 32, side / 5, side / 2);
    painter.drawArc(left, -10 , 190 * 16);
    painter.drawArc(right, - 10 * 16 , 170 * 16);

    painter.setPen(Qt::GlobalColor::transparent);

    painter.rotate(-15);

    painter.drawRect(- side / 10, - side / 40, side / 5, side / 20);

    painter.rotate(15);

    painter.scale(1.2, 1.2); // 6/5 initially had other figure but through the creative process decided to leave logo in circle, hence all the weird scales
}
