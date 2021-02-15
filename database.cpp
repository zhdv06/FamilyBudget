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
                        "Id   INTEGER PRIMARY KEY AUTOINCREMENT,"
                        "Name VARCHAR(80) NOT NULL"
                    ");");
        query.exec("CREATE TABLE Person ("
                       "Id   INTEGER PRIMARY KEY AUTOINCREMENT,"
                       "FullName VARCHAR(80) NOT NULL,"
                       "Age INTEGER NOT NULL,"
                       "SexId INTEGER NOT NULL,"
                       "FOREIGN KEY (SexId) REFERENCES Sex(Id)"
                       "   ON UPDATE NO ACTION"
                       "   ON DELETE NO ACTION"
                   ");");
        query.exec("CREATE TABLE ProductCategory ("
                       "Id   INTEGER PRIMARY KEY AUTOINCREMENT,"
                       "Name VARCHAR(80) NOT NULL"
                   ");");
        query.exec("CREATE TABLE Product ("
                       "Id   INTEGER PRIMARY KEY AUTOINCREMENT,"
                       "Name VARCHAR(80) NOT NULL,"
                       "ProductCategory VARCHAR(80) NOT NULL"
                   ");");
        query.exec("CREATE TABLE RevenueSource ("
                       "Id   INTEGER PRIMARY KEY AUTOINCREMENT,"
                       "Name VARCHAR(80) NOT NULL"
                   ");");
        query.exec("CREATE TABLE Debit ("
                       "Id   INTEGER PRIMARY KEY AUTOINCREMENT,"
                       "Person VARCHAR(80) NOT NULL,"
                       "Date DATE NOT NULL,"
                       "Value DOUBLE NOT NULL,"
                       "RevenueSource VARCHAR(80) NOT NULL,"
                       "Comment VARCHAR(128)"
                   ");");
        query.exec("CREATE TABLE Credit ("
                       "Id   INTEGER PRIMARY KEY AUTOINCREMENT,"
                       "Person VARCHAR(80) NOT NULL,"
                       "Date DATE NOT NULL,"
                       "Value DOUBLE NOT NULL,"
                       "Product VARCHAR(80) NOT NULL,"
                       "ProductCategory VARCHAR(80) NOT NULL,"
                       "Comment VARCHAR(128)"
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
        if (TABLES == _db.tables())
            emit statusUpdated(DS_Connected);
        else
            emit error("Некорректная база данных.");
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
