#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <QString>
#include <Enums.h>

struct LogElement {
    QString string;
    enum LogCriticity criticity;
};

#endif // STRUCTURES_H
