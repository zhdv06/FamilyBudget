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
    /**
     * @brief Выводит сообщение об ошибке
     * @param message сообщение об ошибке
     */

    void showError(const QString &message);

    /**
     * @brief Создает базу данных
     */
    void createDatabase();

    /**
     * @brief Обновляет графический интерфес
     * @param status статус подключения к базе данных
     */
    void updateUi(DatabaseStatus status);

    // processResponse оставил в качестве примера работы схемы запрос-ответ
    /**
     * @brief Обрабатывает ответ от базы данных
     * @param rt тип запроса
     * @param data данные
     */
    void processResponse(RequestType rt, QVariant data);

protected:
    void closeEvent(QCloseEvent *event) override;

signals:
    // request оставил в качестве примера работы схемы запрос-ответ
    /**
     * @brief Сигнал запроса к базе данных
     * @param rt тип запроса
     */
    void request(RequestType rt);

private slots:
    /**
     * @brief Подключается к базе данных
     */
    void connectToDatabase();
    /**
     * @brief Открывает диалог отчета
     */
    void showReport();

private:
    Ui::MainWindow *ui;
    Database  *_database;
};

#endif // MAINWINDOW_H
