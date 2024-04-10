//
// Created by Jayce on 2024/3/15.
//

#pragma once

#include <QQmlExtensionPlugin>

namespace moza::component
{

class QMLPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    // 告诉moc要导出这个类
    Q_PLUGIN_METADATA(IID QQmlExtensionInterface_iid)

public:
    void registerTypes(const char* uri) override;
};

}