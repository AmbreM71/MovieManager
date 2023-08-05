#ifndef OPTIONSDIALOG_H
#define OPTIONSDIALOG_H

#include <QDialog>
#include <QSignalMapper>

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
        QSignalMapper* m_editColumnSignalMapper;
        QSignalMapper* m_deleteColumnSignalMapper;

    public:
        explicit OptionsDialog(QWidget* parent = nullptr);
        ~OptionsDialog();

        void InsertColumn(struct stColumn* stColumnToInsert, int nRow);
        void InsertColumnQt(QString sName, enum eColumnType eType, int nRow);
        void InsertColumnDB(struct stColumn* stColumnToInsert);

        void RemoveColumnQt(int nRow);
        void RemoveColumnDB(QString sName);

    public slots:
        void AddColumn();
        void EditColumn(int nRow);
        void RemoveColumn(int nRow);
};

#endif // OPTIONSDIALOG_H
