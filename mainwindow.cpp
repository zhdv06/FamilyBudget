#include <QApplication>
#include <QCloseEvent>
#include <QMetaObject>
#include <QFileDialog>
#include <QMessageBox>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "database.h"
#include "reportwidget.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->registry, &RegistryWidget::error,
            this, &MainWindow::showError);
    connect(ui->catalog, &CatalogWidget::error,
            this, &MainWindow::showError);
    connect(ui->registry, &RegistryWidget::info,
            [this](const QString &message){ui->statusBar->showMessage(message, 500);});
    connect(ui->catalog, &CatalogWidget::info,
            [this](const QString &message){ui->statusBar->showMessage(message, 500);});

    ui->disconnectAction->setEnabled(false);

    ui->addAction->setEnabled(false);
    ui->removeAction->setEnabled(false);
    ui->confirmAction->setEnabled(false);
    ui->cancelAction->setEnabled(false);
    ui->reportAction->setEnabled(false);

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
    connect(ui->confirmAction, &QAction::triggered,
            [this](){qobject_cast<IWidget*>(ui->tabWidget->currentWidget())->confirmRecord();});
    connect(ui->cancelAction, &QAction::triggered,
            [this](){qobject_cast<IWidget*>(ui->tabWidget->currentWidget())->cancelRecord();});

    connect(ui->reportAction, &QAction::triggered,
            this, &MainWindow::showReport);

    connect(ui->catalog, &CatalogWidget::removalAllowed,
            ui->removeAction, &QAction::setEnabled);

    connect(_database, &Database::statusUpdated,
            this, &MainWindow::updateUi);
    connect(this, &MainWindow::request,
            _database, &Database::processRequest);
    connect(_database, &Database::response,
            this, &MainWindow::processResponse);

    connect(ui->registry, &RegistryWidget::changeStatusUpdated,
            ui->confirmAction, &QAction::setEnabled);
    connect(ui->registry, &RegistryWidget::changeStatusUpdated,
            ui->cancelAction, &QAction::setEnabled);
    connect(ui->catalog, &CatalogWidget::changeStatusUpdated,
            ui->confirmAction, &QAction::setEnabled);
    connect(ui->catalog, &CatalogWidget::changeStatusUpdated,
            ui->cancelAction, &QAction::setEnabled);
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

void MainWindow::showReport()
{
    ReportWidget *reportWidget = new ReportWidget;
    connect(reportWidget, &ReportWidget::error,
            this, &MainWindow::showError);
    connect(reportWidget, &ReportWidget::finished,
            reportWidget, &ReportWidget::hide);
    connect(reportWidget, &ReportWidget::finished,
            reportWidget, &ReportWidget::deleteLater);
    reportWidget->show();
}

void MainWindow::updateUi(DatabaseStatus status)
{
    switch (status)
    {
    case DS_Connected:
        ui->connectAction->setEnabled(false);
        ui->disconnectAction->setEnabled(true);

        ui->addAction->setEnabled(true);
        ui->reportAction->setEnabled(true);

        ui->centralWidget->setEnabled(true);
        ui->statusBar->showMessage("Соединение установлено");

        for (int i = 0; i < ui->tabWidget->count(); i++)
            qobject_cast<IWidget*>(ui->tabWidget->widget(i))->init();

        //emit request(RT_Tables);
        break;

    case DS_Disconnected:
        ui->connectAction->setEnabled(true);
        ui->disconnectAction->setEnabled(false);

        ui->addAction->setEnabled(false);
        ui->confirmAction->setEnabled(false);
        ui->cancelAction->setEnabled(false);
        ui->reportAction->setEnabled(false);

        ui->centralWidget->setEnabled(false);
        ui->statusBar->showMessage("Соединение не установлено");

        for (int i = 0; i < ui->tabWidget->count(); i++)
            qobject_cast<IWidget*>(ui->tabWidget->widget(i))->reset();
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

void MainWindow::closeEvent(QCloseEvent *event)
{
    _database->disconnect_();
    event->accept();
}
