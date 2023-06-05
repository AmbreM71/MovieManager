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

#include "Enums.h"
#include "Log.h"

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

    static void setIconAccordingToTheme(QAction* object, enum eTheme theme, QString filename);

    static QString getVersion();

    static void LogDatabaseError(QSqlQuery *query);
};

#endif // COMMON_H
