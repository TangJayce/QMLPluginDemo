//
// Created by Jayce on 2024/3/15.
//

#include "QMLPlugin.h"

#include <QtQml>
#include <QQmlPropertyMap>
#include "ElementTree.h"
#include "ElementTreeModel.h"
#include "DashboardEditorManager.h"
#include "ArtBoardListModel.h"
#include "ContentLayerListModel.h"
#include "ElementInteractor.h"
#include "DashboardStore.h"

namespace MOZA::DashboardEditor
{

void QMLPlugin::registerTypes(const char *uri)
{
    globalRegisterTypes();
}

void QMLPlugin::globalRegisterTypes()
{
    qmlRegisterUncreatableType<ElementTree>("MOZA.DashboardEditor", 1, 0, "ElementTree", "Cannot create ElementTree");
    qmlRegisterUncreatableType<DashboardStore>("MOZA.DashboardEditor", 1, 0, "DashboardStore", "Cannot create DashboardStore");
    qmlRegisterUncreatableType<QQmlPropertyMap>("MOZA.DashboardEditor", 1, 0, "QmlPropertyMap", "Cannot create QmlPropertyMap");
    qmlRegisterType<ElementTreeModel>("MOZA.DashboardEditor", 1, 0, "ElementTreeModel");
    qmlRegisterType<ArtBoardListModel>("MOZA.DashboardEditor", 1, 0, "ArtBoardListModel");
    qmlRegisterType<ContentLayerListModel>("MOZA.DashboardEditor", 1, 0, "ContentLayerListModel");
    qmlRegisterType<ElementInteractor>("MOZA.DashboardEditor", 1, 0, "ElementInteractor");
    qmlRegisterSingletonType<DashboardEditorManager>("MOZA.DashboardEditor", 1, 0, "DashboardEditorManager",
                                                     [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject * {
        return DashboardEditorManager::instance();
    });
}

}
