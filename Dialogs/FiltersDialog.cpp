#include "FiltersDialog.h"
#include "ui_FiltersDialog.h"

FiltersDialog::FiltersDialog(QString* name, int* minYear, int* maxYear, int* minRating, int* maxRating, int* minEntries, QWidget *parent) : QDialog(parent) {
    m_ui = new Ui::FiltersDialog;
    m_ui->setupUi(this);

    m_ui->MaxYearInput->setValue(QDate::currentDate().year());
    m_ui->MaxYearInput->setMaximum(QDate::currentDate().year());

    m_name = name;
    m_minYear = minYear;
    m_maxYear = maxYear;
    m_minRating = minRating;
    m_maxRating = maxRating;
    m_minEntries = minEntries;


    if(*name != "")
        m_ui->NameInput->setText(*name);
    if(*minYear != 0)
        m_ui->MinYearInput->setValue(*minYear);
    if(*maxYear != 0)
        m_ui->MaxYearInput->setValue(*maxYear);
    if(*minRating != 0)
        m_ui->MinRatingInput->setValue(*minRating);
    if(*maxRating != 0)
        m_ui->MaxRatingInput->setValue(*maxRating);
    if(*minEntries != 0)
        m_ui->MinEntriesInput->setValue(*minEntries);

}

FiltersDialog::~FiltersDialog() {
    *m_name = m_ui->NameInput->text();
    *m_minYear = m_ui->MinYearInput->value();
    *m_maxYear = m_ui->MaxYearInput->value();
    *m_minRating = m_ui->MinRatingInput->value();
    *m_maxRating = m_ui->MaxRatingInput->value();
    *m_minEntries = m_ui->MinEntriesInput->value();
    delete m_ui;
}
