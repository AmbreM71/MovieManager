#include "OptionsDialog.h"
#include "ui_OptionsDialog.h"

OptionsDialog::OptionsDialog(QWidget *parent) : QDialog(parent) {
    m_ui = new Ui::OptionsDialog;
    m_ui->setupUi(this);
    this->setWindowIcon(QIcon(":/assets/Assets/Icons/Dark/settings.png"));

    // Set current settings values
    m_ui->ThemeCombobox->setCurrentIndex((enum eTheme)Common::Settings->value("theme").toInt());
    m_ui->LanguageCombobox->setCurrentIndex((enum eLanguage)Common::Settings->value("language").toInt());
    m_ui->MatrixModeCheckbox->setChecked(Common::Settings->value("matrixMode").toBool());
    m_ui->QuickSearchCaseCheckbox->setChecked(Common::Settings->value("quickSearchCaseSensitive").toBool());
    m_ui->MoreLogsCheckbox->setChecked(Common::Settings->value("moreLogs").toBool());
    m_ui->DateFormatCombobox->setCurrentIndex(m_ui->DateFormatCombobox->findText(Common::Settings->value("dateFormat").toString()));

}

OptionsDialog::~OptionsDialog() {
    Common::Settings->setValue("language", (enum eLanguage)m_ui->LanguageCombobox->currentIndex());
    Common::Settings->setValue("theme", (enum eTheme)m_ui->ThemeCombobox->currentIndex());
    Common::Settings->setValue("matrixMode", m_ui->MatrixModeCheckbox->isChecked());
    Common::Settings->setValue("quickSearchCaseSensitive", m_ui->QuickSearchCaseCheckbox->isChecked());
    Common::Settings->setValue("moreLogs", m_ui->MoreLogsCheckbox->isChecked());
    Common::Settings->setValue("dateFormat", m_ui->DateFormatCombobox->currentText());
    delete m_ui;
}
