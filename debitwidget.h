#ifndef DEBITWIDGET_H
#define DEBITWIDGET_H

#include <QWidget>
#include "iwidget.h"

namespace Ui {
class DebitWidget;
}

class QSqlRelationalTableModel;
class QItemSelectionModel;
class QDataWidgetMapper;

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

signals:
    void error(const QString &message);

private slots:
    void updateButtons(int row);

private:
    Ui::DebitWidget *ui;
    QSqlRelationalTableModel *_model;
    QItemSelectionModel *_selectionModel;
    QDataWidgetMapper *_mapper;
    int _personIndex;
    int _revenueSourceIndex;
};

#endif // DEBITWIDGET_H
