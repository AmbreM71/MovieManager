#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QTranslator>
#include <QFile>
#include <QDir>
#include <QJsonObject>
#include <QJsonDocument>
#include <QMessageBox>
#include <QFileDialog>
#include <QSettings>
#include <QPixmap>
#include <QRandomGenerator>
#include <QStandardPaths>
#include <QImage>

#include <cmath>
#include <ctime>
#include <iostream>

#include "Log.h"
#include "Enums.h"
#include "Tag.h"

#include "Dialogs/AddViewDialog.h"
#include "Dialogs/EditViewsDialog.h"
#include "Dialogs/FiltersDialog.h"
#include "Dialogs/LogDialog.h"
#include "Dialogs/AboutDialog.h"
#include "Dialogs/OptionsDialog.h"
#include "Dialogs/EditMovieDialog.h"
#include "Dialogs/ChartsDialog.h"
#include "Dialogs/ChangelogDialog.h"
#include "Dialogs/CalendarDialog.h"

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
        QTranslator m_translator;
        int m_savedMovieID = -1;
        QString m_savepath = "";
        QLocale* m_locale;

        //Setting variables
        QSettings* m_settings;

        //Filter variables
        bool m_isFiltered = false;
        QString m_filter_name = "";
        int m_filter_minYear = 0,
            m_filter_maxYear = 0,
            m_filter_minRating = 0,
            m_filter_maxRating = 0,
            m_filter_minEntries = 0;


    public:

        explicit MainWindow(QApplication* app);
        void databaseConnection();
        void fillGlobalStats();
        void filterTable();
        void removeUnusedTags();
        void refreshLanguage();
        void refreshTheme();
        void setMatrixMode(bool state);
        int getIndexOfMovie(int ID);
        ~MainWindow();

    public slots:
        void fillTable(const QString &text);
        void addView();
        void editViews();
        void openFilters();
        void menuBarConnectors();
        void openLog();
        void openAbout();
        void openSettings();
        void openCharts();
        void openCalendar();
        void resetFilters();
        void importDB();
        void exportDB();
        void customMenuRequested(QPoint pos);
        void deleteMovie();
        void editMovie();
        void fillMovieInfos();
        void selectedMovieChanged();
        void on_EasterEggAct_triggered();
        void on_whatsnewAct_triggered();

        void clickedTag(Tag* tag);
        void clickedFilterTag(Tag* tag);
        void mouseEnteredTag(Tag* tag);
        void mouseLeftTag(Tag* tag);

};

#endif // MAINWINDOW_H
