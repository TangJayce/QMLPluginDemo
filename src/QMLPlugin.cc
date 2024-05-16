//
// Created by Jayce on 2024/3/15.
//

#include "QMLPlugin.h"

#include <QtQml>
#include <QQmlPropertyMap>
#include "ElementTree.h"
#include "MozaTreeModel.h"
#include "DashboardEditorManager.h"
#include "ArtBoardListModel.h"
#include "LayerListModel.h"
#include "ElementInteractor.h"
#include "DashboardStore.h"
#include "CustomPropertyGroup.h"

namespace MOZA::DashboardEditor
{

void QMLPlugin::registerTypes(const char *uri)
{
    globalRegisterTypes();
}

void QMLPlugin::globalRegisterTypes()
{
    qmlRegisterUncreatableType<MozaTree>("MOZA.DashboardEditorModel", 1, 0, "ElementTree", "Cannot create MozaTree");
    qmlRegisterUncreatableType<ElementTree>("MOZA.DashboardEditorModel", 1, 0, "ElementTree", "Cannot create ElementTree");
    qmlRegisterUncreatableType<DashboardStore>("MOZA.DashboardEditorModel", 1, 0, "DashboardStore", "Cannot create DashboardStore");
    qmlRegisterUncreatableType<QQmlPropertyMap>("MOZA.DashboardEditorModel", 1, 0, "QmlPropertyMap", "Cannot create QmlPropertyMap");
    qmlRegisterUncreatableType<CustomPropertyGroup>("MOZA.DashboardEditorModel", 1, 0, "CustomPropertyGroup", "Cannot create CustomPropertyGroup");
    qmlRegisterType<MozaTreeModel>("MOZA.DashboardEditorModel", 1, 0, "MozaTreeModel");
    qmlRegisterType<ArtBoardListModel>("MOZA.DashboardEditorModel", 1, 0, "ArtBoardListModel");
    qmlRegisterType<LayerListModel>("MOZA.DashboardEditorModel", 1, 0, "LayerListModel");
    qmlRegisterType<ElementInteractor>("MOZA.DashboardEditorModel", 1, 0, "ElementInteractor");
    qmlRegisterSingletonType<DashboardEditorManager>("MOZA.DashboardEditorModel", 1, 0, "DashboardEditorManager",
                                                     [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject * {
        return DashboardEditorManager::instance();
    });
}

}
