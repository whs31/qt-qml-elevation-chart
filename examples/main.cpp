#include "example.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    IntegrationExample w;
    w.show();
    return a.exec();
}
