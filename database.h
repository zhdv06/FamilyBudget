#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QVariant>
#include <QSqlDatabase>
#include "defines.h"

class Database : public QObject
{
    Q_OBJECT
public:
    explicit Database(QObject *parent = nullptr);

public slots:
    void create(const QString &fileName);
    void connect_(const QString &fileName);
    void disconnect_();
    void processRequest(RequestType rt);

signals:
    void error(const QString &message);
    void response(RequestType rt, QVariant data);
    void statusUpdated(DatabaseStatus status);

private:
    QSqlDatabase _db;
    const QStringList TABLES{"Sex", "sqlite_sequence", "Person",
                             "ProductCategory", "Product", "RevenueSource",
                             "Debit", "Credit"};
};

#endif // DATABASE_H
