#include "ChartsDialog.h"
#include "ui_ChartsDialog.h"

int ChartsDialog::instances = 0;

ChartsDialog::ChartsDialog(QWidget *parent) : QDialog(parent) {
    instances--;
    m_ui = new Ui::ChartsDialog;
    m_ui->setupUi(this);
    this->setWindowIcon(QIcon(":/assets/Assets/Icons/Dark/chart.png"));


}

ChartsDialog::~ChartsDialog() {
    instances++;
    delete m_ui;
}

int ChartsDialog::instancesCount() {
    return instances;
}
