#include "integrationexample.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qSetMessagePattern("[%{time process}] %{if-debug}\033[01;38;05;15m%{endif}%{if-info}\033[01;38;05;146m%{endif}%{if-warning}\033[1;33m%{endif}%{if-critical}\033[1;31m%{endif}%{if-fatal}F%{endif}%{message}\033[0m");

    IntegrationExample w;
    w.show();
    return a.exec();
}
