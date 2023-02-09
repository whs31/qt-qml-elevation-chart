#ifndef INTEGRATIONEXAMPLE_H
#define INTEGRATIONEXAMPLE_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class IntegrationExample; }
QT_END_NAMESPACE

class IntegrationExample : public QMainWindow
{
    Q_OBJECT

public:
    IntegrationExample(QWidget *parent = nullptr);
    ~IntegrationExample();

private:
    Ui::IntegrationExample *ui;
};
#endif // INTEGRATIONEXAMPLE_H
