#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "BoatController.hpp"
#include "optionsdialog.hpp"

#include <QLabel>
#include <QMessageBox>
#include <QKeyEvent>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _ctl (_opts, this)
{
    ui->setupUi(this);
    ui->connectionStateLabel->setText (_ctl.connector ()->state ());
    connect (&_ctl, SIGNAL (connectorStateChanged (const QString&)), this, SLOT (connectorStateChanged (const QString&)));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::connectorStateChanged (const QString& state)
{
    ui->connectionStateLabel->setText (state);
    ui->connectionOptionsButton->setDisabled (_ctl.connector ()->is_connected ());
    ui->connectButton->setText(_ctl.connector ()->is_connected () ? tr ("&Disconnect") : tr ("&Connect"));
}


void MainWindow::connectButtonClicked ()
{
    if (_ctl.connector ()->is_connected ())
        _ctl.disconnectDevice ();
    else
        if (!_ctl.connectDevice ())
            QMessageBox::warning(this, tr ("Connect failed"), tr ("Connection failed"));
}


void MainWindow::throttleSliderChanged (int val)
{
    ui->throttleLabel->setText (QString ("%1%").arg (val));
    _ctl.setThrottle (val);
}


void MainWindow::rotateSliderChanged (int val)
{
    ui->rotateLabel->setText(QString ("%1%").arg (val));
    _ctl.setRotate (val+50);
}


void MainWindow::keyPressEvent (QKeyEvent* event)
{
    if (event->modifiers () == Qt::NoModifier) {
        switch (event->key ()) {
        case Qt::Key_W:
            ui->throttleSlider->setValue (ui->throttleSlider->value () + 1);
            break;
        case Qt::Key_S:
            ui->throttleSlider->setValue (ui->throttleSlider->value () - 1);
            break;
        case Qt::Key_D:
            ui->rotateSlider->setValue (ui->rotateSlider->value () + 1);
            break;
        case Qt::Key_A:
            ui->rotateSlider->setValue (ui->rotateSlider->value () - 1);
            break;
        case Qt::Key_Z:
            ui->throttleSlider->setValue (0);
            break;
        case Qt::Key_X:
            ui->rotateSlider->setValue (0);
            break;
        default:
            QMainWindow::keyPressEvent (event);

        }
    }
    else
        QMainWindow::keyPressEvent(event);
}


void MainWindow::optionsButtonClicked ()
{
    OptionsDialog dlg (_opts, this);

    if (dlg.exec () == QDialog::Accepted) {
        _opts = dlg.getOptions ();
        _opts.save ();
        _ctl.updateOptions (_opts);
    }
}
