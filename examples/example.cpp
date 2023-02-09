#include "example.h"
#include "./ui_example.h"
#include <qqml.h>
#include "elevationchart.hpp"


IntegrationExample::IntegrationExample(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::IntegrationExample)
{
    // регистрация типа в мета-системе QML
    // необходимо выполнить ее до того, как объект будет инстанциирован в QML-е
    qmlRegisterType<ElevationChart>("ElevationChart", 1, 0, "ElevationChart");

    ui->setupUi(this);
    ui->quickWidget->setSource(QUrl("qrc:/IntegrationExample.qml"));
    ui->quickWidget->show();
}

IntegrationExample::~IntegrationExample()
{
    delete ui;
}

