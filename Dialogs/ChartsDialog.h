#ifndef CHARTSDIALOG_H
#define CHARTSDIALOG_H

#include <QDialog>
#include <QBarSet>

namespace Ui {
class ChartsDialog;
}

class ChartsDialog : public QDialog {
    Q_OBJECT

    private:
        Ui::ChartsDialog* m_ui;
        static int instances;

    public:
        explicit ChartsDialog(QWidget *parent = nullptr);
        static int instancesCount();
        ~ChartsDialog();


};

#endif // CHARTSDIALOG_H
