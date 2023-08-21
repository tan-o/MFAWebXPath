#include "mainwindow.h"

#include <QApplication>
//#include <QSslSocket>
int main(int argc, char *argv[])
{
//    qDebug() << "Device supports OpenSSL: " << QSslSocket::supportsSsl();
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
