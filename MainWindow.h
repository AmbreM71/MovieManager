#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <cmath>
#include <ctime>
#include <iostream>

#include "Log.h"

#include "Dialogs/AddViewDialog.h"
#include "Dialogs/EditViewsDialog.h"
#include "Dialogs/FiltersDialog.h"
#include "Dialogs/LogDialog.h"
#include "Dialogs/AboutDialog.h"
#include "Dialogs/OptionsDialog.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

    private:
        Ui::MainWindow* m_ui;
        QApplication* m_app;
        QSqlDatabase m_db;
        Log* m_log;
        std::tm* now;


    public:
        explicit MainWindow(QApplication* app, QWidget *parent = nullptr);
        void databaseConnection();
        void loadDB();
        void fillGlobalStats();
        ~MainWindow();

    public slots:
        void addView();
        void editViews();
        void openFilters();
        void menuBarConnectors();
        void openLog();
        void openAbout();
        void openSettings();

};

#endif // MAINWINDOW_H
