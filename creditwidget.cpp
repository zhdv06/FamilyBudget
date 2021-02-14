#include <QDebug>
#include "creditwidget.h"
#include "ui_creditwidget.h"

CreditWidget::CreditWidget(QWidget *parent) :
    IWidget(parent),
    ui(new Ui::CreditWidget)
{
    ui->setupUi(this);
}

CreditWidget::~CreditWidget()
{
    delete ui;
}

void CreditWidget::addRecord()
{
    qDebug() << "CreditWidget::addRecord()";
}

void CreditWidget::removeRecord()
{
    qDebug() << "CreditWidget::removeRecord()";
}
