#include "AddViewDialog.h"
#include "ui_AddViewDialog.h"

AddViewDialog::AddViewDialog(QWidget *parent) : QDialog(parent) {
    m_ui = new Ui::AddViewDialog;
    m_ui->setupUi(this);
    this->setFixedSize(600,351);

    m_ui->MovieViewDateInput->setDate(QDate::currentDate());

    m_ui->MovieReleaseYearInput->setMaximum(QDate::currentDate().year());
    m_ui->MovieReleaseYearInput->setValue(QDate::currentDate().year());

    FillMovieComboBox();

    m_ui->ButtonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

    QObject::connect(m_ui->ExistingMoviesComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(comboboxChanged()));
    QObject::connect(m_ui->UnknownViewDateCheckbox, SIGNAL(stateChanged(int)), this, SLOT(toggleViewDateInput(int)));
    QObject::connect(m_ui->UnknownViewTypeCheckbox, SIGNAL(stateChanged(int)), this, SLOT(toggleViewTypeInput(int)));
    QObject::connect(m_ui->PosterButton, SIGNAL(clicked()), this, SLOT(loadPoster()));

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
    moviesQuery.exec("SELECT Name, ReleaseYear FROM movies ASC;");
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
QString AddViewDialog::getPosterPath() {
    return m_posterPath;
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
        m_ui->PosterButton->setEnabled(true);
        if (m_ui->PosterLabel->text() == tr("Affiche"))
            loadPoster(m_posterPath);
        else {
            m_ui->PosterLabel->setText(tr("Affiche"));
            m_posterPath = "";
        }

    }
    else {
        m_ui->MovieNameInput->setEnabled(false);
        m_ui->MovieReleaseYearInput->setEnabled(false);
        m_ui->MovieRatingInput->setEnabled(false);
        m_ui->EntriesInput->setEnabled(false);
        m_ui->PosterButton->setEnabled(false);

        QString movieName = m_ui->ExistingMoviesComboBox->currentText().remove(m_ui->ExistingMoviesComboBox->currentText().length()-7, m_ui->ExistingMoviesComboBox->currentText().length());
        QString movieYear = m_ui->ExistingMoviesComboBox->currentText().remove(0, m_ui->ExistingMoviesComboBox->currentText().length()-4);

        QSqlQuery posterQuery;
        posterQuery.exec("SELECT Poster FROM movies WHERE Name='"+movieName+"' AND ReleaseYear='"+movieYear+"'");
        posterQuery.first();



        loadPoster(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "\\MovieManager\\Posters\\"+posterQuery.value(0).toString());
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

void AddViewDialog::loadPoster(QString path) {
    if (path == "") {
        bool extOK;
        do {
            QString temp = "";
            extOK = true;
            temp = QFileDialog::getOpenFileName(this, tr("Selectionner une affiche"), QString(), "Image (*.png; *.jpg; *.webp)");

            QString ext = temp;
            ext = ext.remove(0, temp.lastIndexOf(".")+1);
            // Test if file is a jpg or a png
            if(QString::compare(ext, "png") != 0 && QString::compare(ext, "jpg") && QString::compare(ext, "webp") != 0 && temp.size() > 0) {
                QMessageBox::critical(this, tr("Format incorrect"), tr("Le format de l'image est incorrect\nVeuillez sélectionner un fichier au format jpg, png ou webm"));
                extOK = false;
            }
            else {
                //This is to avoid spamming file selection dialog when closing dialog without selecting a file
                if(temp.size() > 0) {
                    m_posterPath = temp;
                }
            }
        } while (extOK == false);
    }
    else
        m_posterPath = path;

    if(m_posterPath != "") {
        QPixmap* pixmap = new QPixmap(m_posterPath);
        QPixmap pm;
        pm = pixmap->scaledToHeight(230, Qt::SmoothTransformation);
        m_ui->PosterLabel->setPixmap(pm);
    }
}
