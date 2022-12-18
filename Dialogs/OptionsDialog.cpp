#include "OptionsDialog.h"
#include "ui_OptionsDialog.h"

OptionsDialog::OptionsDialog(bool* matrixMode, int *language, int *theme, bool* quickSearchCaseSensitive, QWidget *parent) : QDialog(parent) {
    m_ui = new Ui::OptionsDialog;
    m_ui->setupUi(this);

    m_matrixMode = matrixMode;
    m_language = language;
    m_theme = theme;
    m_quickSearchCaseSensitive = quickSearchCaseSensitive;

    m_ui->ThemeCombobox->setCurrentIndex(*m_theme);
    m_ui->LanguageCombobox->setCurrentIndex(*m_language);
    m_ui->MatrixModeCheckbox->setChecked(*m_matrixMode);
    m_ui->QuickSearchCaseCheckbox->setChecked(*m_quickSearchCaseSensitive);
}

OptionsDialog::~OptionsDialog() {
    *m_matrixMode = m_ui->MatrixModeCheckbox->isChecked();
    *m_language = m_ui->LanguageCombobox->currentIndex();
    *m_theme = m_ui->ThemeCombobox->currentIndex();
    *m_quickSearchCaseSensitive = m_ui->QuickSearchCaseCheckbox->isChecked();
    delete m_ui;
}
