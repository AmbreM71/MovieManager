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
