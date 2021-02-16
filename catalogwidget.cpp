#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlRelationalTableModel>
#include <QSqlRelation>
#include <QSqlRelationalDelegate>
#include <QItemSelectionModel>
#include "catalogwidget.h"
#include "ui_catalogwidget.h"

CatalogWidget::CatalogWidget(QWidget *parent) :
    IWidget(parent),
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

void CatalogWidget::init()
{
    QStringList catalogs{"Sex", "Person", "ProductCategory", "Product", "RevenueSource"};
    QSqlQuery query;
    query.exec("SELECT name FROM sqlite_master WHERE type='table' AND name NOT LIKE 'sqlite_%';");
    while (query.next())
    {
        QString tableName(query.value(0).toString());
        if (catalogs.contains(tableName))
            ui->tableNameBox->addItem(tableName);
    }
}

void CatalogWidget::reset()
{
    ui->tableNameBox->clear();
    ui->tableView->reset();
    delete _model;
}

void CatalogWidget::addRecord()
{
    _model->insertRow(0);
}

void CatalogWidget::removeRecord()
{
    _model->removeRow(_selectedRow);
    emit changeStatusUpdated(true);
}

void CatalogWidget::confirmRecord()
{
    _model->submitAll();
    emit changeStatusUpdated(false);
}

void CatalogWidget::cancelRecord()
{
    _model->select();
    emit changeStatusUpdated(false);
}

void CatalogWidget::setTable(const QString &tableName)
{
    _model = new QSqlRelationalTableModel(this);
    _model->setTable(tableName);
    _model->setEditStrategy(QSqlRelationalTableModel::OnManualSubmit);
    connect(_model, &QSqlRelationalTableModel::dataChanged,
            [this](const QModelIndex &/*topLeft*/, const QModelIndex &/*bottomRight*/, const QVector<int> &/*roles*/) {
                emit changeStatusUpdated(true);
        });

    if (tableName == "Person")
    {
        _model->setRelation(3, QSqlRelation("Sex", "Id", "Name"));
        _model->setHeaderData(0, Qt::Horizontal, tr("Id"));
        _model->setHeaderData(1, Qt::Horizontal, tr("FullName"));
        _model->setHeaderData(2, Qt::Horizontal, tr("Age"));
        _model->setHeaderData(3, Qt::Horizontal, tr("Sex"));
    }

    if (tableName == "Product")
    {
        _model->setRelation(2, QSqlRelation("ProductCategory", "Id", "Name"));
        _model->setHeaderData(0, Qt::Horizontal, tr("Id"));
        _model->setHeaderData(1, Qt::Horizontal, tr("Product"));
        _model->setHeaderData(2, Qt::Horizontal, tr("ProductCategory"));
    }

    _model->select();

    ui->tableView->setModel(_model);
    ui->tableView->setItemDelegate(new QSqlRelationalDelegate(ui->tableView));
    ui->tableView->hideColumn(0);

    connect(ui->tableView->selectionModel(), &QItemSelectionModel::selectionChanged,
            [this](const QItemSelection &selected, const QItemSelection &/*deselected*/) {
                bool isSelected(selected.indexes().count() > 0);
                _selectedRow = isSelected ? selected.indexes().at(0).row(): -1;
                emit removalAllowed(isSelected);
        });
}
