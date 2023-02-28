#include "example.h"
#include "./ui_example.h"
#include <qqml.h>
#include "elevationchart.hpp"
#include "fpscounter.hpp"


IntegrationExample::IntegrationExample(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::IntegrationExample)
{
    // регистрация типа в мета-системе QML
    // необходимо выполнить ее до того, как объект будет инстанциирован в QML-е
    qmlRegisterType<ElevationChart>("ElevationChart", 1, 0, "ElevationChart");
    qmlRegisterType<FPSCounter>("FPS", 1, 0, "FPS");

    ui->setupUi(this);
    ui->quickWidget->setSource(QUrl("qrc:/example.qml"));
    ui->quickWidget->show();
}

IntegrationExample::~IntegrationExample()
{
    delete ui;
}

