#ifndef EDITVIEWSDIALOG_H
#define EDITVIEWSDIALOG_H

#include <QDialog>

namespace Ui {
class EditViewsDialog;
}

class EditViewsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditViewsDialog(QWidget *parent = nullptr);
    ~EditViewsDialog();

private:
    Ui::EditViewsDialog *ui;
};

#endif // EDITVIEWSDIALOG_H
