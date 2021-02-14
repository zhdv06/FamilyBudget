#include "registrywidget.h"
#include "ui_registrywidget.h"

RegistryWidget::RegistryWidget(QWidget *parent) :
    IWidget(parent),
    ui(new Ui::RegistryWidget)
{
    ui->setupUi(this);
    connect(ui->creditTab, &CreditWidget::error,
            this, &RegistryWidget::error);
}

RegistryWidget::~RegistryWidget()
{
    delete ui;
}

void RegistryWidget::init()
{
    for (int i = 0; i < ui->tabWidget->count(); i++)
        qobject_cast<IWidget*>(ui->tabWidget->widget(i))->init();
}

void RegistryWidget::reset()
{
    for (int i = 0; i < ui->tabWidget->count(); i++)
        qobject_cast<IWidget*>(ui->tabWidget->widget(i))->reset();
}

void RegistryWidget::addRecord()
{
    qobject_cast<IWidget*>(ui->tabWidget->currentWidget())->addRecord();
}

void RegistryWidget::removeRecord()
{
    qobject_cast<IWidget*>(ui->tabWidget->currentWidget())->removeRecord();
}

void RegistryWidget::updateData()
{

}
