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
    void confirmRecord() override;
    void cancelRecord() override;

signals:
    void error(const QString &message);
    void info(const QString &message);
    void changeStatusUpdated(bool status);

private slots:
    void selectRecord();
    int countRecords();
    void updateButtons();

private:
    Ui::CreditWidget *ui;
    int _index;
};

#endif // CREDITWIDGET_H
