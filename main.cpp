#include <iostream>
#include <QApplication>
#include "MainWindow.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    MainWindow window(&app, nullptr);
    window.show();

    return app.exec();
}
