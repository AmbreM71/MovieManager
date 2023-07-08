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

        void InsertColumn(struct stColumn* stColumnToInsert);
        void InsertColumnQt(QString sName, enum eColumnType eType);
        void InsertColumnDB(struct stColumn* stColumnToInsert);

        void RemoveColumnQt(QString sName);
        void RemoveColumnDB(QString sName);

    public slots:
        void AddColumn();
        void EditColumn(QString sName);
        void RemoveColumn(QString sName);
};

#endif // OPTIONSDIALOG_H
