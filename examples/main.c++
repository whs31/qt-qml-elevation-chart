/**
  *  @file main.c++
  *  @author Dmitry Ryazancev
  *  @date 02.08.23
  *  @copyright Radar-MMS 2023
  */

#include <QtWidgets/QApplication>
#include <QtQml/QQmlEngine>
#include <QtQml/QQmlComponent>
#include <QtQml/QQmlContext>
#include <QtQuick/QQuickWindow>
#include <QtQuickControls2/QQuickStyle>
#include "../src/c++/register.h"

#if defined Q_OS_WINDOWS
#include <windows.h>
#endif

int main(int argc, char* argv[])
{
    #if defined Q_OS_WINDOWS
        FreeConsole();
    #endif

    QApplication app(argc, argv);
    QCoreApplication::setApplicationName(PROJECT_NAME);
    QCoreApplication::setApplicationVersion(PROJECT_VERSION);
    QCoreApplication::setOrganizationName(PROJECT_COMPANY);

    #ifndef Q_OS_WINDOWS
        QApplication::setWindowIcon(QIcon(":/icon.png"));
    #else
        QApplication::setWindowIcon(QIcon(":/icon.ico"));
    #endif

    qInfo().noquote() << QCoreApplication::applicationName() << "version" << QCoreApplication::applicationVersion();

    const QUrl qml_entry(QStringLiteral("qrc:/Main.qml"));

    ElevationChart::registerQMLTypes();

    qputenv("QT_QUICK_CONTROLS_MATERIAL_VARIANT", "Dense");
    QQuickStyle::setStyle("Material");

    QQmlEngine engine;
    QObject::connect(&engine, &QQmlEngine::quit, qApp, &QCoreApplication::quit);

    QQmlComponent component(&engine);
    QQuickWindow::setDefaultAlphaBuffer(true);
    component.loadUrl(qml_entry);
    if(component.isReady())
        component.create();
    else
        qCritical() << "[QML ERROR]" << component.errorString();

    return QApplication::exec();
}
