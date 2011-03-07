#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "BoatController.hpp"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected slots:
    void connectorStateChanged (const QString& state);
    void connectButtonClicked ();

private:
    Ui::MainWindow *ui;
    BoatController _ctl;
};

#endif // MAINWINDOW_H
