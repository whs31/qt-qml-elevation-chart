#include "example.h"
#include "./ui_example.h"
#include <qqml.h>
#include "elevationchart.hpp"
#include "fpscounter.hpp"


IntegrationExample::IntegrationExample(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::IntegrationExample)
{
    // выполняем всегда
    qmlRegisterType<FPSCounter>("FPS", 1, 0, "FPS");

    // регистрация типа в мета-системе QML
    // необходимо выполнить ее до того, как объект будет инстанциирован в QML-е

    // для инстанциирования в QML-е
    qmlRegisterType<ElevationChart>("ElevationChart", 1, 0, "ElevationChart");

    // для инстанциирования в C++ с передачей в QML (не тестил)
    //qmlRegisterSingletonInstance<ElevationChart>("ElevationChart", 1, 0, "ElevationChart", new ElevationChart(this));
    // альтернативный вариант (не тестил)
    //ui->quickWidget->rootContext()->setContextProperty("ElevationChart", new ElevationChart(this));

    ui->setupUi(this);
    ui->quickWidget->setSource(QUrl("qrc:/example.qml"));
    ui->quickWidget->show();
}

IntegrationExample::~IntegrationExample()
{
    delete ui;
}

