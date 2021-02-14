#include <QDebug>
#include "debitwidget.h"
#include "ui_debitwidget.h"

DebitWidget::DebitWidget(QWidget *parent) :
    IWidget(parent),
    ui(new Ui::DebitWidget)
{
    ui->setupUi(this);
}

DebitWidget::~DebitWidget()
{
    delete ui;
}

void DebitWidget::addRecord()
{
    qDebug() << "DebitWidget::addRecord()";
}

void DebitWidget::removeRecord()
{
    //This operation is not allowed!
}
