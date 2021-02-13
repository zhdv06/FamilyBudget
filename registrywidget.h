#ifndef REGISTRYWIDGET_H
#define REGISTRYWIDGET_H

#include <QWidget>

namespace Ui {
class RegistryWidget;
}

class RegistryWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RegistryWidget(QWidget *parent = nullptr);
    ~RegistryWidget();

private:
    Ui::RegistryWidget *ui;
};

#endif // REGISTRYWIDGET_H
