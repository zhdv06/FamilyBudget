#include <QApplication>
#include <QMetaObject>
#include <QFileDialog>
#include <QMessageBox>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "database.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->disconnectAction->setEnabled(false);

    ui->addAction->setEnabled(false);
    ui->removeAction->setEnabled(false);

    ui->centralWidget->setEnabled(false);
    ui->statusBar->showMessage("Соединение не установлено");

    _database = new Database(this);

    connect(_database, &Database::error,
            this, &MainWindow::showError);
    connect(ui->createAction, &QAction::triggered,
            this, &MainWindow::createDatabase);
    connect(ui->connectAction, &QAction::triggered,
            this, &MainWindow::connectToDatabase);
    connect(ui->disconnectAction, &QAction::triggered,
            _database, &Database::disconnect_);

    connect(ui->addAction, &QAction::triggered,
            [this](){qobject_cast<IWidget*>(ui->tabWidget->currentWidget())->addRecord();});
    connect(ui->removeAction, &QAction::triggered,
            [this](){qobject_cast<IWidget*>(ui->tabWidget->currentWidget())->removeRecord();});

    connect(ui->tabWidget, &QTabWidget::currentChanged,
            [this](int index){ui->removeAction->setDisabled(index == 0);});

    connect(_database, &Database::statusUpdated,
            this, &MainWindow::updateDatabaseStatus);
    connect(this, &MainWindow::request,
            _database, &Database::processRequest);
    connect(_database, &Database::response,
            this, &MainWindow::processResponse);


}

void MainWindow::showError(const QString &message)
{
    QMessageBox::critical(this, "Error", message);
}

void MainWindow::createDatabase()
{
    QString fileName =
            QFileDialog::getSaveFileName(this, "Создать файл",
                                         QApplication::applicationDirPath(), "*.db");
    if (!fileName.isEmpty())
        QMetaObject::invokeMethod(_database, "create", Qt::QueuedConnection, Q_ARG(QString, fileName));
}

void MainWindow::connectToDatabase()
{
    QString fileName =
            QFileDialog::getOpenFileName(this, "Открыть файл",
                                         QApplication::applicationDirPath(), "*.db");
    if (!fileName.isEmpty())
        QMetaObject::invokeMethod(_database, "connect_", Qt::QueuedConnection, Q_ARG(QString, fileName));
}

void MainWindow::updateDatabaseStatus(DatabaseStatus status)
{
    switch (status)
    {
    case DS_Connected:
        ui->connectAction->setEnabled(false);
        ui->disconnectAction->setEnabled(true);

        ui->addAction->setEnabled(true);
        ui->removeAction->setEnabled(true);

        ui->centralWidget->setEnabled(true);
        ui->statusBar->showMessage("Соединение установлено");
        emit request(RT_Tables);
        break;

    case DS_Disconnected:
        ui->connectAction->setEnabled(true);
        ui->disconnectAction->setEnabled(false);

        ui->addAction->setEnabled(false);
        ui->removeAction->setEnabled(false);

        ui->centralWidget->setEnabled(false);
        ui->statusBar->showMessage("Соединение не установлено");
        break;
    }
}

void MainWindow::processResponse(RequestType rt, QVariant data)
{
    switch (rt)
    {
    case RT_Command:
        if (data.canConvert<bool>())
            data.toBool();
            //show message at status bar
        break;

    case RT_Tables:
        if (data.canConvert<QStringList>())
            ui->catalog->setTables(data.toStringList());
        break;
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
