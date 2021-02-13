#ifndef CATALOGWIDGET_H
#define CATALOGWIDGET_H

#include <QWidget>

namespace Ui {
class CatalogWidget;
}

class QSqlRelationalTableModel;

class CatalogWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CatalogWidget(QWidget *parent = nullptr);
    ~CatalogWidget();
    void setTables(const QStringList &tables);
    void addRecord();
    void removeRecord();

private slots:
    void setTable(const QString &tableName);

private:
    Ui::CatalogWidget *ui;
    QSqlRelationalTableModel *_model;
};

#endif // CATALOGWIDGET_H
