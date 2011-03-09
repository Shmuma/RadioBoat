#include <QtGui/QApplication>
#include <QCoreApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName ("Shmuma");
    QCoreApplication::setOrganizationDomain ("www.shmuma.ru");
    QCoreApplication::setApplicationName ("BoatControllerGUI");

    MainWindow w;
    w.show();

    return a.exec();
}
