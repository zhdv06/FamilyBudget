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
    ui(new Ui::DebitWidget),
    _index(1)
{
    ui->setupUi(this);
    connect(ui->fullNameEdit, &QLineEdit::textChanged,
            [this](const QString &/*text*/){emit changeStatusUpdated(true);});
    connect(ui->dateEdit, &QDateEdit::dateChanged,
            [this](const QDate &/*date*/){emit changeStatusUpdated(true);});
    connect(ui->valueSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            [this](double /*d*/){emit changeStatusUpdated(true);});
    connect(ui->revenueSourceEdit, &QLineEdit::textChanged,
            [this](const QString &/*text*/){emit changeStatusUpdated(true);});
    connect(ui->commentEdit, &QLineEdit::textChanged,
            [this](const QString &/*text*/){emit changeStatusUpdated(true);});

    connect(ui->previousButton, &QPushButton::clicked,
            [this](){_index--; selectRecord();});
    connect(ui->nextButton, &QPushButton::clicked,
            [this](){_index++; selectRecord();});
}

DebitWidget::~DebitWidget()
{
    delete ui;
}

void DebitWidget::selectRecord()
{
    qDebug() << "DebitWidget::selectRecord()";
    QSqlQuery query;
    query.prepare("SELECT Person, Date, Value, RevenueSource, Comment FROM Debit WHERE Id = :index;");
    query.bindValue(":index", _index);
    query.exec();
    if (query.lastError().type() == QSqlError::NoError && query.next())
    {
        ui->fullNameEdit->setText(query.value("Person").toString());
        ui->dateEdit->setDate(query.value("Date").toDate());
        ui->valueSpinBox->setValue(query.value("Value").toDouble());
        ui->revenueSourceEdit->setText(query.value("RevenueSource").toString());
        ui->commentEdit->setText(query.value("Comment").toString());
    }
    updateButtons();
}

int DebitWidget::countRecords()
{
    QSqlQuery query;
    query.exec("SELECT COUNT(*) FROM Debit;");
    return (query.lastError().type() == QSqlError::NoError && query.next())
            ? query.value(0).toInt()
            : -1;
}

void DebitWidget::init()
{
    qDebug() << "DebitWidget::init()";
    selectRecord();
}

void DebitWidget::reset()
{

}

void DebitWidget::addRecord()
{
    qDebug() << "DebitWidget::addRecord()";

    QSqlQuery query;
    query.prepare("INSERT INTO Debit (Person, Date, Value, RevenueSource) "
                  "VALUES ((SELECT Id FROM Person ORDER BY Id DESC LIMIT 1), "
                           ":date, "
                           "1000.00, "
                           "(SELECT Name FROM RevenueSource ORDER BY Id DESC LIMIT 1));"
    );
    query.bindValue(":date", QDate::currentDate().toString("yyyy-MM-dd"));
    query.exec();
    qDebug() << query.lastQuery();
    if (query.lastError().type() == QSqlError::NoError)
    {
        emit info("Запись успешно добавлена.");
        int count = countRecords();
        if (count != -1)
        {
            _index = count;
            selectRecord();
            updateButtons();
        }
        else
            emit error("Не удалось отобразить запись.");
    }
    else
        emit error(query.lastError().text());
}

void DebitWidget::removeRecord()
{
    //This operation is not allowed!
    emit error("Эта операция запрещена!");
}

void DebitWidget::confirmRecord()
{
    QSqlQuery query;
    query.exec("SELECT FullName FROM Person;");
    if (query.lastError().type() == QSqlError::NoError)
    {
        const QString &fullName = ui->fullNameEdit->text();
        bool result = false;
        while (query.next())
            if (query.value(0).toString() == fullName)
            {
                result = true;
                break;
            }
        if (!result)
        {
            emit error(QString("Значение \"%1\" отсутствует в справочнике.").arg(fullName));
            //удалить добавленную строку!!!
            return;
        }
    }
    else
    {
        emit error(query.lastError().text());
        return;
    }

    query.exec("SELECT Name FROM RevenueSource;");
    if (query.lastError().type() == QSqlError::NoError)
    {
        const QString &name = ui->revenueSourceEdit->text();
        bool result = false;
        while (query.next())
            if (query.value(0).toString() == name)
            {
                result = true;
                break;
            }
        if (!result)
        {
            emit error(QString("Значение \"%1\" отсутствует в справочнике.").arg(name));
            //удалить добавленную строку
            return;
        }
    }
    else
    {
        emit error(query.lastError().text());
        return;
    }

    query.prepare("UPDATE Debit"
                  "SET Person = ':person' Date = ':date' Value = ':value' RevenueSource = ':revenueSource' Comment = ':comment'"
                  "WHERE Id = :index;");
    query.bindValue(":person", ui->fullNameEdit->text());
    query.bindValue(":date", ui->dateEdit->date().toString("yyyy-MM-dd"));
    query.bindValue(":value", ui->valueSpinBox->value());
    query.bindValue(":revenueSource", ui->revenueSourceEdit->text());
    query.bindValue(":comment", ui->commentEdit->text());
    query.exec();
    if (query.lastError().type() == QSqlError::NoError)
        emit changeStatusUpdated(false);
    else
        emit error(query.lastError().text());
}

void DebitWidget::cancelRecord()
{
    selectRecord();
    emit changeStatusUpdated(false);
}

void DebitWidget::updateButtons()
{
    int count = countRecords();
    if (count != -1)
    {
        ui->previousButton->setEnabled(_index > 1);
        ui->nextButton->setEnabled(_index < count);
    }
    else
        emit error("Не удалось получить количество записей.");
}
