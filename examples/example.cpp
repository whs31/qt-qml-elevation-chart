#include "example.h"
#include "./ui_example.h"
#include <qqml.h>
#include "elevationchart.hpp"


IntegrationExample::IntegrationExample(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::IntegrationExample)
{
    elevationWidget = new ElevationWidget(this);
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

    uiSetup();
    elevationWidget->setGeopath(QGeoPath({ QGeoCoordinate(60, 30, 20), QGeoCoordinate(60.1, 30.1, 30) }));
}

IntegrationExample::~IntegrationExample()
{
    delete ui;
}

void IntegrationExample::uiSetup()
{
    ui->checkBox->setChecked(true);
    ui->doubleSpinBox->setValue(75);
    ui->doubleSpinBox_2->setValue(1);
    ui->doubleSpinBox_3->setValue(1);
}

void IntegrationExample::on_checkBox_toggled(bool checked)
{ elevationWidget->showIndexes(checked); }

void IntegrationExample::on_doubleSpinBox_valueChanged(double arg1)
{ elevationWidget->setVelocity(arg1); }

void IntegrationExample::on_doubleSpinBox_2_valueChanged(double arg1)
{ elevationWidget->setClimbRate(arg1); }

void IntegrationExample::on_doubleSpinBox_3_valueChanged(double arg1)
{ elevationWidget->setDescendRate(arg1); }

