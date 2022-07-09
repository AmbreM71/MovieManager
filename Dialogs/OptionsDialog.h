#ifndef OPTIONSDIALOG_H
#define OPTIONSDIALOG_H

#include <QDialog>

namespace Ui {
class OptionsDialog;
}

class OptionsDialog : public QDialog
{
    Q_OBJECT

    private:
        Ui::OptionsDialog* m_ui;
        bool* m_matrixMode;

    public:
        explicit OptionsDialog(bool* matrixMode, QWidget *parent = nullptr);
        ~OptionsDialog();


};

#endif // OPTIONSDIALOG_H
