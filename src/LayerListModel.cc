//
// Created by Jayce on 2024/4/26.
//

#include "LayerListModel.h"

namespace MOZA::DashboardEditor
{

LayerListModel::LayerListModel(QObject *parent) : MozaTreeModel(parent)
{
    m_roleNames.insert(HAS_TOP_RADIUS, "hasTopRadius");
    m_roleNames.insert(HAS_BOTTOM_RADIUS, "hasBottomRadius");
    m_roleNames.insert(SELECT_STATUS, "selectStatus");
}

}