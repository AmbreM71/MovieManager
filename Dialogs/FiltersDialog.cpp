#include "FiltersDialog.h"
#include "ui_FiltersDialog.h"

FiltersDialog::FiltersDialog(stFilters* filters, QWidget *parent) : QDialog(parent) {
    m_ui = new Ui::FiltersDialog;
    m_ui->setupUi(this);
    this->setWindowIcon(QIcon(":/assets/Assets/Icons/Dark/search.png"));

    m_customColumnsMap = new QMap<QWidget*, QString>();
    m_customColumnsUnknownCheckBox = new QList<QCheckBox*>();

    m_ui->MaxYearInput->setValue(QDate::currentDate().year());
    m_ui->MaxYearInput->setMaximum(QDate::currentDate().year());
    m_ui->MinYearInput->setMaximum(QDate::currentDate().year());

    m_filters = filters;

    m_ui->NameInput->setText(filters->sName);
    m_ui->MinYearInput->setValue(filters->nMinYear);
    m_ui->MaxYearInput->setValue(filters->nMaxYear);
    m_ui->MinRatingInput->setValue(filters->nMinRating);
    m_ui->MaxRatingInput->setValue(filters->nMaxRating);

    //m_ui->FormLayout
    QSqlQuery customColumnsQuery;
    if(!customColumnsQuery.exec("SELECT Name, Type, Min, Max, Precision, TextMaxLength FROM columns;"))
        Common::LogDatabaseError(&customColumnsQuery);
    int nColumnIndex = 0;
    while(customColumnsQuery.next()) {
        QLabel* columnLabel = new QLabel(customColumnsQuery.value(0).toString());
        m_ui->FormLayout->addWidget(columnLabel, 3 + nColumnIndex, 0);

        if(customColumnsQuery.value(1).toInt() == 0) {
            // Int
            QSpinBox* lowValue = new QSpinBox();
            QSpinBox* highValue = new QSpinBox();
            lowValue->setMinimum(customColumnsQuery.value(2).toInt());
            lowValue->setMaximum(customColumnsQuery.value(3).toInt());
            lowValue->setValue(customColumnsQuery.value(2).toInt());
            highValue->setMinimum(customColumnsQuery.value(2).toInt());
            highValue->setMaximum(customColumnsQuery.value(3).toInt());
            highValue->setValue(customColumnsQuery.value(3).toInt());

            m_ui->FormLayout->addWidget(lowValue, 3 + nColumnIndex, 1);
            m_ui->FormLayout->addWidget(highValue, 3 + nColumnIndex, 3);
            QLabel* andLabel = new QLabel(tr("and"));
            andLabel->setAlignment(Qt::AlignCenter);
            m_ui->FormLayout->addWidget(andLabel, 3 + nColumnIndex, 2);
            m_customColumnsMap->insert(lowValue, columnLabel->text());
            m_customColumnsMap->insert(highValue, columnLabel->text());
        }
        else if(customColumnsQuery.value(1).toInt() == 1) {
            // Double
            QDoubleSpinBox* lowValue = new QDoubleSpinBox();
            QDoubleSpinBox* highValue = new QDoubleSpinBox();
            lowValue->setMinimum(customColumnsQuery.value(2).toDouble());
            lowValue->setMaximum(customColumnsQuery.value(3).toDouble());
            lowValue->setValue(customColumnsQuery.value(2).toDouble());
            lowValue->setDecimals(customColumnsQuery.value(4).toInt());
            highValue->setMinimum(customColumnsQuery.value(2).toDouble());
            highValue->setMaximum(customColumnsQuery.value(3).toDouble());
            highValue->setValue(customColumnsQuery.value(3).toDouble());
            highValue->setDecimals(customColumnsQuery.value(4).toInt());

            m_ui->FormLayout->addWidget(lowValue, 3 + nColumnIndex, 1);
            m_ui->FormLayout->addWidget(highValue, 3 + nColumnIndex, 3);
            QLabel* andLabel = new QLabel(tr("and"));
            andLabel->setAlignment(Qt::AlignCenter);
            m_ui->FormLayout->addWidget(andLabel, 3 + nColumnIndex, 2);
            m_customColumnsMap->insert(lowValue, columnLabel->text());
            m_customColumnsMap->insert(highValue, columnLabel->text());
        }
        else if(customColumnsQuery.value(1).toInt() == 2) {
            // Text
            QLineEdit* input = new QLineEdit();
            input->setMaxLength(customColumnsQuery.value(5).toInt());
            m_ui->FormLayout->addWidget(input, 3 + nColumnIndex, 1, 1, 3);
            m_customColumnsMap->insert(input, columnLabel->text());
        }
        QCheckBox* unknown = new QCheckBox();
        unknown->setText(tr("Unknown only"));
        m_ui->FormLayout->addWidget(unknown, 3 + nColumnIndex, 4);
        m_customColumnsUnknownCheckBox->append(unknown);

        nColumnIndex++;
    }
    this->adjustSize();
}

FiltersDialog::~FiltersDialog() {
    m_filters->sName = m_ui->NameInput->text();
    m_filters->nMinYear = m_ui->MinYearInput->value();
    m_filters->nMaxYear = m_ui->MaxYearInput->value();
    m_filters->nMinRating = m_ui->MinRatingInput->value();
    m_filters->nMaxRating = m_ui->MaxRatingInput->value();
    delete m_ui;
}

QMap<QWidget*, QString>* FiltersDialog::getCustomColumnsMap() {
    return m_customColumnsMap;
}

QList<QCheckBox*>* FiltersDialog::getCustomColumnsUnknownCheckBox() {
    return m_customColumnsUnknownCheckBox;
}
