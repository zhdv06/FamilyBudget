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
    void init() override;
    void reset() override;
    void addRecord() override;
    void removeRecord() override;

signals:
    void error(const QString &message);

private:
    Ui::CreditWidget *ui;
};

#endif // CREDITWIDGET_H
