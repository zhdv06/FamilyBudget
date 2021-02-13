#include <QSqlRelationalTableModel>
#include <QSqlRelation>
#include <QSqlRelationalDelegate>
#include "catalogwidget.h"
#include "ui_catalogwidget.h"

CatalogWidget::CatalogWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CatalogWidget)
{
    ui->setupUi(this);
    connect(ui->tableNameBox, &QComboBox::currentTextChanged,
            this, &CatalogWidget::setTable);
    _model = new QSqlRelationalTableModel(this);
}

CatalogWidget::~CatalogWidget()
{
    delete ui;
}

void CatalogWidget::setTables(const QStringList &tables)
{
    ui->tableNameBox->addItems(tables);
}

void CatalogWidget::addRecord()
{

}

void CatalogWidget::removeRecord()
{

}

void CatalogWidget::setTable(const QString &tableName)
{
    _model = new QSqlRelationalTableModel(this);
    _model->setTable(tableName);
    _model->setEditStrategy(QSqlRelationalTableModel::OnFieldChange);

    if (tableName == "Person")
        _model->setRelation(3, QSqlRelation("Sex", "Id", "Name"));

    _model->select();

//    _model->setHeaderData(0, Qt::Horizontal, QObject::tr("Id"));
//    _model->setHeaderData(1, Qt::Horizontal, QObject::tr("FullName"));
//    _model->setHeaderData(2, Qt::Horizontal, QObject::tr("Age"));
//    _model->setHeaderData(3, Qt::Horizontal, QObject::tr("Sex"));


    ui->tableView->setModel(_model);
    ui->tableView->setItemDelegate(new QSqlRelationalDelegate(ui->tableView));
    ui->tableView->hideColumn(0);
//        _model->select();
}
