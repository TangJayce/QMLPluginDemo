//
// Created by Jayce on 2024/3/15.
//

#include "QMLPlugin.h"

#include "MozaUser.h"
#include "MozaTreeModel.h"
#include "MozaEditorFileManager.h"

namespace moza::component
{

void QMLPlugin::registerTypes(const char *uri)
{
    qmlRegisterType<MozaUser>(uri, 1, 0, "MozaUser");
    qmlRegisterUncreatableType<MozaTreeModel>(uri, 1, 0, "MozaTreeModel", "Cannot create MozaTreeModel");
    qmlRegisterSingletonType<MozaEditorFileManager>(uri, 1, 0, "MozaEditorFileManager",
                                                     [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject * {
        return MozaEditorFileManager::instance();
    });
}

}