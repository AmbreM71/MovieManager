#ifndef ADDVIEWDIALOG_H
#define ADDVIEWDIALOG_H

#include <QDialog>

namespace Ui {
class AddViewDialog;
}

class AddViewDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddViewDialog(QWidget *parent = nullptr);
    ~AddViewDialog();

private:
    Ui::AddViewDialog *ui;
};

#endif // ADDVIEWDIALOG_H
