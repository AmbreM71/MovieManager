#ifndef FILTERSDIALOG_H
#define FILTERSDIALOG_H

#include <QDialog>
#include <QDate>
#include <QSqlQuery>

#include <Structures.h>
#include <Common.h>

namespace Ui {
class FiltersDialog;
}

class FiltersDialog : public QDialog
{
    Q_OBJECT

    public:
        explicit FiltersDialog(stFilters* filters, QWidget* parent = nullptr);
        ~FiltersDialog();
        QMap<QWidget*, QString>* getCustomColumnsMap();

    private:
        Ui::FiltersDialog* m_ui;
        stFilters* m_filters;
        QMap<QWidget*, QString>* m_customColumnsMap;
};

#endif // FILTERSDIALOG_H
