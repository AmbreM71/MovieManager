#ifndef COMMON_H
#define COMMON_H

#include <QString>
#include <QFileDialog>
#include <QMessageBox>
#include <QLabel>
#include <QFileInfo>
#include <QLayoutItem>
#include <QLayout>
#include <QSettings>
#include <QSqlQuery>
#include <QSqlError>
#include <QPushButton>

#include "Enums.h"
#include "Log.h"
#include "BuildFunctions.h"

class Common
{
public:

    Common();

    static Log* Log;
    static QSettings* Settings;

    static void loadPoster(QWidget* parent, QLabel* poster, int posterHeight, float safeRatio, QString path = "", QString* resultpath = nullptr);

    static void ratingToStar(int rating, QLabel* ratingLabel);

    static void clearLayout(QLayout* layout);

    static QString viewTypeToQString(enum eViewType type);

    static enum eViewType QStringToViewType(QString type);

    static QString ColumnTypeToQString(enum eColumnType type);

    static void setIconAccordingToTheme(QAction* object, QString filename);
    static void setIconAccordingToTheme(QPushButton* object, QString filename);

    static QString getVersion();

    static void LogDatabaseError(QSqlQuery *query);
};

#endif // COMMON_H
