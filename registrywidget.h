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
    void init() override;
    void reset() override;
    void addRecord() override;
    void removeRecord() override;
    void confirmRecord() override;
    void cancelRecord() override;
    void updateData();

signals:
    void error(const QString &message);
    void info(const QString &message);
    void changeStatusUpdated(bool status);

private:
    Ui::RegistryWidget *ui;
};

#endif // REGISTRYWIDGET_H
