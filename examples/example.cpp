 #include "example.h"
#include "./ui_example.h"
#include <qqml.h>


IntegrationExample::IntegrationExample(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::IntegrationExample)
{
    elevationWidget = new GLCharts::ElevationWidget(this);

    ui->setupUi(this);
    ui->quickWidget->setSource(QUrl("qrc:/example.qml"));
    ui->quickWidget->show();

    elevationWidget->linkWithQML(ui->quickWidget->rootObject());

    ui->checkBox->setChecked(true);
    ui->doubleSpinBox->setValue(75);
    ui->doubleSpinBox_2->setValue(1);
    ui->doubleSpinBox_3->setValue(1);
    ui->doubleSpinBox_4->setValue(100);
    ui->doubleSpinBox_5->setValue(100);
}

IntegrationExample::~IntegrationExample()
{
    delete ui;
}

void IntegrationExample::on_checkBox_toggled(bool checked)
{
//    elevationWidget->showIndexes(checked);
}
void IntegrationExample::on_doubleSpinBox_valueChanged(double arg1)
{
    elevationWidget->setGlobalVelocity(arg1);
}
void IntegrationExample::on_doubleSpinBox_2_valueChanged(double arg1)
{
    elevationWidget->setClimbRate(arg1);
}
void IntegrationExample::on_doubleSpinBox_3_valueChanged(double arg1)
{
    elevationWidget->setDescendRate(arg1);
}
void IntegrationExample::on_doubleSpinBox_4_valueChanged(double arg1)
{
    elevationWidget->setEnvelopeMinimumAltitude(arg1);
}
void IntegrationExample::on_doubleSpinBox_5_valueChanged(double arg1)
{
    elevationWidget->setEnvelopeCoridorSize(arg1);
}
void IntegrationExample::on_pushButton_clicked()
{
    elevationWidget->applyMetricsCorrection();
}

void IntegrationExample::on_pushButton_2_clicked()
{
    elevationWidget->setRoute({ GLCharts::GeoPoint(60, 30, 50, 50),
                                GLCharts::GeoPoint(60.1, 30.1, 100, 100),
                                GLCharts::GeoPoint(60.2, 30.2, 25, 50),
                                GLCharts::GeoPoint(60.3, 30.3, 45, 50),
                                GLCharts::GeoPoint(60.4, 30.4, 35, 50),
                                GLCharts::GeoPoint(60.5, 30.5, 50, 50)
                              });
}

void IntegrationExample::on_pushButton_3_clicked() {
    elevationWidget->setRoute({});
}
void IntegrationExample::on_pushButton_4_clicked() {
    elevationWidget->estimateEnvelope();
}
void IntegrationExample::on_pushButton_5_clicked() {
    elevationWidget->applyEnvelopeCorrection();
}

void IntegrationExample::on_pushButton_6_clicked()
{
    double lat = 60 + static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    double lon = 30 + static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    elevationWidget->setUAVPosition(lat, lon);
    qDebug() << "<example> Moved UAV to" << lat << "," << lon;
}


void IntegrationExample::on_horizontalSlider_0_valueChanged(int value)
{
    elevationWidget->setRoute({ GLCharts::GeoPoint(60, 30, 50, 50),
                                GLCharts::GeoPoint(60.1, 30.1, 100, 100),
                                GLCharts::GeoPoint(60.2, 30.2, 25, 50),
                                GLCharts::GeoPoint(60.3, 30.3, 45, 50),
                                GLCharts::GeoPoint(60.4, 30.4, 35, 50),
                                GLCharts::GeoPoint(60.5, 30.5, 50, 50),
                                GLCharts::GeoPoint(60 + (float)value / 100, 30 + (float)value / 100, 50, 50)
                              });
}
