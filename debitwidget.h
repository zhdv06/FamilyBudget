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
    Ui::DebitWidget *ui;
    int _index;
};

#endif // DEBITWIDGET_H
