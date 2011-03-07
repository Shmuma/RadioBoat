#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "BoatController.hpp"

#include <QLabel>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _ctl (this)
{
    ui->setupUi(this);
    ui->connectionStateLabel->setText (_ctl.connector ()->state ());
}

MainWindow::~MainWindow()
{
    delete ui;
}
