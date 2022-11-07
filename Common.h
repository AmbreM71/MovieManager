#ifndef COMMON_H
#define COMMON_H

#include <QString>
#include <QFileDialog>
#include <QMessageBox>
#include <QLabel>
#include <QFileInfo>

class Common
{
public:
    Common();

    static void loadPoster(QWidget* parent, QLabel* poster, int posterHeight, float safeRatio, QString path = "", QString* resultpath = nullptr);

    static void ratingToStar(int rating, QLabel* ratingLabel);
};

#endif // COMMON_H
