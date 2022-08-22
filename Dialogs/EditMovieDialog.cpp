#include "EditMovieDialog.h"
#include "ui_EditMovieDialog.h"

EditMovieDialog::EditMovieDialog(QTableWidget* table, QWidget *parent) : QDialog(parent) {
    m_ui = new Ui::EditMovieDialog;
    m_ui->setupUi(this);
    m_table = table;

    m_ui->NameInput->setText(table->item(table->currentRow(),0)->text());
    m_ui->ReleaseYearInput->setText(table->item(table->currentRow(),1)->text());
    m_ui->EntriesInput->setValue(table->item(table->currentRow(),5)->text().toInt());
    m_ui->RatingInput->setValue(table->item(table->currentRow(),6)->text().toInt());
}

EditMovieDialog::~EditMovieDialog() {
    delete m_ui;
}

bool EditMovieDialog::edited() {
    return m_edited;
}

QString EditMovieDialog::getMovieName() {
    return m_ui->NameInput->text();
}

QString EditMovieDialog::getReleaseYear() {
     return m_ui->ReleaseYearInput->text();
}

int EditMovieDialog::getRating() {
    return m_ui->RatingInput->value();
}

int EditMovieDialog::getEntries() {
    return m_ui->EntriesInput->value();
}
