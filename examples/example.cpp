#include "example.h"
#include "./ui_example.h"
#include <qqml.h>


IntegrationExample::IntegrationExample(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::IntegrationExample)
{
    elevationWidget = new Charts::ElevationWidget(this);

    ui->setupUi(this);
    ui->quickWidget->setSource(QUrl("qrc:/example.qml"));
    ui->quickWidget->show();

    elevationWidget->setGeopath(QGeoPath());
    elevationWidget->setPallete("#2e3440", "#eceff4", "#4c566a",
                                "#a3be8c", "#ebcb8b", "#bf616a",
                                "#0000FF");
    uiSetup();
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
    ui->doubleSpinBox_4->setValue(100);
    ui->doubleSpinBox_5->setValue(100);
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
{ elevationWidget->setEnvelopeMinHeight(arg1); }
void IntegrationExample::on_doubleSpinBox_5_valueChanged(double arg1)
{ elevationWidget->setEnvelopeCoridorSize(arg1); }
void IntegrationExample::on_pushButton_clicked()
{ elevationWidget->applyMetricsCorrection(); }


void IntegrationExample::on_pushButton_2_clicked()
{
    elevationWidget->setGeopath(QGeoPath({
                                            QGeoCoordinate(60.01, 30.0, 5),
                                            QGeoCoordinate(60.01, 30.01, 70),
                                            QGeoCoordinate(60.02, 30.02, 60),
                                            QGeoCoordinate(60.03, 30.03, 50),
                                            QGeoCoordinate(60.04, 30.04, 40),
                                            QGeoCoordinate(60.05, 30.05, 50),
                                            QGeoCoordinate(60.06, 30.06, 60),
                                            QGeoCoordinate(60.07, 30.07, 70),
                                            QGeoCoordinate(60.08, 30.08, 80),
                                            QGeoCoordinate(60.09, 30.09, 70),
                                            QGeoCoordinate(60.10, 30.10, 60),
                                            QGeoCoordinate(60.11, 30.11, 50),
                                            QGeoCoordinate(60.12, 30.12, 20),
                                            QGeoCoordinate(60.13, 30.14, 10),
                                            QGeoCoordinate(61.0, 30.14, 10)
                                        })
                                );
    std::vector<uint8_t> vec = { 30, 40, 30, 40, 70, 10, 20, 30, 80, 100, 200, 100, 200, 100, 100 };
    elevationWidget->setVelocity(vec);
}

void IntegrationExample::on_pushButton_3_clicked() { elevationWidget->setGeopath(QGeoPath()); }
void IntegrationExample::on_pushButton_4_clicked() { elevationWidget->calculateTerrainEnvelope(); }
void IntegrationExample::on_pushButton_5_clicked() { elevationWidget->applyTerrainEnvelope(); }
