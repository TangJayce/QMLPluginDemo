//
// Created by Jayce on 2024/4/26.
//

#include "ContentLayerListModel.h"

namespace MOZA::DashboardEditor
{

ContentLayerListModel::ContentLayerListModel(QObject *parent) : ElementTreeModel(parent)
{
    m_roleNames.insert(HAS_TOP_RADIUS, "hasTopRadius");
    m_roleNames.insert(HAS_BOTTOM_RADIUS, "hasBottomRadius");
    m_roleNames.insert(SELECT_STATUS, "selectStatus");
    m_roleNames.insert(LOCK_STATUS, "lockStatus");
    m_roleNames.insert(HIDE_STATUS, "hideStatus");
}

}