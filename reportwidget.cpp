#include <QFile>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include "reportwidget.h"
#include "ui_reportwidget.h"

ReportWidget::ReportWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ReportWidget)
{
    ui->setupUi(this);
    connect(ui->pushButton, &QPushButton::clicked,
            this, &ReportWidget::showReport);
}

ReportWidget::~ReportWidget()
{
    delete ui;
}

void ReportWidget::showReport()
{
    QStringList header;
    QList<QStringList> results;

    QSqlQuery personQuery;
    //Person
    personQuery.exec("SELECT DISTINCT Person FROM Credit;");
    if (personQuery.lastError().type() == QSqlError::NoError)
    {
        header.append("Person");
        QSqlQuery productCategoryQuery;
        while (personQuery.next())
        {
            QStringList record;
            const QString &personValue = personQuery.value(0).toString();
            record.append(personValue);

            productCategoryQuery.exec("SELECT Name FROM ProductCategory");
            if (productCategoryQuery.lastError().type() == QSqlError::NoError)
            {
                QSqlQuery sumQuery;
                while (productCategoryQuery.next())
                {
                    const QString &productCategoryValue = productCategoryQuery.value(0).toString();
                    if (!header.contains(productCategoryValue))
                        header.append(productCategoryValue);
                    sumQuery.prepare("SELECT SUM(Value) "
                                     "FROM (SELECT Value FROM (SELECT * FROM Credit WHERE Date >= :beginDate AND Date <= :endDate) "
                                     "WHERE ProductCategory = :productCategory AND Person = :person);");
                    sumQuery.bindValue(":beginDate", ui->beginDateEdit->date().toString("yyyy-MM-dd"));
                    sumQuery.bindValue(":endDate", ui->endDateEdit->date().toString("yyyy-MM-dd"));
                    sumQuery.bindValue(":productCategory", productCategoryQuery.value(0).toString());
                    sumQuery.bindValue(":person", personQuery.value(0).toString());
                    sumQuery.exec();

                    if (productCategoryQuery.lastError().type() == QSqlError::NoError && sumQuery.next())
                        record.append(sumQuery.value(0).toString());
                }
                if (!header.contains("Sum"))
                    header.append("Sum");

                double sum = 0;
                for (int i = 1; i < record.size(); i++)
                    sum += record.at(i).toDouble();
                record.append(QString::number(sum));
                results.append(record);
            }
        }

        QStringList record;
        record.append("Total");
        for (int i = 1; i < header.size(); i++)
        {
            double sum = 0;
            for (int j = 0; j < results.size(); j++)
                sum += results.at(j).at(i).toDouble();
            record.append(QString::number(sum));
        }
        results.append(record);
        results.prepend(header);
    }

    for (const QStringList& list: results)
        qDebug() << list;

    writeFile(results);



    emit finished();
}

void ReportWidget::writeFile(const QList<QStringList>& results)
{
    QString text =
        "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">"
        "<html>"
        "<head>"
        "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\">"
        "<title>Отчет</title>"
        "</head>"
        "<body>"
        "<table>";

    for (const QStringList& row: results)
    {
        text += "<tr>";
        for (const QString& value: row)
        {
            text += "<td>";
            text += value;
            text += "</td>";
        }
        text += "</tr>";
    }
    text += "</table></body></html>";

    QFile file("c:/Users/Denis/QtProjects/TestTaskQt/db/1.html");
    file.open(QIODevice::WriteOnly);
    file.write(text.toUtf8());
    file.close();
}
