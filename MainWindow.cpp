#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QApplication* app, QWidget *parent) {
    m_ui = new Ui::MainWindow;
    m_ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete m_ui;
}
