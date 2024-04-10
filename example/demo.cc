//
// Created by Jayce on 2024/4/10.
//

#include <QGuiApplication>
#include <QWindow>
#include <QQmlApplicationEngine>
#include <MozaUser.h>
#include <MozaTreeModel.h>
#include <MozaEditorFileManager.h>

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.addImportPath(QStringLiteral("qrc:/"));
    qmlRegisterType<moza::component::MozaUser>("MOZA.DashboardEditor", 1, 0, "MozaUser");
    qmlRegisterUncreatableType<moza::component::MozaTreeModel>("MOZA.DashboardEditor", 1, 0, "MozaTreeModel", "Cannot create MozaTreeModel");
    qmlRegisterSingletonType<moza::component::MozaEditorFileManager>("MOZA.DashboardEditor", 1, 0, "MozaEditorFileManager",
                                                    [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject * {
                                                        return moza::component::MozaEditorFileManager::instance();
                                                    });

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        }, Qt::QueuedConnection);
    engine.load(url);

    return QGuiApplication::exec();
}
