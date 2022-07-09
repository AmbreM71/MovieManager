#include "OptionsDialog.h"
#include "ui_OptionsDialog.h"

OptionsDialog::OptionsDialog(bool* matrixMode, int *language, int *theme, QWidget *parent) : QDialog(parent) {
    m_ui = new Ui::OptionsDialog;
    m_ui->setupUi(this);
    m_matrixMode = matrixMode;
    m_language = language;
    m_theme = theme;

    m_ui->ThemeCombobox->setCurrentIndex(*theme);
    m_ui->LanguageCombobox->setCurrentIndex(*language);
    m_ui->MatrixModeCheckbox->setChecked(*m_matrixMode);
}

OptionsDialog::~OptionsDialog() {
    *m_matrixMode = m_ui->MatrixModeCheckbox->isChecked();
    *m_language = m_ui->LanguageCombobox->currentIndex();
    *m_theme = m_ui->ThemeCombobox->currentIndex();
    delete m_ui;
}
