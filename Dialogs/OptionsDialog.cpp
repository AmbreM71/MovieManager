#include "OptionsDialog.h"
#include "ui_OptionsDialog.h"

OptionsDialog::OptionsDialog(QSettings* settings, QWidget *parent) : QDialog(parent) {
    m_ui = new Ui::OptionsDialog;
    m_ui->setupUi(this);
    this->setWindowIcon(QIcon(":/assets/Assets/Icons/Dark/settings.png"));

    m_settings = settings;

    // Set current settings values
    m_ui->ThemeCombobox->setCurrentIndex((enum eTheme)m_settings->value("theme").toInt());
    m_ui->LanguageCombobox->setCurrentIndex((enum eLanguage)m_settings->value("language").toInt());
    m_ui->MatrixModeCheckbox->setChecked(m_settings->value("matrixMode").toBool());
    m_ui->QuickSearchCaseCheckbox->setChecked(m_settings->value("quickSearchCaseSensitive").toBool());
    m_ui->DateFormatCombobox->setCurrentIndex(m_ui->DateFormatCombobox->findText(m_settings->value("dateFormat").toString()));

}

OptionsDialog::~OptionsDialog() {
    m_settings->setValue("language", (enum eLanguage)m_ui->LanguageCombobox->currentIndex());
    m_settings->setValue("theme", (enum eTheme)m_ui->ThemeCombobox->currentIndex());
    m_settings->setValue("matrixMode", m_ui->MatrixModeCheckbox->isChecked());
    m_settings->setValue("quickSearchCaseSensitive", m_ui->QuickSearchCaseCheckbox->isChecked());
    m_settings->setValue("dateFormat", m_ui->DateFormatCombobox->currentText());
    delete m_ui;
}
