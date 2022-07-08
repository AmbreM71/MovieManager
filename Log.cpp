#include "Log.h"

void Log::append(QString s) {
    m_log.append(s);
}

void Log::clear() {
    m_log.clear();
}

QString Log::getLog(int i) {
    QList<QString>::iterator it = m_log.begin();
    std::advance(it, i);
    return *it;
}

int Log::size() {
    return m_log.size();
}
