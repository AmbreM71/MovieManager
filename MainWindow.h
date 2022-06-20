#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        explicit MainWindow(QApplication* app, QWidget *parent = nullptr);
        ~MainWindow();

    private:
        Ui::MainWindow* m_ui;
        QApplication* m_app;
};

#endif // MAINWINDOW_H
