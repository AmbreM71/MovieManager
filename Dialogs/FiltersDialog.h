#ifndef FILTERSDIALOG_H
#define FILTERSDIALOG_H

#include <QDialog>
#include <QDate>
#include <QSqlQuery>
#include <QCheckBox>

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
        QList<QCheckBox*>* getCustomColumnsUnknownCheckBox();

    private:
        Ui::FiltersDialog* m_ui;
        stFilters* m_filters;
        QMap<QWidget*, QString>* m_customColumnsMap;
        QList<QCheckBox*>* m_customColumnsUnknownCheckBox;
};

#endif // FILTERSDIALOG_H
