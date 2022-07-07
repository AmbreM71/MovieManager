#include "EditViewsDialog.h"
#include "ui_EditViewsDialog.h"

EditViewsDialog::EditViewsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditViewsDialog)
{
    ui->setupUi(this);
}

EditViewsDialog::~EditViewsDialog()
{
    delete ui;
}
