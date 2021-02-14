#ifndef DEBITWIDGET_H
#define DEBITWIDGET_H

#include <QWidget>
#include "iwidget.h"

namespace Ui {
class DebitWidget;
}

class DebitWidget : public IWidget
{
    Q_OBJECT

public:
    explicit DebitWidget(QWidget *parent = nullptr);
    ~DebitWidget();

public slots:
    void addRecord() override;
    void removeRecord() override;

private:
    Ui::DebitWidget *ui;
};

#endif // DEBITWIDGET_H
