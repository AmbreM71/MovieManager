#include "AddViewDialog.h"
#include "ui_AddViewDialog.h"

AddViewDialog::AddViewDialog(QWidget *parent, int nMovieID) : QDialog(parent) {
    m_ui = new Ui::AddViewDialog;
    m_tags = new QList<QString>;
    m_customColumnsInputList = new QList<QWidget*>;
    m_customColumnsNameList = new QList<QString>;
    m_ui->setupUi(this);
    this->setWindowIcon(QIcon(":/assets/Assets/Icons/Dark/plus.png"));
    m_ui->TagsInput->installEventFilter(this);

    m_ui->MovieViewDateInput->setDate(QDate::currentDate());
    m_ui->MovieReleaseYearInput->setMaximum(QDate::currentDate().year());
    m_ui->MovieReleaseYearInput->setValue(QDate::currentDate().year());
    m_ui->MovieViewDateInput->setDisplayFormat(Common::Settings->value("dateFormat").toString());

    m_tagsScrollArea = new TagsScrollArea(this);
    m_ui->FormLayout->addWidget(m_tagsScrollArea, 9, 0, 1, 2);

    FillMovieComboBox();

    m_ui->ButtonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

    for(int viewType = 0 ; viewType < eViewType::MaxViewType ; viewType++) {
        m_ui->ViewTypeComboBox->addItem(Common::viewTypeToQString((enum eViewType)viewType));
    }



    QObject::connect(m_ui->ExistingMoviesComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(comboboxChanged()));
    QObject::connect(m_ui->UnknownViewDateCheckbox, SIGNAL(stateChanged(int)), this, SLOT(toggleViewDateInput(int)));
    QObject::connect(m_ui->UnknownViewTypeCheckbox, SIGNAL(stateChanged(int)), this, SLOT(toggleViewTypeInput(int)));
    QObject::connect(m_ui->PosterButton, SIGNAL(clicked()), this, SLOT(SelectPoster()));
    QObject::connect(m_ui->TagsAddButton, SIGNAL(clicked()), this, SLOT(addTag()));

    //Connectors to check if input are filled to enable Ok button
    QObject::connect(m_ui->MovieNameInput, SIGNAL(textChanged(QString)), this, SLOT(checkValid()));
    QObject::connect(m_ui->ExistingMoviesComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(checkValid()));

    QSqlQuery customColumnsQuery;
    if(!customColumnsQuery.exec("SELECT Name, Type, Min, Max, Precision, TextMaxLength FROM columns;"))
        Common::LogDatabaseError(&customColumnsQuery);

    int nColumnIndex = 0;
    while(customColumnsQuery.next()) {
        QLabel* columnLabel = new QLabel(customColumnsQuery.value(0).toString());
        m_ui->CustomColumnsLabelLayout->addWidget(columnLabel, nColumnIndex);
        m_customColumnsNameList->append(columnLabel->text());

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

    // If a movie is preselected when calling the dialog
    if(nMovieID != -1) {
        QSqlQuery moviesQuery;
        if(!moviesQuery.exec("SELECT Name, ReleaseYear FROM movies WHERE ID = '" + QString::number(nMovieID) + "';"))
            Common::LogDatabaseError(&moviesQuery);
        moviesQuery.first();
        m_ui->ExistingMoviesComboBox->setCurrentIndex(m_ui->ExistingMoviesComboBox->findText(moviesQuery.value(0).toString()+" - "+moviesQuery.value(1).toString()));
    }

    this->setFixedSize(this->sizeHint());
}

AddViewDialog::~AddViewDialog() {
    delete m_ui;
}

void AddViewDialog::FillMovieComboBox() {
    m_ui->ExistingMoviesComboBox->addItem("");
    QSqlQuery moviesQuery;
    if(!moviesQuery.exec("SELECT Name, ReleaseYear FROM movies ORDER BY Name ASC;"))
        Common::LogDatabaseError(&moviesQuery);
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
QDate AddViewDialog::getViewDate() {
    return m_ui->MovieViewDateInput->date();
}
QString AddViewDialog::getPosterPath() {
    return m_posterPath;
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
        m_ui->PosterButton->setEnabled(true);
        m_ui->TagsAddButton->setEnabled(true);
        m_ui->TagsInput->setEnabled(true);
        // Set state for custom columns
        for(int nWidget = 0; nWidget < m_customColumnsInputList->size(); nWidget++) {
            m_customColumnsInputList->at(nWidget)->setEnabled(true);
        }

        m_ui->PosterLabel->setText(tr("Poster"));
        m_posterPath = "";
    }
    else {
        m_ui->MovieNameInput->setEnabled(false);
        m_ui->MovieReleaseYearInput->setEnabled(false);
        m_ui->MovieRatingInput->setEnabled(false);
        m_ui->PosterButton->setEnabled(false);
        m_ui->TagsAddButton->setEnabled(false);
        m_ui->TagsInput->setEnabled(false);
        // Set state for custom columns
        for(int nWidget = 0; nWidget < m_customColumnsInputList->size(); nWidget++) {
            m_customColumnsInputList->at(nWidget)->setEnabled(false);
        }

        QString movieName = m_ui->ExistingMoviesComboBox->currentText().remove(m_ui->ExistingMoviesComboBox->currentText().length()-7, m_ui->ExistingMoviesComboBox->currentText().length());
        QString movieYear = m_ui->ExistingMoviesComboBox->currentText().remove(0, m_ui->ExistingMoviesComboBox->currentText().length()-4);

        QSqlQuery posterQuery;
        if(!posterQuery.exec("SELECT Poster FROM movies WHERE Name=\""+movieName+"\" AND ReleaseYear='"+movieYear+"'"))
            Common::LogDatabaseError(&posterQuery);
        posterQuery.first();

#ifdef DEV
        QString sPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "\\MovieManager_Dev\\Posters\\"+posterQuery.value(0).toString();
#else
        QString sPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "\\MovieManager\\Posters\\"+posterQuery.value(0).toString();
#endif
        Common::DisplayPoster(m_ui->PosterLabel, m_ui->PosterLabel->sizeHint().height(), m_ui->PosterLabel->sizeHint().height() / (float)m_ui->PosterLabel->sizeHint().width(), sPath);
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

void AddViewDialog::SelectPoster() {
    float ratio = (float)m_ui->PosterLabel->sizeHint().height() / (float)m_ui->PosterLabel->sizeHint().width();
    m_posterPath = Common::SelectPoster(this);
    if(m_posterPath != "")
        Common::DisplayPoster(m_ui->PosterLabel, m_ui->PosterLabel->sizeHint().height(), ratio, m_posterPath);
}

void AddViewDialog::addTag() {
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
    tag->setSavedTag(tag->text());
    tag->setText("❌");
}

void AddViewDialog::mouseLeftTag(Tag* tag) {
    tag->setText(tag->getSavedTag());
}

QList<QWidget*>* AddViewDialog::getCustomColumnsInputList() {
    return m_customColumnsInputList;
}

QList<QString>* AddViewDialog::getCustomColumnsNameList() {
    return m_customColumnsNameList;
}

bool AddViewDialog::eventFilter(QObject *obj, QEvent *event) {
    if (obj == m_ui->TagsInput && event->type() == QEvent::KeyPress) {
        QKeyEvent* key = static_cast<QKeyEvent*>(event);
        if(key->key() == Qt::Key_Return) {
            addTag();
            return true;
        }
    }
    return false;
}
