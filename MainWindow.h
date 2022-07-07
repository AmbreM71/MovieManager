#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QSqlDatabase"
#include "QSqlQuery"
#include "QSqlError"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

    private:
        Ui::MainWindow* m_ui;
        QApplication* m_app;
        QSqlDatabase m_db;

    public:
        explicit MainWindow(QApplication* app, QWidget *parent = nullptr);
        void databaseConnection();
        void loadDB();
        ~MainWindow();

    public slots:
        void addView();

};

#endif // MAINWINDOW_H
