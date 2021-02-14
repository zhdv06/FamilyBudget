#ifndef REGISTRYWIDGET_H
#define REGISTRYWIDGET_H

#include <QWidget>
#include "iwidget.h"

namespace Ui {
class RegistryWidget;
}

class RegistryWidget : public IWidget
{
    Q_OBJECT

public:
    explicit RegistryWidget(QWidget *parent = nullptr);
    ~RegistryWidget();

public slots:
    void addRecord() override;
    void removeRecord() override;

private:
    Ui::RegistryWidget *ui;
};

#endif // REGISTRYWIDGET_H
