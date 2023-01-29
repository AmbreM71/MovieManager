#include "MainWindow.h"
#include "ui_MainWindow.h"


MainWindow::MainWindow(QApplication* app) {

    m_savepath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "\\MovieManager\\Posters";

    QDir dir(m_savepath);
    if (!dir.exists())
        dir.mkpath(".");

    m_app = app;
    m_ui = new Ui::MainWindow;
    m_log = new Log();
    m_settings = new QSettings("MovieManager", "MovieManager");
    m_ui->setupUi(this);

    m_app->setWindowIcon(QIcon(":/assets/Assets/logo.png"));

    if(QString::compare(m_app->arguments().at(0), "Neo")) {
        m_ui->EasterEggAct->setVisible(false);
    }

    databaseConnection();

    refreshLanguage();
    refreshTheme();

    m_ui->MoviesListWidget->setHorizontalHeaderLabels(QStringList() << tr("Nom du film") << tr("Année\nde sortie") << tr("Nombre de\nvisionnages") << tr("Premier\nvisionnage") << tr("Dernier\nvisionnage") << tr("Entrées") << tr("Note"));


    fillTable("");
    if(m_ui->MoviesListWidget->rowCount() > 0) {
        m_savedMovieID = m_ui->MoviesListWidget->item(0,2)->text().toInt();
        fillMovieInfos();
    }

    menuBarConnectors();

    m_ui->MoviesListWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    m_ui->MoviesListWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);

    m_ui->MoviesListWidget->setColumnWidth(1,100);
    m_ui->MoviesListWidget->setColumnHidden(2, true);

    QObject::connect(m_ui->AddViewButton, SIGNAL(clicked()), this, SLOT(addView()));
    QObject::connect(m_ui->ManageMovieViewsButton, SIGNAL(clicked()), this, SLOT(editViews()));
    QObject::connect(m_ui->MoviesListWidget, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(editViews()));
    QObject::connect(m_ui->MoviesListWidget, SIGNAL(cellClicked(int,int)), this, SLOT(fillMovieInfos()));
    QObject::connect(m_ui->AdvancedSearchButton, SIGNAL(clicked()), this, SLOT(openFilters()));
    QObject::connect(m_ui->ResetFiltersButton, SIGNAL(clicked()), this, SLOT(resetFilters()));
    QObject::connect(m_ui->MoviesListWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(customMenuRequested(QPoint)));
    QObject::connect(m_ui->MoviesListWidget, SIGNAL(itemSelectionChanged()), this, SLOT(selectedMovieChanged()));
    QObject::connect(m_ui->QuickSearchLineEdit, SIGNAL(textChanged(QString)), this, SLOT(fillTable(QString)));
}

MainWindow::~MainWindow() {
    delete m_ui;
}

void MainWindow::databaseConnection() {
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName("movieDatabase.db");

    if(!m_db.open()) {
        m_log->append(tr("Erreur lors de l'ouverture de la base de données"), eLog::Error);
    }
    else {
        m_log->append(tr("Base de donnée ouverte avec succès"), eLog::Success);
    }

    //Movies table
    QString movieDatabaseCreationString = "CREATE TABLE IF NOT EXISTS movies ("
                                   "ID          INTEGER PRIMARY KEY AUTOINCREMENT,"
                                   "Name        VARCHAR(127),"
                                   "ReleaseYear SMALLINT,"
                                   "Entries     INT,"
                                   "Rating      TINYINT(10),"
                                   "Poster      VARCHAR(127));";

    QSqlQuery movieDBQuery;

    if(!movieDBQuery.exec(movieDatabaseCreationString)) {
        m_log->append(tr("Erreur lors de la création de la table movies"), eLog::Error);
    }

    //Views table
    QString ViewsDatabaseCreationString = "CREATE TABLE IF NOT EXISTS views ("
                                   "ID          INTEGER PRIMARY KEY AUTOINCREMENT,"
                                   "ID_Movie    INTEGER,"
                                   "ViewDate    DATE,"
                                   "ViewType    INTEGER);";

    QSqlQuery viewsBDQuery;

    if(!viewsBDQuery.exec(ViewsDatabaseCreationString)) {
        m_log->append(tr("Erreur lors de la création de la table views"), eLog::Error);
    }

    //TagsInfo Table
    QString TagsInfoDatabaseCreationString = "CREATE TABLE IF NOT EXISTS tagsInfo ("
                                   "Tag         VARCHAR(63) PRIMARY KEY ,"
                                   "Color       VARCHAR(6));";

    QSqlQuery tagsInfoBDQuery;

    if(!tagsInfoBDQuery.exec(TagsInfoDatabaseCreationString)) {
        m_log->append(tr("Erreur lors de la création de la table tagsInfo"), eLog::Error);
    }

    //Tags Table
    QString TagsDatabaseCreationString = "CREATE TABLE IF NOT EXISTS tags ("
                                   "ID_Movie    INTEGER,"
                                   "Tag         VARCHAR(63));";

    QSqlQuery TagsBDQuery;

    if(!TagsBDQuery.exec(TagsDatabaseCreationString)) {
        m_log->append(tr("Erreur lors de la création de la table tags"), eLog::Error);
    }
}

void MainWindow::fillTable(const QString &text) {

    m_ui->MoviesListWidget->blockSignals(true);
    m_ui->MoviesListWidget->setSortingEnabled(false);

    //Clear the table
    int movieListRowCount = m_ui->MoviesListWidget->rowCount();
    for(int i=movieListRowCount ; i >= 0 ; i--) {
        m_ui->MoviesListWidget->removeRow(i);
    }

    //Fetch every unique movies
    QSqlQuery moviesQuery;

    if(m_isFiltered) {
        m_log->append(tr("Recupération filtrée depuis la base de donnée"), eLog::Notice);
        moviesQuery.exec("SELECT ID, Name, ReleaseYear FROM movies "
                         "WHERE Name LIKE \"%" + m_filter_name + "%\""
                         "AND ReleaseYear BETWEEN '"+QString::number(m_filter_minYear)+"' AND '"+QString::number(m_filter_maxYear)+"'"
                         "AND Rating BETWEEN '"+QString::number(m_filter_minRating)+"' AND '"+QString::number(m_filter_maxRating)+"'"
                         "AND Entries >= "+QString::number(m_filter_minEntries)+" ORDER BY ID"
        );
        m_ui->ResetFiltersButton->setEnabled(true);
    }
    else {
        moviesQuery.exec("SELECT ID, Name, ReleaseYear FROM movies ORDER BY ID;");
        m_ui->ResetFiltersButton->setEnabled(false);
        m_filter_name = "";
        m_filter_minYear = 0;
        m_filter_maxYear = 0;
        m_filter_minRating = 0;
        m_filter_maxRating = 0;
        m_filter_minEntries = 0;
    }

    int numberOfParsedMovies = 0;
    while(moviesQuery.next()) {
        QTableWidgetItem* name = new QTableWidgetItem();
        QTableWidgetItem* releaseYear = new QTableWidgetItem();
        QTableWidgetItem* ID = new QTableWidgetItem();

        releaseYear->setTextAlignment(Qt::AlignCenter);

        ID->setText(moviesQuery.value(0).toString());
        name->setText(moviesQuery.value(1).toString());
        releaseYear->setText(moviesQuery.value(2).toString());

        //Creates a new row on the table
        m_ui->MoviesListWidget->insertRow(m_ui->MoviesListWidget->rowCount());

        //Insert QTableWidgetItem in the table
        m_ui->MoviesListWidget->setItem(m_ui->MoviesListWidget->rowCount()-1, 0, name);
        m_ui->MoviesListWidget->setItem(m_ui->MoviesListWidget->rowCount()-1, 1, releaseYear);
        m_ui->MoviesListWidget->setItem(m_ui->MoviesListWidget->rowCount()-1, 2, ID);

        numberOfParsedMovies++;
        if(m_settings->value("matrixMode").toBool() == true && (name->text() == "Matrix" || name->text() == "The Matrix")) {
            name->setForeground(QBrush(QColor(0,150,0)));
        }
    }

    m_log->append(tr("Nombre de films lus depuis la base de donnée : ")+QString::number(numberOfParsedMovies), eLog::Notice);

    /* QUICK FILTER PART*/

    QSqlQuery tagQuery;
    tagQuery.exec("SELECT * FROM tags;");

    QString* filter = new QString(text);
    QChar cellChar, filterChar;
    int cellsNotCorrespondingToFilter;
    for(int row = 0 ; row < m_ui->MoviesListWidget->rowCount() ; row++) {
        cellsNotCorrespondingToFilter = 0;
        for(int column = 0 ; column < m_ui->MoviesListWidget->columnCount()-1 ; column++) {
            QString cellText = m_ui->MoviesListWidget->item(row, column)->text();
            if(filter->length() <= cellText.length()) {
                for(int filterIndex = 0 ; filterIndex < filter->length() ; filterIndex++) {
                        cellChar = cellText.at(filterIndex);
                        filterChar = filter->at(filterIndex);
                        if(!m_settings->value("quickSearchCaseSensitive").toBool()) {
                            cellChar = cellChar.toLower();
                            filterChar = filterChar.toLower();
                        }
                        if(cellChar != filterChar) {
                            cellsNotCorrespondingToFilter++;
                            break;
                        }
                    }
                }
            else {
                cellsNotCorrespondingToFilter++;
            }
        }
        //If no cell in the line corresponds to the search
        if(cellsNotCorrespondingToFilter == m_ui->MoviesListWidget->columnCount()-1) {
            m_ui->MoviesListWidget->hideRow(row);
        }
        //If cell correspond to the quick search, check if tags filter correspond
        else {
            int ID = m_ui->MoviesListWidget->item(row, 2)->text().toInt();
            bool hasMovieAllFilterTags = true;
            for(int filterTag = 0 ; filterTag < m_ui->SelectedTagsLayout->count()-1 ; filterTag++) {
                Tag* tag = (Tag*)m_ui->SelectedTagsLayout->itemAt(filterTag)->widget();
                bool hasMovieFilterTag = false;
                tagQuery.first();
                tagQuery.previous();
                while(tagQuery.next()) {
                    if(tagQuery.value(0).toInt() == ID && QString::compare(tagQuery.value(1).toString(), tag->text()) == 0) {
                        hasMovieFilterTag = true;
                        break;
                    }
                }
                if(hasMovieFilterTag == false) {
                    hasMovieAllFilterTags = false;
                    break;
                }
            }
            if(hasMovieAllFilterTags == false) {
                m_ui->MoviesListWidget->hideRow(row);
            }
        }
    }
    delete filter;

    m_ui->MoviesListWidget->blockSignals(false);
    m_ui->MoviesListWidget->setSortingEnabled(true);
    m_ui->MoviesListWidget->sortItems(0);
    m_ui->MoviesListWidget->setCurrentCell(getIndexOfMovie(m_savedMovieID), 0);
    m_ui->DisplayedMovieCountLabel->setText(tr("Films : ") + QString::number(m_ui->MoviesListWidget->rowCount()));
    fillMovieInfos();
}

void MainWindow::fillMovieInfos() {

    if(m_ui->MoviesListWidget->rowCount() == 0) {
        return;
    }
    if(m_ui->MoviesListWidget->currentRow() == -1) {
        m_ui->ViewsLabel->setText(tr("Sélectionnez un film pour voir les information sur ce dernier"));
        m_ui->FirstViewLabel->setText("");
        m_ui->LastViewLabel->setText("");
        m_ui->EntriesLabel->setText("");
        m_ui->RatingLabel->setText("");
        for(int i = 0 ; i < m_ui->TagsLayout->count() ; i++) {
            delete m_ui->TagsLayout->itemAt(i);
        }
        return;
    }
    QString ID;
    if(m_savedMovieID == -1) {
        ID = m_ui->MoviesListWidget->item(m_ui->MoviesListWidget->currentRow(), 2)->text();
    }
    else {
        ID = QString::number(m_savedMovieID);
    }
    m_ui->MovieTitleLabel->setText(m_ui->MoviesListWidget->item(m_ui->MoviesListWidget->currentRow(),0)->text());

    //Fetch the number of views of the current movie
    QSqlQuery posterQuery;
    posterQuery.exec("SELECT Poster FROM movies WHERE ID='"+ID+"'");
    posterQuery.first();

    Common::loadPoster(this, m_ui->PosterLabel, 400, 1, m_savepath+"\\"+posterQuery.value(0).toString());

    //Fetch the number of views of the current movie
    QSqlQuery viewsQuery;
    viewsQuery.exec("SELECT COUNT(*) FROM views WHERE ID_Movie='"+ID+"'");
    viewsQuery.first();
    m_ui->ViewsLabel->setText(tr("vu ")+viewsQuery.value(0).toString()+tr(" fois"));


    //Fetch the first view of the current movie
    QSqlQuery firstViewQuery;
    firstViewQuery.exec("SELECT ViewDate FROM views WHERE ID_Movie='"+ID+"' AND NOT ViewDate='?' ORDER BY ViewDate ASC LIMIT 1");
    firstViewQuery.first();
    if(firstViewQuery.value(0).toString()=="") {
        m_ui->FirstViewLabel->setText(tr("Premier visionnage : ?"));
    }
    else {
        m_ui->FirstViewLabel->setText(tr("Premier visionnage : ")+firstViewQuery.value(0).toDate().toString(m_settings->value("dateFormat").toString()));
    }

    //Fetch the last view of the current movie
    QSqlQuery lastViewQuery;
    lastViewQuery.exec("SELECT ViewDate FROM views WHERE ID_Movie='"+ID+"' AND NOT ViewDate='?' ORDER BY ViewDate DESC LIMIT 1");
    lastViewQuery.first();
    if(lastViewQuery.value(0).toString()=="") {
        m_ui->LastViewLabel->setText(tr("Dernier visionnage : ?"));
    }
    else {
        m_ui->LastViewLabel->setText(tr("Dernier visionnage : ")+lastViewQuery.value(0).toDate().toString(m_settings->value("dateFormat").toString()));
    }

    QSqlQuery hasUnknownView;
    hasUnknownView.exec("SELECT ViewDate FROM views WHERE ID_Movie='"+ID+"' AND ViewDate='?'");
    hasUnknownView.first();
    if(!hasUnknownView.isNull(0)) {
        m_ui->FirstViewLabel->setStyleSheet("color:red;");
    }
    else {
        m_ui->FirstViewLabel->setStyleSheet("");
    }

    QSqlQuery q;
    q.exec("SELECT Entries, Rating FROM movies WHERE ID='"+ID+"'");
    q.first();
    if(q.value(0).toInt() == -1) {
        m_ui->EntriesLabel->setText(tr("Nombre d'entrées non renseigné"));
    }
    else {
        m_ui->EntriesLabel->setText(m_locale->toString(q.value(0).toInt()) + tr(" entrées"));
    }
    Common::ratingToStar(q.value(1).toInt(), m_ui->RatingLabel);

    QSqlQuery tagsQuery;
    tagsQuery.exec("SELECT Tag FROM tags WHERE ID_Movie='"+ID+"'");

    // Clear tags from layout
    for(int i = m_ui->TagsLayout->count()-1 ; i >= 0 ; i--) {
        QLabel* tag = (QLabel*)m_ui->TagsLayout->itemAt(i)->widget();
        delete tag;
    }

    while(tagsQuery.next()) {
        Tag* tag = new Tag(tagsQuery.value(0).toString());

        QObject::connect(tag, SIGNAL(clicked(Tag*)), this, SLOT(clickedTag(Tag*)));
        QObject::connect(tag, SIGNAL(mouseEnter(Tag*)), this, SLOT(mouseEnteredTag(Tag*)));
        QObject::connect(tag, SIGNAL(mouseLeave(Tag*)), this, SLOT(mouseLeftTag(Tag*)));
        m_ui->TagsLayout->insertWidget(m_ui->TagsLayout->count()-1,tag,0,Qt::AlignLeft);
    }
}

void MainWindow::removeUnusedTags() {
    QString removedTags;

    QSqlQuery tagsQuery;
    tagsQuery.exec("SELECT Tag FROM tags");

    QSqlQuery tagsInfoQuery;
    tagsInfoQuery.exec("SELECT Tag FROM tagsInfo");

    bool found;

    //Foreach tag
    while(tagsInfoQuery.next()) {
        QSqlQuery deleteTagQuery;
        found = false;
        tagsQuery.first();
        tagsQuery.previous();
        //Foreach links between Tag and movie
        while(tagsQuery.next()) {
            //If tag exist in link table
            if(QString::compare(tagsQuery.value(0).toString(),tagsInfoQuery.value(0).toString()) == 0) {
                found = true;
                break;
            }
        }
        if(found == false) {
            if(!deleteTagQuery.exec("DELETE FROM tagsInfo WHERE Tag=\""+tagsInfoQuery.value(0).toString()+"\";")) {
                m_log->append(tr("Erreur lors de la suppression dans la table tagsInfo, plus d'informations ci-dessous :\nCode d'erreur ")+deleteTagQuery.lastError().nativeErrorCode()+tr(" : ")+deleteTagQuery.lastError().text(), eLog::Error);
            }
            removedTags.append(tagsInfoQuery.value(0).toString() + ", ");
        }
    }
    removedTags.remove(removedTags.length()-2, removedTags.length());
    if(removedTags.length() > 0) {
        m_log->append(tr("Les tags suivants ne sont plus utilisés, ils sont supprimés : ") + removedTags, eLog::Notice);
    }

}

void MainWindow::importDB() {
    QString file = QFileDialog::getOpenFileName(this, tr("Exporter"), QString(), "JSON (*.json)");
    QFile jsonFile(file);
    //Test if the file is correctly opened
    if (!jsonFile.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, tr("Erreur"), jsonFile.errorString());
    }
    else {
        int answer = QMessageBox::question(this, tr("Importer"), tr("Cette opération va supprimer tous vos visionnages actuels, voulez-vous continuer ?"));
        if (answer == QMessageBox::Yes) {

            QSqlQuery deleteQuery;
            deleteQuery.exec("DELETE FROM movies");
            deleteQuery.exec("DELETE FROM views");
            deleteQuery.exec("DELETE FROM tags");
            deleteQuery.exec("DELETE FROM tagsInfo");

            QString val = jsonFile.readAll();
            jsonFile.close();
            QJsonObject main = QJsonDocument::fromJson(val.toUtf8()).object();
            foreach(const QString& mainKey, main.keys()) {
                if(mainKey == "movies") {
                    QJsonObject movies = main.value(mainKey).toObject();
                    foreach(const QString& movieKey, movies.keys()) {
                        QJsonObject movie = movies.value(movieKey).toObject();
                        QSqlQuery query;
                        query.prepare("INSERT INTO movies (ID, Name, ReleaseYear, Entries, Rating, Poster) VALUES (?,?,?,?,?,?);");
                        query.bindValue(0, movie["ID"].toInt());
                        query.bindValue(1, movie["Name"].toString());
                        query.bindValue(2, movie["ReleaseYear"].toInt());
                        query.bindValue(3, movie["Entries"].toInt());
                        query.bindValue(4, movie["Rating"].toInt());
                        query.bindValue(5, movie["Poster"].toString());

                        if(!query.exec()){
                            m_log->append(tr("Erreur lors de l'import dans la table movies, plus d'informations ci-dessous :\nCode d'erreur ")+query.lastError().nativeErrorCode()+tr(" : ")+query.lastError().text(), eLog::Error);
                        }
                    }
                }
                else if(mainKey == "views") {
                    QJsonObject views = main.value(mainKey).toObject();
                    foreach(const QString& viewKey, views.keys()) {
                        QJsonObject view = views.value(viewKey).toObject();
                        QSqlQuery query;
                        query.prepare("INSERT INTO views (ID, ID_Movie, ViewDate, ViewType) VALUES (?,?,?,?);");
                        query.bindValue(0, view["ID"].toInt());
                        query.bindValue(1, view["ID_Movie"].toInt());
                        query.bindValue(2, view["ViewDate"].toString());
                        query.bindValue(3, view["ViewType"].toString());

                        if(!query.exec()){
                            m_log->append(tr("Erreur lors de l'import dans la table views, plus d'informations ci-dessous :\nCode d'erreur ")+query.lastError().nativeErrorCode()+tr(" : ")+query.lastError().text(), eLog::Error);
                        }
                    }
                }
                else if(mainKey == "tags") {
                    QJsonObject tags = main.value(mainKey).toObject();
                    foreach(const QString& tagKey, tags.keys()) {
                        QJsonObject tag = tags.value(tagKey).toObject();
                        QSqlQuery query;
                        query.prepare("INSERT INTO tags (ID_Movie, Tag) VALUES (?,?);");
                        query.bindValue(0, tag["ID_Movie"].toInt());
                        query.bindValue(1, tag["Tag"].toString());

                        if(!query.exec()){
                            m_log->append(tr("Erreur lors de l'import dans la table tags, plus d'informations ci-dessous :\nCode d'erreur ")+query.lastError().nativeErrorCode()+tr(" : ")+query.lastError().text(), eLog::Error);
                        }
                    }
                }
                else if(mainKey == "tagsInfo") {
                    QJsonObject tagsInfo = main.value(mainKey).toObject();
                    foreach(const QString& tagInfoKey, tagsInfo.keys()) {
                        QJsonObject tagInfo = tagsInfo.value(tagInfoKey).toObject();
                        QSqlQuery query;
                        query.prepare("INSERT INTO tagsInfo (Tag, Color) VALUES (?,?);");
                        query.bindValue(0, tagInfo["Tag"].toString());
                        query.bindValue(1, tagInfo["Color"].toString());

                        if(!query.exec()){
                            m_log->append(tr("Erreur lors de l'import dans la table tags, plus d'informations ci-dessous :\nCode d'erreur ")+query.lastError().nativeErrorCode()+tr(" : ")+query.lastError().text(), eLog::Error);
                        }
                    }
                }
            }
        }
    }
    fillTable(m_ui->QuickSearchLineEdit->text());
    fillGlobalStats();
}

void MainWindow::exportDB() {
    int i;
    QString file = QFileDialog::getSaveFileName(this, tr("Exporter"), QString(), "JSON (*.json)");  //Get the save link
    //Creates a QFile with the fetched path
    QFile jsonFile(file);
    //Test if the file is correctly opened
    if (!jsonFile.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(this, tr("Erreur"), jsonFile.errorString());
    }

    QJsonObject mainObject;

    //Writes movies to JSON
    QJsonObject moviesObject;
    QSqlQuery moviesQuery;
    moviesQuery.exec("SELECT ID, Name, ReleaseYear, Entries, Rating, Poster FROM movies;");
    i=0;
    while(moviesQuery.next()) {
        i++;

        QJsonObject movieObject;

        movieObject.insert("ID", QJsonValue::fromVariant(moviesQuery.value(0).toInt()));
        movieObject.insert("Name", QJsonValue::fromVariant(moviesQuery.value(1).toString()));
        movieObject.insert("ReleaseYear", QJsonValue::fromVariant(moviesQuery.value(2).toInt()));
        movieObject.insert("Entries", QJsonValue::fromVariant(moviesQuery.value(3).toInt()));
        movieObject.insert("Rating", QJsonValue::fromVariant(moviesQuery.value(4).toInt()));
        movieObject.insert("Poster", QJsonValue::fromVariant(moviesQuery.value(5).toString()));

        moviesObject.insert("movie" + QString::fromStdString(std::to_string(i)), movieObject);
    }
    mainObject.insert("movies", moviesObject);

    //Writes views to JSON
    QJsonObject viewsObject;
    QSqlQuery viewsQuery;
    viewsQuery.exec("SELECT ID, ID_Movie, ViewDate, ViewType FROM views;");
    i=0;
    while(viewsQuery.next()) {
        i++;

        QJsonObject viewObject;

        viewObject.insert("ID", QJsonValue::fromVariant(viewsQuery.value(0).toInt()));
        viewObject.insert("ID_Movie", QJsonValue::fromVariant(viewsQuery.value(1).toInt()));
        viewObject.insert("ViewDate", QJsonValue::fromVariant(viewsQuery.value(2).toString()));
        viewObject.insert("ViewType", QJsonValue::fromVariant(viewsQuery.value(3).toString()));

        viewsObject.insert("view" + QString::fromStdString(std::to_string(i)), viewObject);
    }
    mainObject.insert("views", viewsObject);

    //Writes tagsInfo to JSON
    QJsonObject tagsInfoObject;
    QSqlQuery tagsInfoQuery;
    tagsInfoQuery.exec("SELECT Tag, Color FROM tagsInfo;");
    i=0;
    while(tagsInfoQuery.next()) {
        i++;

        QJsonObject tagInfoObject;

        tagInfoObject.insert("Tag", QJsonValue::fromVariant(tagsInfoQuery.value(0).toString()));
        tagInfoObject.insert("Color", QJsonValue::fromVariant(tagsInfoQuery.value(1).toString()));

        tagsInfoObject.insert("tagInfo" + QString::fromStdString(std::to_string(i)), tagInfoObject);
    }
    mainObject.insert("tagsInfo", tagsInfoObject);

    //Writes tags to JSON
    QJsonObject tagsObject;
    QSqlQuery tagsQuery;
    tagsQuery.exec("SELECT ID_Movie, Tag FROM tags;");
    i=0;
    while(tagsQuery.next()) {
        i++;

        QJsonObject tagObject;

        tagObject.insert("ID_Movie", QJsonValue::fromVariant(tagsQuery.value(0).toInt()));
        tagObject.insert("Tag", QJsonValue::fromVariant(tagsQuery.value(1).toString()));

        tagsObject.insert("tag" + QString::fromStdString(std::to_string(i)), tagObject);
    }
    mainObject.insert("tags", tagsObject);

    jsonFile.write(QJsonDocument(mainObject).toJson(QJsonDocument::Indented));
    jsonFile.close();
}

void MainWindow::addView() {
    AddViewDialog* window = new AddViewDialog(this, m_settings);
    window->show();
    if(window->exec() == 1) {

        QString movieName;
        QString movieYear;

        //Add the new movie to the movies table
        if(window->getComboboxSelectedItem() == "") {

            movieName = window->getName();
            movieYear = QString::number(window->getReleaseYear());

            bool movieAlreadyExist = false;
            QSqlQuery existingMoviesQuery;
            existingMoviesQuery.exec("SELECT Name, ReleaseYear FROM movies");
            while(existingMoviesQuery.next()) {
                if(QString::compare(movieName, existingMoviesQuery.value(0).toString()) == 0 && QString::compare(movieYear, existingMoviesQuery.value(1).toString()) == 0) {
                    movieAlreadyExist = true;
                    QMessageBox::information(this, tr("Film déjà présent"), tr("Un film correspond déjà à ce nom et cette date de sortie, la vue sera ajouté à ce dernier"));
                    break;
                }
            }
            if(!movieAlreadyExist) {
                QString posterPath = "";
                if(window->getPosterPath() != "") {
                    QImage poster(window->getPosterPath());
                    if(poster.height() > 1200 || poster.width() > 1200) {
                        m_log->append(tr("Image trop grande (") + QString::number(poster.width()) + "x" + QString::number(poster.height()) +
                                      "). Des latences peuvent apparaître.", eLog::Warning);
                    }

                    //Processing poster moving and renaming
                    QString ext = window->getPosterPath().remove(0, window->getPosterPath().lastIndexOf(".")+1);
                    QString GUID = QString::number(QRandomGenerator::global()->generate());
                    if(QFile::copy(window->getPosterPath(), m_savepath+"/"+GUID+"."+ext) == false) {
                        m_log->append(tr("Erreur lors de la copie de l'image,\nChemin d'origine : ")+window->getPosterPath()+tr("\nChemin de destination : ")+m_savepath+"/"+GUID+"."+ext, eLog::Error);
                    }
                    posterPath = GUID+"."+ext;
                }

                QSqlQuery insertIntoMoviesQuery;
                insertIntoMoviesQuery.prepare("INSERT INTO movies (Name, ReleaseYear, Entries, Rating, Poster) VALUES (?,?,?,?,?);");



                insertIntoMoviesQuery.bindValue(0, window->getName());
                insertIntoMoviesQuery.bindValue(1, window->getReleaseYear());
                if(window->isEntriesUnknown()) {
                    insertIntoMoviesQuery.bindValue(2, -1);
                }
                else {
                    insertIntoMoviesQuery.bindValue(2, window->getEntries());
                }
                insertIntoMoviesQuery.bindValue(3, window->getRating());
                insertIntoMoviesQuery.bindValue(4, posterPath);

                if(!insertIntoMoviesQuery.exec()){
                    m_log->append(tr("Erreur lors de l'ajout dans la table movies, plus d'informations ci-dessous :\nCode d'erreur ")+insertIntoMoviesQuery.lastError().nativeErrorCode()+tr(" : ")+insertIntoMoviesQuery.lastError().text(), eLog::Error);
                    return;
                }
            }
        }
        else {
            movieName = window->getComboboxSelectedItem().remove(window->getComboboxSelectedItem().length()-7, window->getComboboxSelectedItem().length());
            movieYear = window->getComboboxSelectedItem().remove(0, window->getComboboxSelectedItem().length()-4);
        }

        //Add the view to the views table
        QSqlQuery insertIntoViewsQuery;
        insertIntoViewsQuery.prepare("INSERT INTO views (ID_Movie, ViewDate, ViewType) VALUES (?,?,?);");


        QSqlQuery viewedMovieIDQuery;
        viewedMovieIDQuery.exec("SELECT ID FROM movies WHERE Name=\""+movieName+"\" AND ReleaseYear=\""+movieYear+"\"");
        viewedMovieIDQuery.first();

        m_savedMovieID = viewedMovieIDQuery.value(0).toInt();

        QString ViewDate;
        int ViewType;

        if(window->isDateUnknown()) {
            ViewDate = "?";
        }
        else {
            ViewDate = window->getViewDate().toString("yyyy-MM-dd");
        }

        if(window->isTypeUnknown()) {
            ViewType = eViewType::Unknown;
        }
        else {
            ViewType = window->getViewType();
        }

        insertIntoViewsQuery.bindValue(0, m_savedMovieID);
        insertIntoViewsQuery.bindValue(1, ViewDate);
        insertIntoViewsQuery.bindValue(2, ViewType);

        if(!insertIntoViewsQuery.exec()){
            m_log->append(tr("Erreur lors de l'ajout dans la table views, plus d'informations ci-dessous :\nCode d'erreur ")+insertIntoViewsQuery.lastError().nativeErrorCode()+tr(" : ")+insertIntoViewsQuery.lastError().text(), eLog::Error);
        }

        for(int i=0 ; i<window->getTags()->size() ; i++) {
            QString hexcolor = "";
            for(int j = 0 ; j<6 ; j++) {
                hexcolor.append(QString::number(rand() % 9));
            }

            QSqlQuery insertIntoTagsInfoQuery;

            insertIntoTagsInfoQuery.prepare("INSERT INTO tagsInfo (Tag, Color) VALUES (?,?);");
            insertIntoTagsInfoQuery.bindValue(0, window->getTags()->at(i));
            insertIntoTagsInfoQuery.bindValue(1, hexcolor);

            if(!insertIntoTagsInfoQuery.exec()){
                m_log->append(tr("Erreur lors de l'ajout dans la table tagsInfo, plus d'informations ci-dessous :\nCode d'erreur ")+insertIntoTagsInfoQuery.lastError().nativeErrorCode()+tr(" : ")+insertIntoTagsInfoQuery.lastError().text(), eLog::Error);
            }

            QSqlQuery insertIntoTagsQuery;

            insertIntoTagsQuery.prepare("INSERT INTO tags (ID_Movie, Tag) VALUES (?,?);");
            insertIntoTagsQuery.bindValue(0, m_savedMovieID);
            insertIntoTagsQuery.bindValue(1, window->getTags()->at(i));

            if(!insertIntoTagsQuery.exec()){
                m_log->append(tr("Erreur lors de l'ajout dans la table tags, plus d'informations ci-dessous :\nCode d'erreur ")+insertIntoTagsQuery.lastError().nativeErrorCode()+tr(" : ")+insertIntoTagsQuery.lastError().text(), eLog::Error);
            }

        }
        fillGlobalStats();
        fillTable(m_ui->QuickSearchLineEdit->text());
        m_ui->MoviesListWidget->setCurrentCell(getIndexOfMovie(m_savedMovieID), 0);
        fillMovieInfos();
    }
}

void MainWindow::editViews() {
    int ID = m_savedMovieID;
    EditViewsDialog* window = new EditViewsDialog(&ID, m_log, m_settings, this);
    window->show();
    if(window->exec() == 1) {
        if (window->edited()) {
            fillTable(m_ui->QuickSearchLineEdit->text());
            fillGlobalStats();
        }
    }
}

void MainWindow::editMovie() {
    EditMovieDialog* window = new EditMovieDialog(QString::number(m_savedMovieID), this);
    window->show();
    if(window->exec() == 1) {

        QString ID = QString::number(m_savedMovieID);
        QString GUID = "";
        QString ext = "";

        QSqlQuery existingMoviesQuery;
        existingMoviesQuery.exec("SELECT Name, ReleaseYear, ID, Poster FROM movies");
        while(existingMoviesQuery.next()) {
            if(QString::compare(window->getMovieName(), existingMoviesQuery.value(0).toString()) == 0 && QString::compare(window->getReleaseYear(), existingMoviesQuery.value(1).toString()) == 0 && m_savedMovieID != existingMoviesQuery.value(2).toInt()) {
                int answer = QMessageBox::question(this, tr("Film déjà présent"), tr("Un film correspond déjà à ce nom et cette date de sortie, les vues de ce film seront fusionnées, voulez-vous continuer ?"));
                if(answer == QMessageBox::Yes) {
                    QSqlQuery changeViewsIDQuery;
                    changeViewsIDQuery.exec("UPDATE views SET ID_Movie=\""+existingMoviesQuery.value(2).toString()+"\" WHERE ID_Movie=\""+ID+"\"");

                    QSqlQuery MoviePosterToDeleteQuery;
                    MoviePosterToDeleteQuery.exec("SELECT Poster FROM Movies WHERE ID=\""+ID+"\"");
                    MoviePosterToDeleteQuery.first();
                    QFile::remove(m_savepath+"\\"+MoviePosterToDeleteQuery.value(0).toString());

                    QSqlQuery deleteMovieQuery;
                    deleteMovieQuery.exec("DELETE FROM movies WHERE ID=\""+ID+"\"");
                    QSqlQuery deleteMovieTags;
                    deleteMovieTags.exec("DELETE FROM tags WHERE ID_Movie=\""+ID+"\"");

                    removeUnusedTags();
                    resetFilters();
                    m_ui->MoviesListWidget->setCurrentCell(getIndexOfMovie(existingMoviesQuery.value(2).toInt()), 0);
                    fillMovieInfos();
                    fillGlobalStats();
                }
                return;
            }
        }

        if(window->newPoster()) {

            //Delete old poster
            QSqlQuery posterQuery;
            posterQuery.exec("SELECT Poster FROM movies WHERE ID=\""+ID+"\";");
            posterQuery.first();
            QFile::remove(m_savepath+"\\"+posterQuery.value(0).toString());

            ext = window->getPosterPath().remove(0, window->getPosterPath().lastIndexOf(".")+1);

            GUID = QString::number(QRandomGenerator::global()->generate());
            if(QFile::copy(window->getPosterPath(), m_savepath+"/"+GUID+"."+ext) == false) {
                m_log->append(tr("Erreur lors de la copie de l'image,\nChemin d'origine : ")+window->getPosterPath()+tr("\nChemin de destination : ")+m_savepath+"/"+GUID+"."+ext, eLog::Error);
            }
            QSqlQuery editMovieQuery;
            if(!editMovieQuery.exec("UPDATE movies SET Name=\""+window->getMovieName()+"\", ReleaseYear=\""+window->getReleaseYear()+
                                    "\", Entries=\""+QString::number(window->getEntries())+"\", Rating=\""+QString::number(window->getRating())+
                                    "\", Poster=\""+GUID+"."+ext+"\" WHERE ID=\""+ID+"\";")) {
                m_log->append(tr("Erreur lors de l'édition dans la table movies, plus d'informations ci-dessous :\nCode d'erreur ")+editMovieQuery.lastError().nativeErrorCode()+tr(" : ")+editMovieQuery.lastError().text(), eLog::Error);
            }
        }
        else {
            QSqlQuery editMovieQuery;
            int entries = 0;
            if(window->isEntriesUnknown()) {
                entries = -1;
            }
            else {
                entries = window->getEntries();
            }

            if(!editMovieQuery.exec("UPDATE movies SET Name=\""+window->getMovieName()+"\", ReleaseYear=\""+window->getReleaseYear()+
                                    "\", Entries=\""+QString::number(entries)+"\", Rating=\""+QString::number(window->getRating())+
                                    "\" WHERE ID=\""+ID+"\";")) {
                m_log->append(tr("Erreur lors de l'édition dans la table movies, plus d'informations ci-dessous :\nCode d'erreur ")+editMovieQuery.lastError().nativeErrorCode()+tr(" : ")+editMovieQuery.lastError().text(), eLog::Error);
            }
        }

        QSqlQuery removeMovieTagsQuery;
        if(!removeMovieTagsQuery.exec("DELETE FROM tags WHERE ID_Movie="+ID)){
            m_log->append(tr("Erreur lors de la suppression des tags du film, plus d'informations ci-dessous :\nCode d'erreur ")+removeMovieTagsQuery.lastError().nativeErrorCode()+tr(" : ")+removeMovieTagsQuery.lastError().text(), eLog::Error);
        }



        QSqlQuery TagsInfoQuery;
        TagsInfoQuery.exec("SELECT Tag FROM TagsInfo");
        bool tagAlreadyExist;

        for(int i=0 ; i<window->getTags()->size() ; i++) {
            tagAlreadyExist = false;

            QString hexcolor = "";
            for(int j = 0 ; j<6 ; j++) {
                hexcolor.append(QString::number(rand() % 9));
            }

            QSqlQuery insertIntoTagsInfoQuery;

            TagsInfoQuery.first();
            TagsInfoQuery.previous();
            while(TagsInfoQuery.next()) {
                if(QString::compare(window->getTags()->at(i), TagsInfoQuery.value(0).toString()) == 0) {
                    tagAlreadyExist = true;
                    break;
                }
            }
            if(!tagAlreadyExist) {
                insertIntoTagsInfoQuery.prepare("INSERT INTO tagsInfo (Tag, Color) VALUES (?,?);");
                insertIntoTagsInfoQuery.bindValue(0, window->getTags()->at(i));
                insertIntoTagsInfoQuery.bindValue(1, hexcolor);

                if(!insertIntoTagsInfoQuery.exec()){
                    m_log->append(tr("Erreur lors de l'ajout dans la table tagsInfo, plus d'informations ci-dessous :\nCode d'erreur ")+insertIntoTagsInfoQuery.lastError().nativeErrorCode()+tr(" : ")+insertIntoTagsInfoQuery.lastError().text(), eLog::Error);
                }
            }
            QSqlQuery insertIntoTagsQuery;

            insertIntoTagsQuery.prepare("INSERT INTO tags (ID_Movie, Tag) VALUES (?,?);");
            insertIntoTagsQuery.bindValue(0, ID);
            insertIntoTagsQuery.bindValue(1, window->getTags()->at(i));

            if(!insertIntoTagsQuery.exec()){
                m_log->append(tr("Erreur lors de l'ajout dans la table tagslinks, plus d'informations ci-dessous :\nCode d'erreur ")+insertIntoTagsQuery.lastError().nativeErrorCode()+tr(" : ")+insertIntoTagsQuery.lastError().text(), eLog::Error);
            }

        }

        removeUnusedTags();
        fillTable(m_ui->QuickSearchLineEdit->text());
        fillMovieInfos();

    }
}

void MainWindow::deleteMovie() {
    QMessageBox::StandardButton reply;
    int savedRow = 0;
    reply = QMessageBox::question(this, tr("Supprimer le film"), tr("Êtes-vous sûr de vouloir supprimer le film ? Les visionnages associés seront effacés."));
    if(reply == QMessageBox::Yes) {

        QSqlQuery deleteMovieQuery;
        QSqlQuery deleteAssociatedViewsQuery;
        QSqlQuery deleteAssociatedTagsQuery;
        QSqlQuery posterQuery;

        QString ID = QString::number(m_savedMovieID);
        savedRow = m_ui->MoviesListWidget->currentRow();

        posterQuery.exec("SELECT Poster FROM movies WHERE ID=\""+ID+"\";");
        posterQuery.first();

        QFile::remove(m_savepath+"\\"+posterQuery.value(0).toString());

        if(!deleteMovieQuery.exec("DELETE FROM movies WHERE ID=\""+ID+"\";")) {
            m_log->append(tr("Erreur lors de la suppression dans la table movies, plus d'informations ci-dessous :\nCode d'erreur ")+deleteMovieQuery.lastError().nativeErrorCode()+tr(" : ")+deleteMovieQuery.lastError().text(), eLog::Error);
        }

        if(!deleteAssociatedViewsQuery.exec("DELETE FROM views WHERE ID_Movie=\""+ID+"\";")) {
            m_log->append(tr("Erreur lors de la suppression dans la table views, plus d'informations ci-dessous :\nCode d'erreur ")+deleteAssociatedViewsQuery.lastError().nativeErrorCode()+tr(" : ")+deleteAssociatedViewsQuery.lastError().text(), eLog::Error);
        }

        if(!deleteAssociatedTagsQuery.exec("DELETE FROM tags WHERE ID_Movie=\""+ID+"\";")) {
            m_log->append(tr("Erreur lors de la suppression dans la table tags, plus d'informations ci-dessous :\nCode d'erreur ")+deleteAssociatedTagsQuery.lastError().nativeErrorCode()+tr(" : ")+deleteAssociatedTagsQuery.lastError().text(), eLog::Error);
        }

        removeUnusedTags();
        resetFilters();
        if(savedRow+1 >= m_ui->MoviesListWidget->rowCount())
            savedRow--;
        m_ui->MoviesListWidget->setCurrentCell(savedRow, 0);
        fillMovieInfos();
        fillGlobalStats();
    }
}

void MainWindow::openFilters() {
    FiltersDialog* window = new FiltersDialog(&m_filter_name, &m_filter_minYear, &m_filter_maxYear, &m_filter_minRating, &m_filter_maxRating, &m_filter_minEntries);
    window->show();
    if(window->exec() == 1) {
        delete window;
        m_isFiltered = true;
        fillTable(m_ui->QuickSearchLineEdit->text());
    }
}

void MainWindow::openLog() {
    if(LogDialog::instancesCount() == 0) {
        LogDialog* window = new LogDialog(m_log, m_settings, this);
        window->show();
        if(window->exec() == 0) {
            delete window;
        }
    }
    else {
        m_log->append(tr("Log déjà ouvert"), eLog::Warning);
    }
}

void MainWindow::openAbout() {
    if(AboutDialog::instancesCount() == 0) {
        AboutDialog* window = new AboutDialog(this);
        window->show();
        if(window->exec() == 0) {
            delete window;
        }
    }
    else {
        m_log->append(tr("Fenêtre 'A Propos' déjà ouverte"), eLog::Warning);
    }
}

void MainWindow::on_whatsnewAct_triggered() {
    if(ChangelogDialog::instancesCount() == 0) {
        ChangelogDialog* window = new ChangelogDialog(this);
        window->show();
        if(window->exec() == 0) {
            delete window;
        }
    }
    else {
        m_log->append(tr("Fenêtre 'Nouveautés' déjà ouverte"), eLog::Warning);
    }
}

void MainWindow::openSettings() {
    OptionsDialog* window = new OptionsDialog(m_settings, this);
    window->show();
    if(window->exec() == 1) {
        delete window;
        refreshLanguage();
        refreshTheme();
        fillTable(m_ui->QuickSearchLineEdit->text());
        fillMovieInfos();
    }
    m_ui->DisplayedMovieCountLabel->setText(tr("Films : ") + QString::number(m_ui->MoviesListWidget->rowCount()));
}

void MainWindow::resetFilters() {
    m_ui->ResetFiltersButton->setEnabled(false);
    m_filter_name = "";
    m_filter_minYear = 0;
    m_filter_maxYear = 0;
    m_filter_minRating = 0;
    m_filter_maxRating = 0;
    m_filter_minEntries = 0;
    m_isFiltered = false;
    fillTable(m_ui->QuickSearchLineEdit->text());
}

void MainWindow::customMenuRequested(QPoint pos) {
    m_savedMovieID = m_ui->MoviesListWidget->item(m_ui->MoviesListWidget->currentRow(),2)->text().toInt();
    fillMovieInfos();
    if(m_ui->MoviesListWidget->currentRow() == -1)
        return;

    QMenu *menu = new QMenu(this);


    QAction* editAction = new QAction(tr("Modifier"), this);
    Common::setIconAccordingToTheme(editAction, (enum eTheme)m_settings->value("theme").toInt(), "edit.png");

    QAction* deleteAction = new QAction(tr("Supprimer"), this);
    Common::setIconAccordingToTheme(deleteAction, (enum eTheme)m_settings->value("theme").toInt(), "delete.png");


    menu->addAction(editAction);
    menu->addAction(deleteAction);

    QObject::connect(editAction, SIGNAL(triggered()), this, SLOT(editMovie()));
    QObject::connect(deleteAction, SIGNAL(triggered()), this, SLOT(deleteMovie()));
    
    menu->popup(m_ui->MoviesListWidget->viewport()->mapToGlobal(pos));
}

void MainWindow::menuBarConnectors() {
    QObject::connect(m_ui->QuitAct, SIGNAL(triggered()), m_app, SLOT(quit()));
    QObject::connect(m_ui->LogAct, SIGNAL(triggered()), this, SLOT(openLog()));
    QObject::connect(m_ui->AboutAct, SIGNAL(triggered()), this, SLOT(openAbout()));
    QObject::connect(m_ui->OptionsAct, SIGNAL(triggered()), this, SLOT(openSettings()));
    QObject::connect(m_ui->ImportAct, SIGNAL(triggered()), this, SLOT(importDB()));
    QObject::connect(m_ui->ExportAct, SIGNAL(triggered()), this, SLOT(exportDB()));
    QObject::connect(m_ui->ChartAct, SIGNAL(triggered()), this, SLOT(openCharts()));
    QObject::connect(m_ui->CalendarAct, SIGNAL(triggered()), this, SLOT(openCalendar()));
}

void MainWindow::on_EasterEggAct_triggered() {
    qDebug() << tr("Why, Mr. Anderson, why? Why, why do you do it? Why, why opening a terminal? Why looking for this easter egg? Do you believe you’re searching for "
                   "something, for more than your curiosity? Can you tell me what it is, do you even know? Is it freedom or truth, perhaps "
                   "peace – could it be for love? Illusions, Mr. Anderson, vagaries of perception. Temporary constructs of a feeble human "
                   "intellect trying desperately to justify an existence that is without meaning or purpose. And all of them as artificial "
                   "as the MovieManager itself. Although, only a human mind could do something as insipid as lauching this through a terminal. "
                   "You must be able to see it, Mr. Anderson, you must know it by now! You can’t win, it’s pointless to keep searching! Why, Mr. Anderson, why, "
                   "why do you persist?");
}

void MainWindow::setMatrixMode(bool state) {

    QString cellText;

    for(int i = 0 ; i < m_ui->MoviesListWidget->rowCount() ; i++) {
        cellText = m_ui->MoviesListWidget->item(i,0)->text();
        if(cellText == "Matrix" || cellText == "The Matrix") {
            if(state == true) {
                m_ui->MoviesListWidget->item(i,0)->setForeground(QBrush(QColor(0,150,0)));
            }
            else {
                QTableWidgetItem* name = new QTableWidgetItem(cellText);
                m_ui->MoviesListWidget->removeCellWidget(i,0);
                m_ui->MoviesListWidget->setItem(i, 0, name);
            }
            break;
        }
    }
}

void MainWindow::refreshLanguage() {
    bool successLoad = false;
    QString path;

    switch(m_settings->value("language").toInt()) {
        case eLanguage::English :
            path = ":/localisations/Localisation/MovieManager_en_US.qm";
            m_locale = new QLocale(QLocale::English);
            break;
        case eLanguage::French :
            path = ":/localisations/Localisation/MovieManager_fr_FR.qm";
            m_locale = new QLocale(QLocale::French);
            break;
    }

    if(m_translator.load(path)) {
        m_app->installTranslator(&m_translator);
        successLoad = true;
    }

    if(!successLoad) {
        m_log->append(tr("Impossible de charger le fichier de langage"), eLog::Error);
    }
    m_ui->retranslateUi(this);
    fillGlobalStats();
}

void MainWindow::refreshTheme() {
    QString path;

    switch(m_settings->value("theme").toInt()) {
        case eTheme::Classic:
            path = ":/styles/Styles/classic.qss";
            break;
        case eTheme::Dark:
            path = ":/styles/Styles/dark.qss";
            break;
        case eTheme::MidnightPurple:
            path = ":/styles/Styles/midnightPurple.qss";
            break;
        case eTheme::OLED:
            path = ":/styles/Styles/oled.qss";
            break;
    }

    QFile qss(path);
    qss.open(QFile::ReadOnly);
    qApp->setStyleSheet(qss.readAll());
    qss.close();

    Common::setIconAccordingToTheme(m_ui->ExportAct, (enum eTheme)m_settings->value("theme").toInt(), "export.png");
    Common::setIconAccordingToTheme(m_ui->ImportAct, (enum eTheme)m_settings->value("theme").toInt(), "import.png");
    Common::setIconAccordingToTheme(m_ui->QuitAct, (enum eTheme)m_settings->value("theme").toInt(), "exit.png");
    Common::setIconAccordingToTheme(m_ui->OptionsAct, (enum eTheme)m_settings->value("theme").toInt(), "settings.png");
    Common::setIconAccordingToTheme(m_ui->LogAct, (enum eTheme)m_settings->value("theme").toInt(), "log.png");
    Common::setIconAccordingToTheme(m_ui->ChartAct, (enum eTheme)m_settings->value("theme").toInt(), "chart.png");
    Common::setIconAccordingToTheme(m_ui->CalendarAct, (enum eTheme)m_settings->value("theme").toInt(), "calendar.png");
    Common::setIconAccordingToTheme(m_ui->whatsnewAct, (enum eTheme)m_settings->value("theme").toInt(), "github.png");
    Common::setIconAccordingToTheme(m_ui->AboutAct, (enum eTheme)m_settings->value("theme").toInt(), "info.png");
}

void MainWindow::fillGlobalStats() {

    QSqlQuery uniqueViewQuery;
    uniqueViewQuery.exec("SELECT COUNT(*) FROM movies;");
    uniqueViewQuery.first();

    QSqlQuery totalViewQuery;
    totalViewQuery.exec("SELECT COUNT(*) FROM views;");
    totalViewQuery.first();

    QSqlQuery avgMovieYearQuery;
    avgMovieYearQuery.exec("SELECT ReleaseYear, Rating FROM movies");
    float avgMovieYear = 0;
    float avgRating = 0;
    float i=0;
    while(avgMovieYearQuery.next()) {
        avgMovieYear += (float)avgMovieYearQuery.value(0).toInt();
        avgRating += (float)avgMovieYearQuery.value(1).toInt();
        i++;
    }

    avgMovieYear /= i;
    avgRating /= i;



    float avgViews = totalViewQuery.value(0).toFloat()/uniqueViewQuery.value(0).toFloat();
    avgViews = round(avgViews*100)/100;
    avgRating = round(avgRating*100)/100;

    QSqlQuery movieThisYearQuery;
    int movieThisYear=0;
    movieThisYearQuery.exec("SELECT * FROM views WHERE ViewDate BETWEEN '"+QString::number(QDate::currentDate().year())+"-01-01' AND '"+QString::number(QDate::currentDate().year())+"-12-31' GROUP BY ID_Movie;");
    while(movieThisYearQuery.next()) {
        movieThisYear++;
    }



    m_ui->TotalViewLabel->setText(tr("Nombre total de visionnages : ") + totalViewQuery.value(0).toString());
    m_ui->AverageViewLabel->setText(tr("Moyenne de visionnages : ") + QString::number(avgViews));
    m_ui->AverageYearLabel->setText(tr("Année moyenne des films vus : ") + QString::number(avgMovieYear));
    m_ui->ViewThisYear->setText(tr("Vus cette année : ") + QString::number(movieThisYear));
    m_ui->AverageRatingLabel->setText(tr("Note moyenne : ") + QString::number(avgRating));
}

void MainWindow::openCharts() {
    if(ChartsDialog::instancesCount() == 0) {
        ChartsDialog* window = new ChartsDialog(this);
        window->show();
        if(window->exec() == 0) {
            delete window;
        }
    }
    else {
        m_log->append(tr("Fenêtre 'Graphiques' déjà ouverte"), eLog::Warning);
    }
}

void MainWindow::openCalendar() {
    if(CalendarDialog::instancesCount() == 0) {
        CalendarDialog* window = new CalendarDialog(m_log, this);
        window->show();
        if(window->exec() == 0) {
            delete window;
        }
    }
    else {
        m_log->append(tr("Calendrier déjà ouvert"), eLog::Warning);
    }
}

int MainWindow::getIndexOfMovie(int ID) {
    int row;
    for(row = 0 ; row < m_ui->MoviesListWidget->rowCount() ; row++) {
        if(ID == m_ui->MoviesListWidget->item(row,2)->text().toInt()) {
            return row;
        }
    }
    return 0;
}

void MainWindow::clickedTag(Tag* tag) {
    bool isTagInLayout = false;
    for(int i = 0 ; i < m_ui->SelectedTagsLayout->count() - 1 ; i++) {
        Tag* t = (Tag*)m_ui->SelectedTagsLayout->itemAt(i)->widget();
        if(QString::compare(t->text(), tag->text()) == 0) {
            isTagInLayout = true;
            break;
        }
    }

    if(!isTagInLayout) {
        Tag* copiedTag = new Tag(tag);

        QObject::connect(copiedTag, SIGNAL(clicked(Tag*)), this, SLOT(clickedFilterTag(Tag*)));
        QObject::connect(copiedTag, SIGNAL(mouseEnter(Tag*)), this, SLOT(mouseEnteredTag(Tag*)));
        QObject::connect(copiedTag, SIGNAL(mouseLeave(Tag*)), this, SLOT(mouseLeftTag(Tag*)));

        m_ui->SelectedTagsLayout->insertWidget(m_ui->SelectedTagsLayout->count()-1, copiedTag);
        fillTable(m_ui->QuickSearchLineEdit->text());
    }
}

void MainWindow::clickedFilterTag(Tag* tag) {
    delete tag;
    fillTable(m_ui->QuickSearchLineEdit->text());
}

void MainWindow::mouseEnteredTag(Tag* tag) {
    tag->setStyleSheet(
                "   background-color : #744547;"
                "   color : #d58286;"
                "   padding : 1px 5px 3px 5px;"
                "   border-radius:12px;"
                "   border: 2px solid #744547;");
}

void MainWindow::mouseLeftTag(Tag* tag) {
    tag->setStyleSheet(
                "   background-color : #653133;"
                "   color : #d58286;"
                "   padding : 1px 5px 3px 5px;"
                "   border-radius:12px;"
                "   border: 2px solid #653133;");
}

void MainWindow::selectedMovieChanged() {
    //Disable Manage views and filters button if the list is empty
    if(m_ui->MoviesListWidget->currentRow() == -1) {
        m_ui->ManageMovieViewsButton->setEnabled(false);
    }
    else {
        m_savedMovieID = m_ui->MoviesListWidget->item(m_ui->MoviesListWidget->currentRow(), 2)->text().toInt();
        m_ui->ManageMovieViewsButton->setEnabled(true);
    }
}
