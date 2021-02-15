#ifndef CATALOGWIDGET_H
#define CATALOGWIDGET_H

#include <QWidget>
#include "iwidget.h"

namespace Ui {
class CatalogWidget;
}

class QSqlRelationalTableModel;
class QItemSelectionModel;

class CatalogWidget : public IWidget
{
    Q_OBJECT

public:
    explicit CatalogWidget(QWidget *parent = nullptr);
    ~CatalogWidget();
    void setTables(const QStringList &tables);
    void init() override;
    void reset() override;
    void addRecord() override;
    void removeRecord() override;
    void confirmRecord() override;
    void cancelRecord() override;

signals:
    void error(const QString &message);
    void info(const QString &message);
    void removalAllowed(bool allowed);
    void changeStatusUpdated(bool status);

private slots:
    void setTable(const QString &tableName);

private:
    Ui::CatalogWidget *ui;
    QSqlRelationalTableModel *_model;
    int _selectedRow;
};

#endif // CATALOGWIDGET_H
