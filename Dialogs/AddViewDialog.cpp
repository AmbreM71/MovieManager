#include "AddViewDialog.h"
#include "ui_AddViewDialog.h"

AddViewDialog::AddViewDialog(QWidget *parent) : QDialog(parent) {
    m_ui = new Ui::AddViewDialog;
    m_ui->setupUi(this);

    m_ui->MovieViewDateInput->setDate(QDate::currentDate());

    m_ui->MovieReleaseYearInput->setMaximum(QDate::currentDate().year());
    m_ui->MovieReleaseYearInput->setValue(QDate::currentDate().year());

    FillMovieComboBox();

    m_ui->ButtonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

    QObject::connect(m_ui->ExistingMoviesComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(comboboxChanged()));
    QObject::connect(m_ui->UnknownViewDateCheckbox, SIGNAL(stateChanged(int)), this, SLOT(toggleViewDateInput(int)));
    QObject::connect(m_ui->UnknownViewTypeCheckbox, SIGNAL(stateChanged(int)), this, SLOT(toggleViewTypeInput(int)));

    //Connectors to check if input are filled to enable Ok button
    QObject::connect(m_ui->MovieNameInput, SIGNAL(textChanged(QString)), this, SLOT(checkValid()));
    QObject::connect(m_ui->ExistingMoviesComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(checkValid()));
}

AddViewDialog::~AddViewDialog() {
    delete m_ui;
}

void AddViewDialog::FillMovieComboBox() {
    m_ui->ExistingMoviesComboBox->addItem("");
    QSqlQuery moviesQuery;
    moviesQuery.exec("SELECT Name, ReleaseYear FROM movieViews GROUP BY Name, ReleaseYear ORDER BY Name ASC;");
    while(moviesQuery.next()) {
        m_ui->ExistingMoviesComboBox->addItem(moviesQuery.value(0).toString()+" - "+moviesQuery.value(1).toString());
    }
}

QString AddViewDialog::getComboboxSelectedItem() {
    return m_ui->ExistingMoviesComboBox->currentText();
}

QString AddViewDialog::getName() {
    return m_ui->MovieNameInput->text();
}
int AddViewDialog::getReleaseYear() {
    return m_ui->MovieReleaseYearInput->text().toInt();
}
QString AddViewDialog::getViewType() {
    return m_ui->ViewTypeComboBox->currentText();
}
QString AddViewDialog::getViewDate() {
    return m_ui->MovieViewDateInput->text();
}
int AddViewDialog::getRating() {
    return m_ui->MovieRatingInput->value();
}
int AddViewDialog::getEntries() {
    return m_ui->EntriesInput->value();
}

void AddViewDialog::comboboxChanged() {
    //If no movie is selected in combobox
    if(m_ui->ExistingMoviesComboBox->currentText() == "") {
        m_ui->MovieNameInput->setEnabled(true);
        m_ui->MovieReleaseYearInput->setEnabled(true);
        m_ui->MovieRatingInput->setEnabled(true);
        m_ui->EntriesInput->setEnabled(true);
    }
    else {
        m_ui->MovieNameInput->setEnabled(false);
        m_ui->MovieReleaseYearInput->setEnabled(false);
        m_ui->MovieRatingInput->setEnabled(false);
        m_ui->EntriesInput->setEnabled(false);
    }
}

void AddViewDialog::toggleViewDateInput(int state) {
    if(state == 2) {
        m_ui->MovieViewDateInput->setEnabled(false);
    }
    else {
        m_ui->MovieViewDateInput->setEnabled(true);
    }
}

void AddViewDialog::toggleViewTypeInput(int state) {
    if(state == 2) {
        m_ui->ViewTypeComboBox->setEnabled(false);
    }
    else {
        m_ui->ViewTypeComboBox->setEnabled(true);
    }
}

bool AddViewDialog::isDateUnknown() {
    return m_ui->UnknownViewDateCheckbox->isChecked();
}

bool AddViewDialog::isTypeUnknown() {
    return m_ui->UnknownViewTypeCheckbox->isChecked();
}

void AddViewDialog::checkValid() {
    //If movie isn't selected in combobox
    if(m_ui->ExistingMoviesComboBox->currentIndex() == 0) {
        if(m_ui->MovieNameInput->text() == "") {
            m_ui->ButtonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
        }
        else {
            m_ui->ButtonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
        }
    }
    else {
        m_ui->ButtonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
    }
}
