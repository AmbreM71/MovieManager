#include "AddViewDialog.h"
#include "ui_AddViewDialog.h"

AddViewDialog::AddViewDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddViewDialog)
{
    ui->setupUi(this);
}

AddViewDialog::~AddViewDialog()
{
    delete ui;
}
