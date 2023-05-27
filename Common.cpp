#include "Common.h"

Log* Common::Log = new class Log();
QSettings* Common::Settings = new class QSettings("MovieManager", "MovieManager");

Common::Common()
{

}

void Common::loadPoster(QWidget* parent, QLabel* poster, int posterHeight, float safeRatio, QString path, QString* resultpath) {
    QString posterPath;
    if (path == "") {
        bool extOK;
        do {
            QString temp = "";
            extOK = true;
            temp = QFileDialog::getOpenFileName(parent, QObject::tr("Selectionner une affiche"), QString(), "Image (*.png; *.jpg; *.jpeg )");
            QString ext = temp;
            ext = ext.remove(0, temp.lastIndexOf(".")+1);
            // Test if file is a jpg or a png
            if(QString::compare(ext, "png", Qt::CaseInsensitive) != 0
            && QString::compare(ext, "jpg", Qt::CaseInsensitive) != 0
            && QString::compare(ext, "jpeg", Qt::CaseInsensitive) != 0
            && temp.size() > 0) {
                QMessageBox::critical(parent, QObject::tr("Format incorrect"), QObject::tr("Le format de l'image est incorrect\nVeuillez sélectionner un fichier au format jpg, png ou jpeg"));
                extOK = false;
            }
            else {
                //This is to avoid spamming file selection dialog when closing dialog without selecting a file
                if(temp.size() > 0) {
                    posterPath = temp;
                }
                else {
                    return;
                }
            }
        } while (extOK == false);
    }
    else
        posterPath = path;

    if(posterPath != "") {
        QPixmap pixmap;
        //If no movie has no cover
        QFileInfo imgFile(posterPath);
        if(imgFile.exists(posterPath) && imgFile.isFile()) {
            pixmap.load(posterPath);
        }
        else {
            pixmap.load(":/assets/Assets/nocover.png");
        }
        QPixmap pm;

        //If picture is too wide, poster is scaled to width to fit in UI (safe until 4:3)
        if((float)pixmap.height()/(float)pixmap.width() < safeRatio) {
            pm = pixmap.scaledToWidth(posterHeight/safeRatio, Qt::SmoothTransformation);
        }
        else {
            pm = pixmap.scaledToHeight(posterHeight, Qt::SmoothTransformation);
        }
        poster->setPixmap(pm);
    }
    if(resultpath != nullptr) {
        *resultpath = posterPath;
    }
}

void Common::ratingToStar(int rating, QLabel* ratingLabel) {

    ratingLabel->setText("");
    QPixmap* pixmap;

    switch(rating) {
        case 0:
            pixmap = new QPixmap(":/assets/Assets/stars/0.png");
            break;
        case 1:
            pixmap = new QPixmap(":/assets/Assets/stars/1.png");
            break;
        case 2:
            pixmap = new QPixmap(":/assets/Assets/stars/2.png");
            break;
        case 3:
            pixmap = new QPixmap(":/assets/Assets/stars/3.png");
            break;
        case 4:
            pixmap = new QPixmap(":/assets/Assets/stars/4.png");
            break;
        case 5:
            pixmap = new QPixmap(":/assets/Assets/stars/5.png");
            break;
        case 6:
            pixmap = new QPixmap(":/assets/Assets/stars/6.png");
            break;
        case 7:
            pixmap = new QPixmap(":/assets/Assets/stars/7.png");
            break;
        case 8:
            pixmap = new QPixmap(":/assets/Assets/stars/8.png");
            break;
        case 9:
            pixmap = new QPixmap(":/assets/Assets/stars/9.png");
            break;
        case 10:
            pixmap = new QPixmap(":/assets/Assets/stars/10.png");
            break;
        default:
            pixmap = new QPixmap();
            break;
    }

    QPixmap resizedPixmap = pixmap->scaledToHeight(20, Qt::SmoothTransformation);
    ratingLabel->setPixmap(resizedPixmap);
}

void Common::clearLayout(QLayout* layout) {
    if (layout == NULL)
        return;
    QLayoutItem *item;
    while((item = layout->takeAt(0))) {
        if (item->layout()) {
            clearLayout(item->layout());
            delete item->layout();
        }
        if (item->widget()) {
           delete item->widget();
        }
        delete item;
    }
}

QString Common::viewTypeToQString(enum eViewType type) {
    if(type == eViewType::Bluray) {
        return QObject::tr("Blu-Ray");
    }
    if(type == eViewType::Television) {
        return QObject::tr("Télévision");
    }
    if(type == eViewType::Cinema) {
        return QObject::tr("Cinéma");
    }
    if(type == eViewType::CinemaReplay) {
        return QObject::tr("Cinéma (reprise)");
    }
    if(type == eViewType::DVD) {
        return QObject::tr("DVD");
    }
    if(type == eViewType::Streaming) {
        return QObject::tr("Streaming");
    }
    if(type == eViewType::Download) {
        return QObject::tr("Téléchargement");
    }
    if(type == eViewType::VHS) {
        return QObject::tr("VHS");
    }
    if(type == eViewType::Other) {
        return QObject::tr("Autre support physique");
    }
    if(type == eViewType::Unknown) {
        return QObject::tr("Inconnu");
    }
    if(type == eViewType::UHD) {
        return QObject::tr("Blu-Ray 4K");
    }
    else {
        qDebug() << "Error";
        //Log(QObject::tr("Type de visionnage inconnu"), Error);
        return "Unknown";
    }
}

enum eViewType Common::QStringToViewType(QString type) {
    if(QString::compare(type, QObject::tr("Blu-Ray")) == 0) {
        return eViewType::Bluray;
    }
    else if(QString::compare(type, QObject::tr("Télévision")) == 0) {
        return eViewType::Television;
    }
    else if(QString::compare(type, QObject::tr("Cinéma")) == 0) {
        return eViewType::Cinema;
    }
    else if(QString::compare(type, QObject::tr("Cinéma (reprise)")) == 0) {
        return eViewType::CinemaReplay;
    }
    else if(QString::compare(type, QObject::tr("DVD")) == 0) {
        return eViewType::DVD;
    }
    else if(QString::compare(type, QObject::tr("Streaming")) == 0) {
        return eViewType::Streaming;
    }
    else if(QString::compare(type, QObject::tr("Téléchargement")) == 0) {
        return eViewType::Download;
    }
    else if(QString::compare(type, QObject::tr("VHS")) == 0) {
        return eViewType::VHS;
    }
    else if(QString::compare(type, QObject::tr("Autre support physique")) == 0) {
        return eViewType::Other;
    }
    else if(QString::compare(type, QObject::tr("Inconnu")) == 0) {
        return eViewType::Unknown;
    }
    else if(QString::compare(type, QObject::tr("Blu-Ray 4K")) == 0) {
        return eViewType::UHD;
    }
    else {
        return eViewType::MaxViewType;
    }
}

void Common::setIconAccordingToTheme(QAction* action, enum eTheme theme, QString filename) {
    if(theme == eTheme::Classic) {
        action->setIcon(QIcon(":/assets/Assets/Icons/Dark/"+filename));
    }
    else {
        action->setIcon(QIcon(":/assets/Assets/Icons/Bright/"+filename));
    }
}

QString Common::getVersion() {
    return "1.0.0";
}
