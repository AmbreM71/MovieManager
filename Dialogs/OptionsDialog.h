#ifndef OPTIONSDIALOG_H
#define OPTIONSDIALOG_H

#include <QDialog>
#include <QSettings>
#include "Enums.h"

namespace Ui {
class OptionsDialog;
}

class OptionsDialog : public QDialog
{
    Q_OBJECT

    private:
        Ui::OptionsDialog* m_ui;
        QSettings* m_settings;

    public:
        explicit OptionsDialog(QSettings* settings, QWidget* parent = nullptr);
        ~OptionsDialog();


};

#endif // OPTIONSDIALOG_H
