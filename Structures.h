#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <QString>
#include <Enums.h>

struct LogElement {
    QString string;
    enum eLog criticity;
};

#endif // STRUCTURES_H
