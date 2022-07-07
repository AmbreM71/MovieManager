#include "AddViewDialog.h"
#include "ui_AddViewDialog.h"

AddViewDialog::AddViewDialog(QWidget *parent) : QDialog(parent) {
    m_ui = new Ui::AddViewDialog;
    m_ui->setupUi(this);
    FillMovieComboBox();

    QObject::connect(m_ui->ExistingMoviesComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(comboboxChanged()));
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

void AddViewDialog::comboboxChanged() {
    //If no movie is selected in combobox
    if(m_ui->ExistingMoviesComboBox->currentText() == "") {
        m_ui->MovieNameInput->setEnabled(true);
        m_ui->MovieReleaseYearInput->setEnabled(true);
        m_ui->MovieRatingInput->setEnabled(true);
    }
    else {
        m_ui->MovieNameInput->setEnabled(false);
        m_ui->MovieReleaseYearInput->setEnabled(false);
        m_ui->MovieRatingInput->setEnabled(false);
    }
}
