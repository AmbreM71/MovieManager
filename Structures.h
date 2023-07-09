#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <QString>
#include <Enums.h>

struct stLogElement {
    QString sText;
    enum eLog eCriticity;
};

struct stFilters {
    QString sName;
    int nMinYear;
    int nMaxYear;
    int nMinRating;
    int nMaxRating;
};

struct stColumn {
    QString sName;
    enum eColumnType eType;
    double nMin;
    double nMax;
    int nPrecision;
    int textMaxLength;
};

#endif // STRUCTURES_H
