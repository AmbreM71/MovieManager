#ifndef FILTERSDIALOG_H
#define FILTERSDIALOG_H

#include <QDialog>
#include <QDate>
#include <QSqlQuery>
#include <QComboBox>

#include "Structures.h"
#include "Common.h"
#include "CustomColumnLineEdit.h"

enum class eCombobox {
    None,
    And,
    Or
};

namespace Ui {
class FiltersDialog;
}

class FilterComboBox : public QComboBox
{
    Q_OBJECT

    private:
        QList<CustomColumnLineEdit*> m_relatedWidgets;
        enum eColumnType m_columnType;

    public:
        explicit FilterComboBox(enum eColumnType columnType);
        void AddRelatedInput(CustomColumnLineEdit* input);
        CustomColumnLineEdit* getWidgetAt(int nIndex);
        enum eColumnType getColumnType();

    public slots:
        void ToggleWidgetState(int comboboxIndex);
};

class FiltersDialog : public QDialog
{
    Q_OBJECT

    public:
        explicit FiltersDialog(QString sRequest, QWidget* parent = nullptr);
        ~FiltersDialog();

        QString FiltersToSQLRequest();
        void SQLRequestToFilters(QString sRequest);

    private:
        Ui::FiltersDialog* m_ui;
        QList<FilterComboBox*> m_filters;
};

#endif // FILTERSDIALOG_H
