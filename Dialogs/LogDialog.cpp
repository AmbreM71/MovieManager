#include "LogDialog.h"
#include "ui_LogDialog.h"

LogDialog::LogDialog(Log* log, QWidget *parent) : QDialog(parent) {
    m_ui = new Ui::LogDialog;
    m_ui->setupUi(this);
    m_log = log;
    fillList();
    QObject::connect(m_log, SIGNAL(logAppended()), this, SLOT(refreshLog()));
}

LogDialog::~LogDialog() {
    delete m_ui;
}

void LogDialog::fillList() {
    m_ui->listWidget->clear();
    for(int i = 0 ; i < m_log->size() ; i++) {
        m_ui->listWidget->addItem(m_log->getLog(i));
    }
}

void LogDialog::refreshLog() {
    this->fillList();
}
