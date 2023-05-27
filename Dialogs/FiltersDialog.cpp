#include "FiltersDialog.h"
#include "ui_FiltersDialog.h"

FiltersDialog::FiltersDialog(stFilters* filters, QWidget *parent) : QDialog(parent) {
    m_ui = new Ui::FiltersDialog;
    m_ui->setupUi(this);
    this->setWindowIcon(QIcon(":/assets/Assets/Icons/Dark/search.png"));

    m_ui->MaxYearInput->setValue(QDate::currentDate().year());
    m_ui->MaxYearInput->setMaximum(QDate::currentDate().year());

    m_filters = filters;

        m_ui->NameInput->setText(filters->sName);
        m_ui->MinYearInput->setValue(filters->nMinYear);
        m_ui->MaxYearInput->setValue(filters->nMaxYear);
        m_ui->MinRatingInput->setValue(filters->nMinRating);
        m_ui->MaxRatingInput->setValue(filters->nMaxRating);
        m_ui->MinEntriesInput->setValue(filters->nMinEntries);
        m_ui->MinEntriesInput->setValue(filters->nMinEntries);
        m_ui->UnknownEntriesCheckbox->setChecked(filters->bShowMovieWithUnknownEntries);


}

FiltersDialog::~FiltersDialog() {
    m_filters->sName = m_ui->NameInput->text();
    m_filters->nMinYear = m_ui->MinYearInput->value();
    m_filters->nMaxYear = m_ui->MaxYearInput->value();
    m_filters->nMinRating = m_ui->MinRatingInput->value();
    m_filters->nMaxRating = m_ui->MaxRatingInput->value();
    m_filters->nMinEntries = m_ui->MinEntriesInput->value();
    m_filters->bShowMovieWithUnknownEntries = m_ui->UnknownEntriesCheckbox->isChecked();
    delete m_ui;
}
