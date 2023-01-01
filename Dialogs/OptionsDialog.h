#ifndef OPTIONSDIALOG_H
#define OPTIONSDIALOG_H

#include <QDialog>
#include "Enums.h"

namespace Ui {
class OptionsDialog;
}

class OptionsDialog : public QDialog
{
    Q_OBJECT

    private:
        Ui::OptionsDialog* m_ui;
        bool *m_matrixMode, *m_quickSearchCaseSensitive;
        enum eTheme *m_theme;
        enum eLanguage *m_language;

    public:
        explicit OptionsDialog(bool* matrixMode, enum eLanguage* language, enum eTheme* theme, bool* quickSearchCaseSensitive, QWidget *parent = nullptr);
        ~OptionsDialog();


};

#endif // OPTIONSDIALOG_H
