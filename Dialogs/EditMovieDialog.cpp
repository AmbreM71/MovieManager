#include "EditMovieDialog.h"
#include "ui_EditMovieDialog.h"

EditMovieDialog::EditMovieDialog(QString ID, QWidget *parent) : QDialog(parent) {
    m_ui = new Ui::EditMovieDialog;
    m_ui->setupUi(this);
    this->setFixedSize(400,200);
    m_ID = &ID;

    QSqlQuery movieQuery;
    movieQuery.exec("SELECT Name, ReleaseYear, Entries, Rating, Poster FROM movies WHERE ID='"+*m_ID+"'");
    movieQuery.first();


    PWSTR path;
    SHGetKnownFolderPath(FOLDERID_Documents, KF_FLAG_DEFAULT, NULL, &path);
    std::wstring wfile(path);
    QString posterpath = QString::fromStdWString(wfile) + "\\MovieManager\\Posters";
    QPixmap* pixmap = new QPixmap(posterpath+"\\"+movieQuery.value(4).toString());
    m_ui->PosterLabel->setPixmap(pixmap->scaledToHeight(150, Qt::SmoothTransformation));

    this->setWindowTitle(tr("Modifier - ") + movieQuery.value(0).toString());

    m_ui->ReleaseYearInput->setMaximum(QDate::currentDate().year());

    m_ui->NameInput->setText(movieQuery.value(0).toString());
    m_ui->ReleaseYearInput->setValue(movieQuery.value(1).toInt());
    m_ui->EntriesInput->setValue(movieQuery.value(2).toInt());
    m_ui->RatingInput->setValue(movieQuery.value(3).toInt());

    QObject::connect(m_ui->PosterButton, SIGNAL(clicked()), this, SLOT(loadPoster()));
}

EditMovieDialog::~EditMovieDialog() {
    delete m_ui;
}

bool EditMovieDialog::edited() {
    return m_edited;
}

bool EditMovieDialog::newPoster() {
    return m_newPoster;
}

QString EditMovieDialog::getMovieName() {
    return m_ui->NameInput->text();
}

QString EditMovieDialog::getReleaseYear() {
     return m_ui->ReleaseYearInput->text();
}

QString EditMovieDialog::getPosterPath() {
    return m_posterPath;
}

int EditMovieDialog::getRating() {
    return m_ui->RatingInput->value();
}

int EditMovieDialog::getEntries() {
    return m_ui->EntriesInput->value();
}

void EditMovieDialog::loadPoster() {
    m_posterPath = "";
    m_posterPath = QFileDialog::getOpenFileName(this, tr("Selectionner une affiche"), QString(), "Image (*.png; *.jpg)");
    if(m_posterPath != "") {
        m_newPoster = true;
        QPixmap* pixmap = new QPixmap(m_posterPath);
        QPixmap pm;
        pm = pixmap->scaledToHeight(150, Qt::SmoothTransformation);
        m_ui->PosterLabel->setPixmap(pm);
    }
}
