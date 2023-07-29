#include "OptionsDialog.h"
#include "ui_OptionsDialog.h"

OptionsDialog::OptionsDialog(QWidget *parent) : QDialog(parent) {
    m_ui = new Ui::OptionsDialog;
    m_ui->setupUi(this);
    this->setWindowIcon(QIcon(":/assets/Assets/Icons/Dark/settings.png"));

    // Set current settings values
    m_ui->ThemeCombobox->setCurrentIndex((enum eTheme)Common::Settings->value("theme").toInt());
    m_ui->LanguageCombobox->setCurrentIndex((enum eLanguage)Common::Settings->value("language").toInt());
    m_ui->MatrixModeCheckbox->setChecked(Common::Settings->value("matrixMode").toBool());
    m_ui->QuickSearchCaseCheckbox->setChecked(Common::Settings->value("quickSearchCaseSensitive").toBool());
    m_ui->MoreLogsCheckbox->setChecked(Common::Settings->value("moreLogs").toBool());
    m_ui->DateFormatCombobox->setCurrentIndex(m_ui->DateFormatCombobox->findText(Common::Settings->value("dateFormat").toString()));

    m_ui->scrollAreaLayout->addStretch(100);

    // Add custom columns to the list
    QSqlQuery columnsQuery;
    if(!columnsQuery.exec("SELECT Name, Type FROM columns ORDER BY Name"))
        Common::LogDatabaseError(&columnsQuery);

    m_editColumnSignalMapper = new QSignalMapper();
    m_deleteColumnSignalMapper = new QSignalMapper();

    while(columnsQuery.next())
        InsertColumnQt(columnsQuery.value(0).toString(), (enum eColumnType)columnsQuery.value(1).toInt());

    QObject::connect(m_editColumnSignalMapper, SIGNAL(mappedString(QString)), this, SLOT(EditColumn(QString)));
    QObject::connect(m_deleteColumnSignalMapper, SIGNAL(mappedString(QString)), this, SLOT(RemoveColumn(QString)));

    QObject::connect(m_ui->AddColumnButton, SIGNAL(clicked()), this, SLOT(AddColumn()));
}

OptionsDialog::~OptionsDialog() {
    Common::Settings->setValue("language", (enum eLanguage)m_ui->LanguageCombobox->currentIndex());
    Common::Settings->setValue("theme", (enum eTheme)m_ui->ThemeCombobox->currentIndex());
    Common::Settings->setValue("matrixMode", m_ui->MatrixModeCheckbox->isChecked());
    Common::Settings->setValue("quickSearchCaseSensitive", m_ui->QuickSearchCaseCheckbox->isChecked());
    Common::Settings->setValue("moreLogs", m_ui->MoreLogsCheckbox->isChecked());
    Common::Settings->setValue("dateFormat", m_ui->DateFormatCombobox->currentText());
    delete m_ui;
}


void OptionsDialog::AddColumn() {
    AddColumnDialog* window = new AddColumnDialog(this);
    window->show();
    if(window->exec() == 1) {
        InsertColumn(window->getColumn());
    }
}

void OptionsDialog::EditColumn(QString sName) {

    QSqlQuery columnsQuery;
    if(!columnsQuery.exec("SELECT Name, Type, Min, Max, Precision, TextMaxLength FROM columns WHERE Name = \"" + sName + "\""))
        Common::LogDatabaseError(&columnsQuery);
    columnsQuery.first();

    struct stColumn stColumnToEdit;
    stColumnToEdit.sName = columnsQuery.value(0).toString();
    stColumnToEdit.eType = (enum eColumnType)columnsQuery.value(1).toInt();
    stColumnToEdit.nMin = columnsQuery.value(2).toDouble();
    stColumnToEdit.nMax = columnsQuery.value(3).toDouble();
    stColumnToEdit.nPrecision = columnsQuery.value(4).toInt();
    stColumnToEdit.textMaxLength = columnsQuery.value(5).toInt();

    AddColumnDialog* window = new AddColumnDialog(this, &stColumnToEdit);
    window->show();
    if(window->exec() == 1) {
        RemoveColumn(sName);
        InsertColumn(window->getColumn());
    }
}

void OptionsDialog::RemoveColumn(QString sName) {
    RemoveColumnQt(sName);
    RemoveColumnDB(sName);
}

void OptionsDialog::RemoveColumnQt(QString sName) {
    QGridLayout* columnLayout;
    QLabel* sNameLabel;

    int nRow;
    for(nRow = 1; nRow < m_ui->scrollAreaLayout->count()-1; nRow++) {
        columnLayout = (QGridLayout*)(m_ui->scrollAreaLayout->itemAt(nRow)->layout());
        sNameLabel = qobject_cast<QLabel*>(columnLayout->itemAtPosition(0,0)->widget());
        if(QString::compare(sNameLabel->text(), sName) == 0)
            break;
    }

    QLabel* sType = qobject_cast<QLabel*>(columnLayout->itemAtPosition(0,1)->widget());
    QHBoxLayout* pActionLayout = (QHBoxLayout*)(columnLayout->itemAtPosition(0,2)->layout());

    QPushButton* editButton = qobject_cast<QPushButton*>(pActionLayout->itemAt(0)->widget());
    QPushButton* deleteButton = qobject_cast<QPushButton*>(pActionLayout->itemAt(1)->widget());

    m_ui->scrollAreaLayout->removeItem(columnLayout);
    columnLayout->removeWidget(sNameLabel);
    columnLayout->removeWidget(sType);
    columnLayout->removeItem(pActionLayout);
    columnLayout->removeWidget(editButton);
    columnLayout->removeWidget(deleteButton);
    delete columnLayout;
    delete sNameLabel;
    delete sType;
    delete pActionLayout;
    delete editButton;
    delete deleteButton;
}

void OptionsDialog::RemoveColumnDB(QString sName) {
    QSqlQuery deleteColumnsQuery;
    if(!deleteColumnsQuery.exec("DELETE FROM columns WHERE Name = \"" + sName + "\""))
        Common::LogDatabaseError(&deleteColumnsQuery);
}

void OptionsDialog::InsertColumn(struct stColumn* stColumnToInsert) {
    InsertColumnQt(stColumnToInsert->sName, stColumnToInsert->eType);
    InsertColumnDB(stColumnToInsert);
}

void OptionsDialog::InsertColumnQt(QString sName, enum eColumnType eType) {
    QGridLayout* pColumnLayout = new QGridLayout();

    QLabel* NameLabel = new QLabel(sName);
    QLabel* TypeLabel = new QLabel(Common::ColumnTypeToQString(eType));
    QPushButton* editButton = new QPushButton(tr("Edit"));
    QPushButton* deleteButton = new QPushButton();
    Common::setIconAccordingToTheme(deleteButton, "delete.png");
    deleteButton->setMaximumWidth(30);

    QHBoxLayout* pActionsLayout = new QHBoxLayout();

    pActionsLayout->addWidget(editButton);
    pActionsLayout->addWidget(deleteButton);

    pColumnLayout->addWidget(NameLabel,0,0,1,1);
    pColumnLayout->addWidget(TypeLabel,0,1,1,1);
    pColumnLayout->addLayout(pActionsLayout,0,2,1,1);

    m_editColumnSignalMapper->setMapping(editButton, NameLabel->text());
    QObject::connect(editButton, SIGNAL(clicked()), m_editColumnSignalMapper, SLOT(map()));

    m_deleteColumnSignalMapper->setMapping(deleteButton, NameLabel->text());
    QObject::connect(deleteButton, SIGNAL(clicked()), m_deleteColumnSignalMapper, SLOT(map()));

    // nRow-1 to insert before the stretch
    m_ui->scrollAreaLayout->insertLayout(m_ui->scrollAreaLayout->count()-1, pColumnLayout);

}

void OptionsDialog::InsertColumnDB(struct stColumn* stColumnToInsert) {
    QSqlQuery insertColumnQuery;
    switch(stColumnToInsert->eType) {
    case eColumnType::eColumnDouble:
        insertColumnQuery.prepare("INSERT INTO columns (Name, Type, Min, Max, Precision) VALUES (?,?,?,?,?);");
        insertColumnQuery.bindValue(0, stColumnToInsert->sName);
        insertColumnQuery.bindValue(1, stColumnToInsert->eType);
        insertColumnQuery.bindValue(2, stColumnToInsert->nMin);
        insertColumnQuery.bindValue(3, stColumnToInsert->nMax);
        insertColumnQuery.bindValue(4, stColumnToInsert->nPrecision);
        break;
    case eColumnType::eColumnInteger:
        insertColumnQuery.prepare("INSERT INTO columns (Name, Type, Min, Max) VALUES (?,?,?,?);");
        insertColumnQuery.bindValue(0, stColumnToInsert->sName);
        insertColumnQuery.bindValue(1, stColumnToInsert->eType);
        insertColumnQuery.bindValue(2, stColumnToInsert->nMin);
        insertColumnQuery.bindValue(3, stColumnToInsert->nMax);
        break;
    case eColumnType::eColumnText:
        insertColumnQuery.prepare("INSERT INTO columns (Name, Type, TextMaxLength) VALUES (?,?,?);");
        insertColumnQuery.bindValue(0, stColumnToInsert->sName);
        insertColumnQuery.bindValue(1, stColumnToInsert->eType);
        insertColumnQuery.bindValue(2, stColumnToInsert->textMaxLength);
        break;
    }

    if(!insertColumnQuery.exec()){
        Common::LogDatabaseError(&insertColumnQuery);
        return;
    }
}

