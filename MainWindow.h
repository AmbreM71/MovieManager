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

        enum Theme {
            Classic = 0,
            Dark = 1,
            AMOLED = 2,
            MidnightPurple = 3,
        };

        enum Language {
            French = 0,
            English = 1,
        };

        Ui::MainWindow* m_ui;
        QApplication* m_app;
        QSqlDatabase m_db;
        Log* m_log;
        std::tm* m_now;

        //Setting variables
        bool m_matrixMode;
        int m_language;
        int m_theme;

        //Filter variables
        QString m_filter_name = "";
        int m_filter_minYear = 0,
            m_filter_maxYear = 0,
            m_filter_minRating = 0,
            m_filter_maxRating = 0,
            m_filter_minEntries = 0;


    public:
        explicit MainWindow(QApplication* app, QWidget *parent = nullptr);
        void databaseConnection();
        void loadDB(bool isFiltered = false);
        void fillGlobalStats();
        void filterTable();
        void setSettings();
        ~MainWindow();

    public slots:
        void addView();
        void editViews();
        void openFilters();
        void menuBarConnectors();
        void openLog();
        void openAbout();
        void openSettings();
        void resetFilters();

};

#endif // MAINWINDOW_H
