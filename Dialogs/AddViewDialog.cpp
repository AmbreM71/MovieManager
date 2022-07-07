#include "AddViewDialog.h"
#include "ui_AddViewDialog.h"

AddViewDialog::AddViewDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddViewDialog) {
    ui->setupUi(this);
}

AddViewDialog::~AddViewDialog() {
    delete ui;
}

QString AddViewDialog::getName() {
    return ui->MovieNameInput->text();
}
int AddViewDialog::getReleaseYear() {
    return ui->MovieReleaseYearInput->text().toInt();
}
QString AddViewDialog::getViewType() {
    return ui->ViewTypeComboBox->currentText();
}
QString AddViewDialog::getViewDate() {
    return ui->MovieViewDateInput->text();
}
int AddViewDialog::getRating() {
    return ui->MovieRatingInput->value();
}
