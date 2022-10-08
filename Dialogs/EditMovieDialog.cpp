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

    m_posterPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "\\MovieManager\\Posters\\"+movieQuery.value(4).toString();
    loadPoster(m_posterPath);

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

void EditMovieDialog::loadPoster(QString path) {
    QString tmp = m_posterPath;
    Common::loadPoster(this, m_ui->PosterLabel, 150, 1.33, path, &m_posterPath);
    if (QString::compare(tmp, m_posterPath) != 0) {
        m_newPoster = true;
    }
}
