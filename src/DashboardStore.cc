//
// Created by Jayce on 2024/5/6.
//

#include "DashboardStore.h"

namespace MOZA::DashboardEditor
{

QQmlPropertyMap *DashboardStore::getProperties(int id)
{
    if (!m_elements.contains(id)) {
        auto map = new QQmlPropertyMap(this);
        map->insert("selected", false);
        map->insert("parentSelected", false);
        m_elements.insert(id, map);
    }
    return m_elements.value(id);
}

}