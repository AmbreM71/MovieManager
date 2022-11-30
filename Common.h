#ifndef COMMON_H
#define COMMON_H

#include <QString>
#include <QFileDialog>
#include <QMessageBox>
#include <QLabel>
#include <QFileInfo>
#include <QLayoutItem>
#include <QLayout>

#include "Enums.h"

class Common
{
public:
    Common();
    static void loadPoster(QWidget* parent, QLabel* poster, int posterHeight, float safeRatio, QString path = "", QString* resultpath = nullptr);

    static void ratingToStar(int rating, QLabel* ratingLabel);

    static void clearLayout(QLayout* layout);

    static QString viewTypeToQString(enum ViewType type);

    static enum ViewType QStringToViewType(QString type);
};

#endif // COMMON_H
