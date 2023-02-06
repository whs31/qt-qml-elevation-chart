#include "integrationexample.h"
#include "./ui_integrationexample.h"
#include <qqml.h>
#include "ec-backend/elevationchart.h"


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

