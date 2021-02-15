#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVariant>
#include "defines.h"

namespace Ui {
class MainWindow;
}

class Database;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void showError(const QString &message);
    void createDatabase();
    void updateDatabaseStatus(DatabaseStatus status);
    void processResponse(RequestType rt, QVariant data);

signals:
    void request(RequestType rt);

private slots:
    void connectToDatabase();
    void showReport();

private:
    Ui::MainWindow *ui;
    Database  *_database;
};

#endif // MAINWINDOW_H
