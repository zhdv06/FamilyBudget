#ifndef CATALOGWIDGET_H
#define CATALOGWIDGET_H

#include <QWidget>
#include "iwidget.h"

namespace Ui {
class CatalogWidget;
}

class QSqlRelationalTableModel;

class CatalogWidget : public IWidget
{
    Q_OBJECT

public:
    explicit CatalogWidget(QWidget *parent = nullptr);
    ~CatalogWidget();
    void setTables(const QStringList &tables);
    void addRecord() override;
    void removeRecord() override;

private slots:
    void setTable(const QString &tableName);

private:
    Ui::CatalogWidget *ui;
    QSqlRelationalTableModel *_model;
};

#endif // CATALOGWIDGET_H
