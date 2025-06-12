#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "fridge_app.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    FridgeApp fridgeApp;
    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("fridgeApp", &fridgeApp);
    engine.rootContext()->setContextProperty("fridgeModel", fridgeApp.getModel());
    /* const QUrl url(u"qrc:/qt/qml/Fridge/Main.qml"_qs);*/
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
                         if (!obj && url == objUrl)
                             QCoreApplication::exit(-1);
                     }, Qt::QueuedConnection);
    engine.load(url);
    return app.exec();
}
