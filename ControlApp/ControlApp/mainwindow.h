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

protected:
    void keyPressEvent (QKeyEvent* event);

protected slots:
    void connectorStateChanged (const QString& state);
    void connectButtonClicked ();
    void optionsButtonClicked ();
    void throttleSliderChanged (int val);
    void rotateSliderChanged (int val);

private:
    Ui::MainWindow *ui;
    BoatController _ctl;
};

#endif // MAINWINDOW_H
