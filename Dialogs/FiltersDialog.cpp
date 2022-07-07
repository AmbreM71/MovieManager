#include "FiltersDialog.h"
#include "ui_FiltersDialog.h"

FiltersDialog::FiltersDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FiltersDialog)
{
    ui->setupUi(this);
}

FiltersDialog::~FiltersDialog()
{
    delete ui;
}
