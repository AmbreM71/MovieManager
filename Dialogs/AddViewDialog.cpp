#include "AddViewDialog.h"
#include "ui_AddViewDialog.h"

AddViewDialog::AddViewDialog(QWidget *parent) : QDialog(parent) {
    m_ui = new Ui::AddViewDialog;
    m_tags = new QList<QString>;
    m_ui->setupUi(this);
    this->setFixedSize(600,407);

    m_ui->MovieViewDateInput->setDate(QDate::currentDate());

    m_ui->MovieReleaseYearInput->setMaximum(QDate::currentDate().year());
    m_ui->MovieReleaseYearInput->setValue(QDate::currentDate().year());

    FillMovieComboBox();

    m_ui->ButtonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

    for(int viewType = 0 ; viewType < MaxViewType ; viewType++) {
        m_ui->ViewTypeComboBox->addItem(Common::viewTypeToQString((enum ViewType)viewType));
    }

    QObject::connect(m_ui->ExistingMoviesComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(comboboxChanged()));
    QObject::connect(m_ui->UnknownViewDateCheckbox, SIGNAL(stateChanged(int)), this, SLOT(toggleViewDateInput(int)));
    QObject::connect(m_ui->UnknownViewTypeCheckbox, SIGNAL(stateChanged(int)), this, SLOT(toggleViewTypeInput(int)));
    QObject::connect(m_ui->UnknownEntriesCheckbox, SIGNAL(stateChanged(int)), this, SLOT(toggleEntriesInput(int)));
    QObject::connect(m_ui->PosterButton, SIGNAL(clicked()), this, SLOT(loadPoster()));
    QObject::connect(m_ui->TagsAddButton, SIGNAL(clicked()), this, SLOT(addTag()));

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
    moviesQuery.exec("SELECT Name, ReleaseYear FROM movies ORDER BY Name ASC;");
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
int AddViewDialog::getViewType() {
    return Common::QStringToViewType(m_ui->ViewTypeComboBox->currentText());
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
        m_ui->TagsAddButton->setEnabled(true);
        m_ui->TagsInput->setEnabled(true);

        for(int i = 0 ; i < m_ui->TagsLayout->count()-1 ; i++) {
            Tag* tag = (Tag*)m_ui->TagsLayout->itemAt(i)->widget();
            tag->setStyleSheet(
                        "   background-color : #653133;"
                        "   color : #d17579;"
                        "   padding : 1px 5px 3px 5px;"
                        "   border-radius:12px;"
                        "   border: 2px solid #653133;");
        }

        m_ui->PosterLabel->setText(tr("Affiche"));
        m_posterPath = "";
    }
    else {
        m_ui->MovieNameInput->setEnabled(false);
        m_ui->MovieReleaseYearInput->setEnabled(false);
        m_ui->MovieRatingInput->setEnabled(false);
        m_ui->EntriesInput->setEnabled(false);
        m_ui->PosterButton->setEnabled(false);
        m_ui->TagsAddButton->setEnabled(false);
        m_ui->TagsInput->setEnabled(false);

        for(int i = 0 ; i < m_ui->TagsLayout->count()-1 ; i++) {
            Tag* tag = (Tag*)m_ui->TagsLayout->itemAt(i)->widget();
            tag->setStyleSheet(
                        "   background-color : #444444;"
                        "   color : #bbbbbb;"
                        "   padding : 1px 5px 3px 5px;"
                        "   border-radius:12px;"
                        "   border: 2px solid #444444;");
        }

        QString movieName = m_ui->ExistingMoviesComboBox->currentText().remove(m_ui->ExistingMoviesComboBox->currentText().length()-7, m_ui->ExistingMoviesComboBox->currentText().length());
        QString movieYear = m_ui->ExistingMoviesComboBox->currentText().remove(0, m_ui->ExistingMoviesComboBox->currentText().length()-4);

        QSqlQuery posterQuery;
        posterQuery.exec("SELECT Poster FROM movies WHERE Name=\""+movieName+"\" AND ReleaseYear='"+movieYear+"'");
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

void AddViewDialog::toggleEntriesInput(int state) {
    if(state == 2) {
        m_ui->EntriesInput->setEnabled(false);
    }
    else {
        m_ui->EntriesInput->setEnabled(true);
    }
}

bool AddViewDialog::isDateUnknown() {
    return m_ui->UnknownViewDateCheckbox->isChecked();
}

bool AddViewDialog::isTypeUnknown() {
    return m_ui->UnknownViewTypeCheckbox->isChecked();
}

bool AddViewDialog::isEntriesUnknown() {
    return m_ui->UnknownEntriesCheckbox->isChecked();
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
    Common::loadPoster(this, m_ui->PosterLabel, 260, 1.4, path, &m_posterPath);
}

void AddViewDialog::addTag() {
    if(m_ui->TagsInput->text().length() != 0) {
        m_tags->append(m_ui->TagsInput->text());
        Tag* tag = new Tag(m_ui->TagsInput->text());

        m_ui->TagsLayout->insertWidget(m_ui->TagsLayout->count()-1,tag,0,Qt::AlignLeft);
        m_ui->TagsInput->clear();

        QObject::connect(tag, SIGNAL(clicked(Tag*)), this, SLOT(clickedTag(Tag*)));
        QObject::connect(tag, SIGNAL(mouseEnter(Tag*)), this, SLOT(mouseEnteredTag(Tag*)));
        QObject::connect(tag, SIGNAL(mouseLeave(Tag*)), this, SLOT(mouseLeftTag(Tag*)));
    }
}

QList<QString>* AddViewDialog::getTags() {
    return m_tags;
}

void AddViewDialog::clickedTag(Tag* tag) {
    for(int i = 0 ; i < m_tags->length() ; i++) {
        if(QString::compare(tag->getSavedTag(), m_tags->at(i)) == 0) {
            m_tags->removeAt(i);
        }
    }

    delete tag;
}

void AddViewDialog::mouseEnteredTag(Tag* tag) {
    int width = tag->width();
    tag->setSavedTag(tag->text());
    tag->setText("❌");
    tag->setMinimumWidth(width);
}

void AddViewDialog::mouseLeftTag(Tag* tag) {
    tag->setMinimumWidth(31);
    tag->setText(tag->getSavedTag());
}
