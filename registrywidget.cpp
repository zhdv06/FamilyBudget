#include "registrywidget.h"
#include "ui_registrywidget.h"

RegistryWidget::RegistryWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RegistryWidget)
{
    ui->setupUi(this);
}

RegistryWidget::~RegistryWidget()
{
    delete ui;
}
