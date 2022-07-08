#ifndef FILTERSDIALOG_H
#define FILTERSDIALOG_H

#include <QDialog>

namespace Ui {
class FiltersDialog;
}

class FiltersDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FiltersDialog(QString* name, int* minYear, int* maxYear, int* minRating, int* maxRating, int* minEntries, QWidget* parent = nullptr);
    ~FiltersDialog();

private:
    Ui::FiltersDialog* m_ui;
    QString* m_name;
    int *m_minYear, *m_maxYear, *m_minRating, *m_maxRating, *m_minEntries;
};

#endif // FILTERSDIALOG_H
