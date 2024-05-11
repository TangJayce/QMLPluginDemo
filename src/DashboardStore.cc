//
// Created by Jayce on 2024/5/6.
//

#include "DashboardStore.h"
#include "CustomPropertyGroup.h"

namespace MOZA::DashboardEditor
{

CustomPropertyGroup *DashboardStore::getCustomProperties(int id)
{
    if (id < 0) { return nullptr; }
    if (!m_customProperties.contains(id)) {
        auto group = new CustomPropertyGroup(this);
        auto map = getInternalProperties(id);
        group->resetBoundary(map->value("xOffset").toDouble(),
                             map->value("yOffset").toDouble(),
                             map->value("width").toDouble(),
                             map->value("height").toDouble(),
                             map->value("angle").toInt());
        connect(map, &QQmlPropertyMap::valueChanged,
                group, [group, map](const QString &key, const QVariant &value){
            static QStringList list = {"xOffset", "yOffset", "width", "height", "angle"};
            if (!list.contains(key)) { return; }
            group->resetBoundary(map->value("xOffset").toDouble(),
                                 map->value("yOffset").toDouble(),
                                 map->value("width").toDouble(),
                                 map->value("height").toDouble(),
                                 map->value("angle").toInt());
        });
        m_customProperties.insert(id, group);
    }
    return m_customProperties.value(id);
}

QQmlPropertyMap *DashboardStore::getInternalProperties(int id)
{
    if (id < 0) { return nullptr; }
    if (!m_internalProperties.contains(id)) {
        auto map = new QQmlPropertyMap(this);
        map->insert("angle", qreal(id % 12 * 30));
        map->insert("xOffset", qreal((id + 1) % 7 * 60));
        map->insert("yOffset", qreal((id + 5) % 8 * 48));
        map->insert("width", qreal(id % 40 + 140));
        map->insert("height", qreal(id % 20 + 80));
        m_internalProperties.insert(id, map);
    }
    return m_internalProperties.value(id);
}

}