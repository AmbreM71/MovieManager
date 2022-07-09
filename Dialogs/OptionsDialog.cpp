#include "OptionsDialog.h"
#include "ui_OptionsDialog.h"

OptionsDialog::OptionsDialog(bool* matrixMode, QWidget *parent) : QDialog(parent) {
    m_ui = new Ui::OptionsDialog;
    m_ui->setupUi(this);
    m_matrixMode = matrixMode;
    qDebug() << *m_matrixMode;
    m_ui->MatrixModeCheckbox->setChecked(*m_matrixMode);
}

OptionsDialog::~OptionsDialog() {
    *m_matrixMode = m_ui->MatrixModeCheckbox->isChecked();
    delete m_ui;
}
