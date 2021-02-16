#include <QDesktopServices>
#include <QUrl>
#include <QFile>
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
        header.append("ФИО");
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
                if (!header.contains("Сумма"))
                    header.append("Сумма");

                double sum = 0;
                for (int i = 1; i < record.size(); i++)
                    sum += record.at(i).toDouble();
                record.append(QString::number(sum));
                results.append(record);
            }
        }

        QStringList record;
        record.append("Итого");
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

    writeFile(results);
    emit finished();
}

void ReportWidget::writeFile(const QList<QStringList>& results)
{
    QString inFileName(QString("%1/template.html").arg(QApplication::applicationDirPath()));
    QString outFileName(QString("%1/report.html").arg(QApplication::applicationDirPath()));
    QFile fin(inFileName);
    QFile fout(outFileName);
    if (!fin.open(QIODevice::ReadOnly) || !fout.open(QIODevice::WriteOnly))
    {
        emit error("Не удалось записать отчет.");
        return;
    }

    QString beginDateMark("<!--begin_date-->");
    QString endDateMark("<!--end_date-->");
    QString tableMark("<!--table_mark-->");

    while (!fin.atEnd())
    {
        QString line = fin.readLine();
        if (line.contains(beginDateMark) && line.contains(endDateMark))
        {
            line.replace(beginDateMark, ui->beginDateEdit->date().toString("dd.MM.yyyy"));
            line.replace(endDateMark, ui->endDateEdit->date().toString("dd.MM.yyyy"));
            fout.write(line.toUtf8());
        }
        else if (line.contains(tableMark))
        {
            QString text("<table border=1>");
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
            text += "</table>";
            line.replace(tableMark, text);
            fout.write(line.toUtf8());
        }
        else
            fout.write(line.toUtf8());
    }

    QDesktopServices::openUrl(QUrl(QString("file:///%1").arg(outFileName), QUrl::TolerantMode));

    fin.close();
    fout.close();
}
