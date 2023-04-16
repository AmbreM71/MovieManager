#include "AboutDialog.h"
#include "ui_AboutDialog.h"

int AboutDialog::instances = 0;

AboutDialog::AboutDialog(QWidget *parent) : QDialog(parent) {
    instances++;
    m_ui = new Ui::AboutDialog;
    m_ui->setupUi(this);
    this->setWindowIcon(QIcon(":/assets/Assets/Icons/Dark/info.png"));

    m_ui->QtVersionLabel->setText(tr("Qt ") + qVersion());

    QObject::connect(m_ui->GithubButton, SIGNAL(clicked()), this, SLOT(redirectGithub()));
}

AboutDialog::~AboutDialog() {
    instances--;
    delete m_ui;
}

int AboutDialog::instancesCount() {
    return instances;
}

void AboutDialog::redirectGithub() {
    QDesktopServices::openUrl(QUrl("https://github.com/AlexM71/MovieManager", QUrl::TolerantMode));
}
