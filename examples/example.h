#pragma once

#include <QMainWindow>
#include "charts/elevationwidget.hpp"

namespace Ui { class IntegrationExample; }

class IntegrationExample : public QMainWindow
{
    Q_OBJECT

    public:
        IntegrationExample(QWidget* parent = nullptr);
        ~IntegrationExample();

    private:
        Ui::IntegrationExample* ui;
        GLCharts::ElevationWidget* elevationWidget;

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
            void on_pushButton_6_clicked();
            void on_horizontalSlider_0_valueChanged(int value);
};

