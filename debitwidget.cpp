#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlRelationalTableModel>
#include <QSqlRelationalDelegate>
#include <QDataWidgetMapper>
#include "debitwidget.h"
#include "ui_debitwidget.h"

DebitWidget::DebitWidget(QWidget *parent) :
    IWidget(parent),
    ui(new Ui::DebitWidget)
{
    ui->setupUi(this);
}

DebitWidget::~DebitWidget()
{
    delete ui;
}

void DebitWidget::init()
{
    qDebug() << "DebitWidget::init()";
    _model = new QSqlRelationalTableModel(this);
    _model->setTable("Debit");
    _model->setEditStrategy(QSqlTableModel::OnFieldChange);

    _personIndex = _model->fieldIndex("PersonId");
    _model->setRelation(_personIndex, QSqlRelation("Person", "Id", "FullName"));

    _revenueSourceIndex = _model->fieldIndex("RevenueSourceId");
    _model->setRelation(_revenueSourceIndex, QSqlRelation("RevenueSource", "Id", "Name"));

    _model->select();

    QSqlTableModel *personModel = _model->relationModel(_personIndex);
    ui->fullNameBox->setModel(personModel);
    ui->fullNameBox->setModelColumn(personModel->fieldIndex("FullName"));

    QSqlTableModel *revenueSourceModel = _model->relationModel(_revenueSourceIndex);
    ui->revenueSourceBox->setModel(revenueSourceModel);
    ui->revenueSourceBox->setModelColumn(revenueSourceModel->fieldIndex("Name"));

    _mapper = new QDataWidgetMapper(this);
    _mapper->setModel(_model);
    _mapper->setItemDelegate(new QSqlRelationalDelegate(this));
    _mapper->addMapping(ui->fullNameBox, _personIndex);
    _mapper->addMapping(ui->dateEdit, _model->fieldIndex("Date"));
    _mapper->addMapping(ui->valueSpinBox, _model->fieldIndex("Value"));
    _mapper->addMapping(ui->revenueSourceBox, _revenueSourceIndex);
    _mapper->addMapping(ui->commentEdit, _model->fieldIndex("Comment"));

    connect(ui->previousButton, &QPushButton::clicked,
            _mapper, &QDataWidgetMapper::toPrevious);
    connect(ui->nextButton, &QPushButton::clicked,
            _mapper, &QDataWidgetMapper::toNext);
    connect(_mapper, &QDataWidgetMapper::currentIndexChanged,
            this, &DebitWidget::updateButtons);

    _mapper->toFirst();
}

void DebitWidget::reset()
{

}

void DebitWidget::addRecord()
{
    qDebug() << "DebitWidget::addRecord()";

    QSqlQuery query;
    query.exec("INSERT INTO Debit (PersonId, Date, Value, RevenueSourceId)"
               "VALUES ((SELECT Id FROM Person ORDER BY Id DESC LIMIT 1),"
                       "'2021-02-07',"
                       "1000.07,"
                       "(SELECT Id FROM RevenueSource ORDER BY Id DESC LIMIT 1));"
    );
    if (query.lastError().type() == QSqlError::NoError)
    {
        _model->select();
        _mapper->toLast();
    }
    else
    {
        emit error(query.lastError().text());
    }
}

void DebitWidget::removeRecord()
{
    //This operation is not allowed!
    emit error("Эта операция запрещена!");
}

void DebitWidget::updateButtons(int row)
{
    ui->previousButton->setEnabled(row > 0);
    ui->nextButton->setEnabled(row < _model->rowCount() - 1);
}
