#ifndef CREDITWIDGET_H
#define CREDITWIDGET_H

#include <QWidget>
#include "iwidget.h"

namespace Ui {
class CreditWidget;
}

class CreditWidget : public IWidget
{
    Q_OBJECT

public:
    explicit CreditWidget(QWidget *parent = nullptr);
    ~CreditWidget();

public slots:
    void addRecord() override;
    void removeRecord() override;

private:
    Ui::CreditWidget *ui;
};

#endif // CREDITWIDGET_H
