#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "BoatController.hpp"

#include <QLabel>
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _ctl (this)
{
    ui->setupUi(this);
    ui->connectionStateLabel->setText (_ctl.connector ()->state ());
    connect (_ctl.connector (), SIGNAL (stateChanged (const QString&)), this, SLOT (connectorStateChanged (const QString&)));
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
        _ctl.connector()->disconnect();
    else
        if (!_ctl.connector ()->connect (QString ()))
            QMessageBox::warning(this, tr ("Connect failed"), tr ("Connection failed"));
}

