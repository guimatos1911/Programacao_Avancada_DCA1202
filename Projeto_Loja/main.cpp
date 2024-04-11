#include "mainloja.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainLoja w;
    w.show();
    return a.exec();
}
