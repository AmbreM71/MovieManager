#ifndef OPTIONSDIALOG_H
#define OPTIONSDIALOG_H

#include <QDialog>
#include "Enums.h"
#include "Common.h"
#include "AddColumnDialog.h"

namespace Ui {
    class OptionsDialog;
}

class OptionsDialog : public QDialog
{
    Q_OBJECT

    private:
        Ui::OptionsDialog* m_ui;

    public:
        explicit OptionsDialog(QWidget* parent = nullptr);
        ~OptionsDialog();

    public slots:
        void AddColumn();
};

#endif // OPTIONSDIALOG_H
