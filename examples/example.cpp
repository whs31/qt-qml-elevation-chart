#include "example.h"
#include "./ui_example.h"
#include <qqml.h>


IntegrationExample::IntegrationExample(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::IntegrationExample)
{
    elevationWidget = new ElevationWidget(this);

    ui->setupUi(this);
    ui->quickWidget->setSource(QUrl("qrc:/example.qml"));
    ui->quickWidget->show();

    uiSetup();
    elevationWidget->setGeopath(QGeoPath({  QGeoCoordinate(65.01, 30.0, 80),
                                            QGeoCoordinate(65.01, 30.01, 70),
                                            QGeoCoordinate(65.02, 30.02, 60),
                                            QGeoCoordinate(65.03, 30.03, 50),
                                            QGeoCoordinate(65.04, 30.04, 40),
                                            QGeoCoordinate(65.05, 30.05, 50),
                                            QGeoCoordinate(65.06, 30.06, 60),
                                            QGeoCoordinate(65.07, 30.07, 70),
                                            QGeoCoordinate(65.08, 30.08, 80),
                                            QGeoCoordinate(65.09, 30.09, 70),
                                            QGeoCoordinate(65.10, 30.10, 60),
                                            QGeoCoordinate(65.11, 30.11, 50),
                                            QGeoCoordinate(65.12, 30.12, 40),
                                            QGeoCoordinate(65.13, 30.14, 50),
                                            QGeoCoordinate(65.14, 30.14, 60)
                                        })
                                );
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
    ui->doubleSpinBox_4->setValue(10);
    ui->doubleSpinBox_5->setValue(10);
}

void IntegrationExample::on_checkBox_toggled(bool checked)
{ elevationWidget->showIndexes(checked); }

void IntegrationExample::on_doubleSpinBox_valueChanged(double arg1)
{ elevationWidget->setVelocity(arg1); }

void IntegrationExample::on_doubleSpinBox_2_valueChanged(double arg1)
{ elevationWidget->setClimbRate(arg1); }

void IntegrationExample::on_doubleSpinBox_3_valueChanged(double arg1)
{ elevationWidget->setDescendRate(arg1); }

void IntegrationExample::on_doubleSpinBox_4_valueChanged(double arg1)
{ elevationWidget->setBoundHeight(arg1); }

void IntegrationExample::on_doubleSpinBox_5_valueChanged(double arg1)
{ elevationWidget->setBoundWidth(arg1); }

