#include "mainwindow.hpp"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qRegisterMetaType<QList<Eigen::Vector3f>>();
    MainWindow w;
    w.show();
    return a.exec();
}
