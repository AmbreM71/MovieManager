#include "EditMovieDialog.h"
#include "ui_EditMovieDialog.h"

EditMovieDialog::EditMovieDialog(QString ID, QWidget *parent) : QDialog(parent) {
    m_ui = new Ui::EditMovieDialog;
    m_tags = new QList<QString>;
    m_customColumnsInputList = new QList<QWidget*>;
    m_customColumnsNameList = new QList<QString>;
    m_ui->setupUi(this);
    m_ID = &ID;
    this->setWindowIcon(QIcon(":/assets/Assets/Icons/Dark/edit.png"));
    m_ui->TagsInput->installEventFilter(this);

    m_tagsScrollArea = new TagsScrollArea(this);
    m_ui->FormLayout->addWidget(m_tagsScrollArea, 6, 0, 1, 2);

    QSqlQuery movieQuery;
    if(!movieQuery.exec("SELECT Name, ReleaseYear, Rating, Poster FROM movies WHERE ID='"+*m_ID+"'"))
        Common::LogDatabaseError(&movieQuery);
    movieQuery.first();

#ifdef DEV
    m_posterPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "\\MovieManager_Dev\\Posters\\"+movieQuery.value(3).toString();
#else
    m_posterPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "\\MovieManager\\Posters\\"+movieQuery.value(3).toString();
#endif

    loadPoster(m_posterPath);

    this->setWindowTitle(tr("Edit - %1").arg(movieQuery.value(0).toString()));

    m_ui->ReleaseYearInput->setMaximum(QDate::currentDate().year());

    m_ui->NameInput->setText(movieQuery.value(0).toString());
    m_ui->ReleaseYearInput->setValue(movieQuery.value(1).toInt());
    m_ui->RatingInput->setValue(movieQuery.value(2).toInt());

    QObject::connect(m_ui->PosterButton, SIGNAL(clicked()), this, SLOT(loadPoster()));
    QObject::connect(m_ui->TagsAddButton, SIGNAL(clicked()), this, SLOT(addTag()));

    QSqlQuery customColumnsQuery;
    QString sCustomColumns;
    int nCustomColumnCount = 0;
    QStringList sCustomColumnsNameList;

    if(!customColumnsQuery.exec("SELECT Name, Type, Min, Max, Precision, TextMaxLength FROM columns;"))
        Common::LogDatabaseError(&customColumnsQuery);
    int nColumnIndex = 0;
    while(customColumnsQuery.next()) {
        QLabel* columnLabel = new QLabel(customColumnsQuery.value(0).toString());
        m_ui->CustomColumnsLabelLayout->addWidget(columnLabel, nColumnIndex);
        m_customColumnsNameList->append(columnLabel->text());

        sCustomColumns.append(" \"" + customColumnsQuery.value(0).toString() + "\",");
        nCustomColumnCount++;
        sCustomColumnsNameList << customColumnsQuery.value(0).toString();

        if(customColumnsQuery.value(1).toInt() == 0) {
            // Int
            QSpinBox* input = new QSpinBox();
            input->setMinimum(customColumnsQuery.value(2).toInt());
            input->setMaximum(customColumnsQuery.value(3).toInt());
            m_ui->CustomColumnsInputLayout->addWidget(input, nColumnIndex);
            m_customColumnsInputList->append(input);
        }
        else if(customColumnsQuery.value(1).toInt() == 1) {
            // Double
            QDoubleSpinBox* input = new QDoubleSpinBox();
            input->setMinimum(customColumnsQuery.value(2).toDouble());
            input->setMaximum(customColumnsQuery.value(3).toDouble());
            input->setDecimals(customColumnsQuery.value(4).toDouble());
            m_ui->CustomColumnsInputLayout->addWidget(input, nColumnIndex);
            m_customColumnsInputList->append(input);
        }
        else if(customColumnsQuery.value(1).toInt() == 2) {
            // Text
            QLineEdit* input = new QLineEdit();
            input->setMaxLength(customColumnsQuery.value(5).toInt());
            m_ui->CustomColumnsInputLayout->addWidget(input, nColumnIndex);
            m_customColumnsInputList->append(input);
        }
        nColumnIndex++;
    }
    sCustomColumns.removeLast(); // Removes the last ","

    // Add custom columns informations
    QString sRequest = "SELECT" + sCustomColumns + " FROM movies WHERE ID='"+ID+"'";
    QSqlQuery customColumnsInformationsQuery;
    if(!customColumnsInformationsQuery.exec(sRequest))
        Common::LogDatabaseError(&customColumnsInformationsQuery);
    customColumnsInformationsQuery.first();

    for(int nColumn = 0; nColumn < nCustomColumnCount; nColumn++)
    {
        if(qobject_cast<QLineEdit*>(m_customColumnsInputList->at(nColumn)) != nullptr)
        {
            QLineEdit* input = qobject_cast<QLineEdit*>(m_customColumnsInputList->at(nColumn));
            input->setText(customColumnsInformationsQuery.value(nColumn).toString());
        }
        else if(qobject_cast<QSpinBox*>(m_customColumnsInputList->at(nColumn)) != nullptr)
        {
            QSpinBox* input = qobject_cast<QSpinBox*>(m_customColumnsInputList->at(nColumn));
            input->setValue(customColumnsInformationsQuery.value(nColumn).toInt());
        }
        else if(qobject_cast<QDoubleSpinBox*>(m_customColumnsInputList->at(nColumn)) != nullptr)
        {
            QDoubleSpinBox* input = qobject_cast<QDoubleSpinBox*>(m_customColumnsInputList->at(nColumn));
            input->setValue(customColumnsInformationsQuery.value(nColumn).toDouble());
        }
    }

    this->setFixedSize(this->sizeHint());

    QLayoutItem* spacer = m_tagsScrollArea->widget()->layout()->takeAt(m_tagsScrollArea->widget()->layout()->count()-1);

    QSqlQuery tagsQuery;
    if(!tagsQuery.exec("SELECT Tag FROM tags WHERE ID_Movie='"+*m_ID+"'"))
        Common::LogDatabaseError(&tagsQuery);
    while(tagsQuery.next()) {
        m_tags->append(tagsQuery.value(0).toString());
        Tag* tag = new Tag(tagsQuery.value(0).toString());

        QObject::connect(tag, SIGNAL(clicked(Tag*)), this, SLOT(clickedTag(Tag*)));
        QObject::connect(tag, SIGNAL(mouseEnter(Tag*)), this, SLOT(mouseEnteredTag(Tag*)));
        QObject::connect(tag, SIGNAL(mouseLeave(Tag*)), this, SLOT(mouseLeftTag(Tag*)));

        m_tagsScrollArea->widget()->layout()->addWidget(tag);
    }
    m_tagsScrollArea->widget()->layout()->addItem(spacer);
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

void EditMovieDialog::loadPoster(QString path) {
    QString tmp = m_posterPath;
    float ratio = (float)m_ui->PosterLabel->sizeHint().height() / (float)m_ui->PosterLabel->sizeHint().width();
    Common::loadPoster(this, m_ui->PosterLabel, m_ui->PosterLabel->sizeHint().height(), ratio, path, &m_posterPath);
    if (QString::compare(tmp, m_posterPath) != 0) {
        m_newPoster = true;
    }
}

void EditMovieDialog::addTag() {
    if(m_ui->TagsInput->text().length() != 0) {
        m_tags->append(m_ui->TagsInput->text());
        Tag* tag = new Tag(m_ui->TagsInput->text());

        QLayoutItem* spacer = m_tagsScrollArea->widget()->layout()->takeAt(m_tagsScrollArea->widget()->layout()->count()-1);
        m_tagsScrollArea->widget()->layout()->addWidget(tag);
        m_tagsScrollArea->widget()->layout()->addItem(spacer);
        m_ui->TagsInput->clear();

        QObject::connect(tag, SIGNAL(clicked(Tag*)), this, SLOT(clickedTag(Tag*)));
        QObject::connect(tag, SIGNAL(mouseEnter(Tag*)), this, SLOT(mouseEnteredTag(Tag*)));
        QObject::connect(tag, SIGNAL(mouseLeave(Tag*)), this, SLOT(mouseLeftTag(Tag*)));
    }
}

QList<QString>* EditMovieDialog::getTags() {
    return m_tags;
}

void EditMovieDialog::clickedTag(Tag* tag) {

    for(int i = 0 ; i < m_tags->length() ; i++) {
        if(QString::compare(tag->getSavedTag(), m_tags->at(i)) == 0) {
            m_tags->removeAt(i);
        }
    }

    delete tag;
}

void EditMovieDialog::mouseEnteredTag(Tag* tag) {
    tag->setSavedTag(tag->text());
    tag->setText("❌");
}

void EditMovieDialog::mouseLeftTag(Tag* tag) {
    tag->setText(tag->getSavedTag());
}

QList<QWidget*>* EditMovieDialog::getCustomColumnsInputList() {
    return m_customColumnsInputList;
}

QList<QString>* EditMovieDialog::getCustomColumnsNameList() {
    return m_customColumnsNameList;
}

bool EditMovieDialog::eventFilter(QObject *obj, QEvent *event) {
    if (obj == m_ui->TagsInput && event->type() == QEvent::KeyPress) {
        QKeyEvent* key = static_cast<QKeyEvent*>(event);
        if(key->key() == Qt::Key_Return) {
            addTag();
            return true;
        }
    }
    return false;
}
