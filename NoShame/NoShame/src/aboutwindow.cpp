#include "aboutwindow.h"
#include "ui_aboutwindow.h"

AboutWindow::AboutWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutWindow)
{
    ui->setupUi(this);

    setFixedSize(geometry().width(), geometry().height());

    logo = new RenderArea(this);

    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding); //size policy for logo to work properly
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(logo->sizePolicy().hasHeightForWidth());
    logo->setSizePolicy(sizePolicy);

    ui->LogoZone->addWidget(logo);
}

AboutWindow::~AboutWindow()
{
    delete ui;
    delete logo;
}
