#include "huecontrolwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    HueControlWindow w;
    w.show();

    return a.exec();
}
