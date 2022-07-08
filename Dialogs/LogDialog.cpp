#include "LogDialog.h"
#include "ui_LogDialog.h"

int LogDialog::instances = 0;

LogDialog::LogDialog(Log* log, QWidget *parent) : QDialog(parent) {
    instances++;
    m_ui = new Ui::LogDialog;
    m_ui->setupUi(this);
    m_log = log;
    fillList();
    QObject::connect(m_log, SIGNAL(logAppended()), this, SLOT(refresh()));
}

LogDialog::~LogDialog() {
    instances--;
    delete m_ui;
}

void LogDialog::fillList() {
    m_ui->listWidget->clear();
    for(int i = 0 ; i < m_log->size() ; i++) {
        m_ui->listWidget->addItem(m_log->getLog(i));
    }
}

int LogDialog::instancesCount() {
    return instances;
}

void LogDialog::refresh() {
    this->fillList();
}
