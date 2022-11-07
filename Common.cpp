#include "Common.h"

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
            temp = QFileDialog::getOpenFileName(parent, QObject::tr("Selectionner une affiche"), QString(), "Image (*.png; *.jpg; *.webp)");
            QString ext = temp;
            ext = ext.remove(0, temp.lastIndexOf(".")+1);
            // Test if file is a jpg or a png
            if(QString::compare(ext, "png") != 0 && QString::compare(ext, "jpg") && QString::compare(ext, "webp") != 0 && temp.size() > 0) {
                QMessageBox::critical(parent, QObject::tr("Format incorrect"), QObject::tr("Le format de l'image est incorrect\nVeuillez sélectionner un fichier au format jpg, png ou webm"));
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
        QPixmap* pixmap;
        //If no movie has no cover
        QFileInfo imgFile(posterPath);
        if(imgFile.exists(posterPath) && imgFile.isFile()) {
            pixmap = new QPixmap(posterPath);
        }
        else {
            pixmap = new QPixmap(":/icons/Icons/nocover.png");
        }
        QPixmap pm;

        //If picture is too wide, poster is scaled to width to fit in UI (safe until 4:3)
        if((float)pixmap->height()/(float)pixmap->width() < safeRatio) {
            pm = pixmap->scaledToWidth(posterHeight/safeRatio, Qt::SmoothTransformation);
        }
        else {
            pm = pixmap->scaledToHeight(posterHeight, Qt::SmoothTransformation);
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
            pixmap = new QPixmap(":/icons/Icons/stars/0.png");
            break;
        case 1:
            pixmap = new QPixmap(":/icons/Icons/stars/1.png");
            break;
        case 2:
            pixmap = new QPixmap(":/icons/Icons/stars/2.png");
            break;
        case 3:
            pixmap = new QPixmap(":/icons/Icons/stars/3.png");
            break;
        case 4:
            pixmap = new QPixmap(":/icons/Icons/stars/4.png");
            break;
        case 5:
            pixmap = new QPixmap(":/icons/Icons/stars/5.png");
            break;
        case 6:
            pixmap = new QPixmap(":/icons/Icons/stars/6.png");
            break;
        case 7:
            pixmap = new QPixmap(":/icons/Icons/stars/7.png");
            break;
        case 8:
            pixmap = new QPixmap(":/icons/Icons/stars/8.png");
            break;
        case 9:
            pixmap = new QPixmap(":/icons/Icons/stars/9.png");
            break;
        case 10:
            pixmap = new QPixmap(":/icons/Icons/stars/10.png");
            break;
        default:
            pixmap = new QPixmap();
            break;
    }

    QPixmap resizedPixmap = pixmap->scaledToHeight(20, Qt::SmoothTransformation);
    ratingLabel->setPixmap(resizedPixmap);
}
