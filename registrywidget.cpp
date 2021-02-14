#include "registrywidget.h"
#include "ui_registrywidget.h"

RegistryWidget::RegistryWidget(QWidget *parent) :
    IWidget(parent),
    ui(new Ui::RegistryWidget)
{
    ui->setupUi(this);
}

RegistryWidget::~RegistryWidget()
{
    delete ui;
}

void RegistryWidget::addRecord()
{
    qobject_cast<IWidget*>(ui->tabWidget->currentWidget())->addRecord();
}

void RegistryWidget::removeRecord()
{
    qobject_cast<IWidget*>(ui->tabWidget->currentWidget())->removeRecord();
}
