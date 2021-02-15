#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include "creditwidget.h"
#include "ui_creditwidget.h"

CreditWidget::CreditWidget(QWidget *parent) :
    IWidget(parent),
    ui(new Ui::CreditWidget),
    _index(1)
{
    ui->setupUi(this);
    connect(ui->fullNameEdit, &QLineEdit::textChanged,
            [this](const QString &/*text*/){emit changeStatusUpdated(true);});
    connect(ui->dateEdit, &QDateEdit::dateChanged,
            [this](const QDate &/*date*/){emit changeStatusUpdated(true);});
    connect(ui->valueSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            [this](double /*d*/){emit changeStatusUpdated(true);});
    connect(ui->productEdit, &QLineEdit::textChanged,
            [this](const QString &/*text*/){emit changeStatusUpdated(true);});
    connect(ui->productCategoryEdit, &QLineEdit::textChanged,
            [this](const QString &/*text*/){emit changeStatusUpdated(true);});
    connect(ui->commentEdit, &QLineEdit::textChanged,
            [this](const QString &/*text*/){emit changeStatusUpdated(true);});

    connect(ui->previousButton, &QPushButton::clicked,
            [this](){_index--; selectRecord();});
    connect(ui->nextButton, &QPushButton::clicked,
            [this](){_index++; selectRecord();});
}

CreditWidget::~CreditWidget()
{
    delete ui;
}

void CreditWidget::selectRecord()
{
    qDebug() << "CreditWidget::selectRecord()";
    QSqlQuery query;
    query.prepare("SELECT Person, Date, Value, Product, ProductCategory, Comment FROM Credit WHERE Id = :index;");
    query.bindValue(":index", _index);
    query.exec();
    if (query.lastError().type() == QSqlError::NoError && query.next())
    {
        ui->fullNameEdit->setText(query.value("Person").toString());
        ui->dateEdit->setDate(query.value("Date").toDate());
        ui->valueSpinBox->setValue(query.value("Value").toDouble());
        ui->productEdit->setText(query.value("Product").toString());
        ui->productCategoryEdit->setText(query.value("ProductCategory").toString());
        ui->commentEdit->setText(query.value("Comment").toString());
    }
    updateButtons();
}

int CreditWidget::countRecords()
{
    QSqlQuery query;
    query.exec("SELECT COUNT(*) FROM Credit;");
    return (query.lastError().type() == QSqlError::NoError && query.next())
            ? query.value(0).toInt()
            : -1;
}

void CreditWidget::init()
{
    qDebug() << "CreditWidget::init()";
    selectRecord();
}

void CreditWidget::reset()
{

}

void CreditWidget::addRecord()
{
    qDebug() << "CreditWidget::addRecord()";

    QSqlQuery query;
    query.prepare("INSERT INTO Credit (Person, Date, Value, Product, ProductCategory) "
                  "VALUES ((SELECT Id FROM Person ORDER BY Id DESC LIMIT 1), "
                           ":date, "
                           "1000.00, "
                           "(SELECT Name FROM Product ORDER BY Id DESC LIMIT 1), "
                           "(SELECT Name FROM ProductCategory ORDER BY Id DESC LIMIT 1));"
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

void CreditWidget::removeRecord()
{
    //This operation is not allowed!
    emit error("Эта операция запрещена!");
}

void CreditWidget::confirmRecord()
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
            return;
        }
    }
    else
    {
        emit error(query.lastError().text());
        return;
    }

    query.exec("SELECT Name FROM Product;");
    if (query.lastError().type() == QSqlError::NoError)
    {
        const QString &name = ui->productEdit->text();
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
            return;
        }
    }
    else
    {
        emit error(query.lastError().text());
        return;
    }

    query.exec("SELECT Name FROM ProductCategory;");
    if (query.lastError().type() == QSqlError::NoError)
    {
        const QString &name = ui->productCategoryEdit->text();
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
            return;
        }
    }
    else
    {
        emit error(query.lastError().text());
        return;
    }

    query.prepare("UPDATE Credit "
                  "SET Person=:person, Date=:date, Value=:value, Product=:product, ProductCategory=:productCategory, Comment=:comment "
                  "WHERE Id=:index;");
    query.bindValue(":person", ui->fullNameEdit->text());
    query.bindValue(":date", ui->dateEdit->date().toString("yyyy-MM-dd"));
    query.bindValue(":value", ui->valueSpinBox->value());
    query.bindValue(":product", ui->productEdit->text());
    query.bindValue(":productCategory", ui->productCategoryEdit->text());
    query.bindValue(":comment", ui->commentEdit->text());
    query.bindValue(":index", _index);
    qDebug() << query.exec();
    qDebug() << query.lastQuery();
    if (query.lastError().type() == QSqlError::NoError)
        emit changeStatusUpdated(false);
    else
        emit error(query.lastError().text());
}

void CreditWidget::cancelRecord()
{
    selectRecord();
    emit changeStatusUpdated(false);
}

void CreditWidget::updateButtons()
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
