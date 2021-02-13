#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include "database.h"

Database::Database(QObject *parent) : QObject(parent)
{

}

void Database::create(const QString &fileName)
{
    _db = QSqlDatabase::addDatabase("QSQLITE");
    _db.setDatabaseName(fileName);
    if (_db.open())
    {
        QSqlQuery query;
        query.exec("CREATE TABLE Sex ("
                        "Id   INTEGER NOT NULL,"
                        "Name VARCHAR(80) NOT NULL,"
                        "PRIMARY KEY (Id)"
                    ");");
        query.exec("CREATE TABLE Person ("
                       "Id   INTEGER NOT NULL,"
                       "FullName VARCHAR(80) NOT NULL,"
                       "Age INTEGER NOT NULL,"
                       "SexId INTEGER NOT NULL,"
                       "PRIMARY KEY (Id),"
                       "FOREIGN KEY (SexId) REFERENCES Sex(Id)"
                       "   ON UPDATE NO ACTION"
                       "   ON DELETE NO ACTION"
                   ");");
        query.exec("CREATE TABLE ProductCategory ("
                       "Id   INTEGER NOT NULL,"
                       "Name VARCHAR(80) NOT NULL,"
                       "PRIMARY KEY (Id)"
                   ");");
        query.exec("CREATE TABLE Product ("
                       "Id   INTEGER NOT NULL,"
                       "Name VARCHAR(80) NOT NULL,"
                       "ProductCategoryId INTEGER NOT NULL,"
                       "PRIMARY KEY (Id),"
                       "FOREIGN KEY (ProductCategoryId)"
                       "REFERENCES ProductCategory(Id)"
                       "   ON UPDATE NO ACTION"
                       "   ON DELETE NO ACTION"
                   ");");
        query.exec("CREATE TABLE RevenueSource ("
                       "Id   INTEGER NOT NULL,"
                       "Name VARCHAR(80) NOT NULL,"
                       "PRIMARY KEY (Id)"
                   ");");
        query.exec("CREATE TABLE Debit ("
                       "Id   INTEGER NOT NULL,"
                       "PersonId INTEGER NOT NULL,"
                       "Date DATE NOT NULL,"
                       "Value DOUBLE NOT NULL,"
                       "RevenueSourceId INTEGER NOT NULL,"
                       "Comment VARCHAR(128),"
                       "PRIMARY KEY (Id),"
                       "FOREIGN KEY (PersonId) REFERENCES Person(Id)"
                       "   ON UPDATE NO ACTION"
                       "   ON DELETE NO ACTION"
                       "FOREIGN KEY (RevenueSourceId) REFERENCES RevenueSource(Id)"
                       "   ON UPDATE NO ACTION"
                       "   ON DELETE NO ACTION"
                   ");");
        query.exec("CREATE TABLE Credit ("
                       "Id   INTEGER NOT NULL,"
                       "PersonId INTEGER NOT NULL,"
                       "Date DATE NOT NULL,"
                       "Value DOUBLE NOT NULL,"
                       "ProductId INTEGER NOT NULL,"
                       "ProductCategoryId INTEGER NOT NULL,"
                       "Comment VARCHAR(128),"
                       "PRIMARY KEY (Id),"
                       "FOREIGN KEY (ProductId) REFERENCES Product(Id)"
                       "   ON UPDATE NO ACTION"
                       "   ON DELETE NO ACTION"
                       "FOREIGN KEY (ProductCategoryId) REFERENCES ProductCategory(Id)"
                       "   ON UPDATE NO ACTION"
                       "   ON DELETE NO ACTION"
                   ");");


        emit statusUpdated(DS_Connected);
    }
    else
    {
        emit error(_db.lastError().text());
    }
}

void Database::connect_(const QString &fileName)
{
    _db = QSqlDatabase::addDatabase("QSQLITE"/*, "FamilyBudget"*/);
    _db.setDatabaseName(fileName);
    if (_db.open())
    {
        emit statusUpdated(DS_Connected);
    }
    else
    {
        emit error(_db.lastError().text());
    }
}

void Database::disconnect_()
{
    _db.close();
//    _db = QSqlDatabase();
//    QSqlDatabase::removeDatabase("FamilyBudget");
    emit statusUpdated(DS_Disconnected);
}

void Database::processRequest(RequestType rt)
{
    switch (rt)
    {
    case RT_Tables:
        emit response(rt, QVariant(_db.tables()));
        break;
    }
}
