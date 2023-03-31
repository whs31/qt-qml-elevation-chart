#ifndef INTEGRATIONEXAMPLE_H
#define INTEGRATIONEXAMPLE_H

#include <QMainWindow>
#include "charts/elevationwidget.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class IntegrationExample; }
QT_END_NAMESPACE

class IntegrationExample : public QMainWindow
{
    Q_OBJECT

public:
    IntegrationExample(QWidget* parent = nullptr);
    ~IntegrationExample();



private:
    Ui::IntegrationExample* ui;
    Charts::ElevationWidget* elevationWidget;

    void uiSetup();

    private slots:
    void on_checkBox_toggled(bool checked);
    void on_doubleSpinBox_valueChanged(double arg1);
    void on_doubleSpinBox_2_valueChanged(double arg1);
    void on_doubleSpinBox_3_valueChanged(double arg1);
    void on_doubleSpinBox_4_valueChanged(double arg1);
    void on_doubleSpinBox_5_valueChanged(double arg1);
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();
};
#endif // INTEGRATIONEXAMPLE_H
