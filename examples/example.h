#ifndef INTEGRATIONEXAMPLE_H
#define INTEGRATIONEXAMPLE_H

#include <QMainWindow>
#include "elevationwidget.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class IntegrationExample; }
QT_END_NAMESPACE

class IntegrationExample : public QMainWindow
{
    Q_OBJECT

public:
    IntegrationExample(QWidget* parent = nullptr);
    ~IntegrationExample();

    private slots:
    void on_checkBox_toggled(bool checked);

    void on_doubleSpinBox_valueChanged(double arg1);

    void on_doubleSpinBox_2_valueChanged(double arg1);

    void on_doubleSpinBox_3_valueChanged(double arg1);

    private:
    Ui::IntegrationExample* ui;
    ElevationWidget* elevationWidget;

};
#endif // INTEGRATIONEXAMPLE_H
