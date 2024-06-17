#include "mainwindow.hpp"
#include <QApplication>

int main(int argc, char *argv[])
{
    qputenv("QT3D_RENDERER", "opengl");
    QApplication a(argc, argv);
    qRegisterMetaType<QList<Eigen::Vector3f>>();
    MainWindow w;
    w.show();
    return a.exec();
}
