#ifndef FILTERSDIALOG_H
#define FILTERSDIALOG_H

#include <QDialog>
#include <QDate>

#include <Structures.h>

namespace Ui {
class FiltersDialog;
}

class FiltersDialog : public QDialog
{
    Q_OBJECT

    public:
        explicit FiltersDialog(stFilters* filters, QWidget* parent = nullptr);
        ~FiltersDialog();

    private:
        Ui::FiltersDialog* m_ui;
        stFilters* m_filters;
};

#endif // FILTERSDIALOG_H
