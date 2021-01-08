#include "lyli.h"

#include <QApplication>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    Lyli lyli;
    lyli.start();

    int res = a.exec();

    lyli.stop();

    return res;
}

