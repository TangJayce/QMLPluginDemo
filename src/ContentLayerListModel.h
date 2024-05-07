//
// Created by Jayce on 2024/4/26.
//

#pragma once

#include "ElementTreeModel.h"

namespace MOZA::DashboardEditor
{

class ContentLayerListModel: public ElementTreeModel
{
    Q_OBJECT
public:
    explicit ContentLayerListModel(QObject *parent = nullptr);

//    Q_INVOKABLE void switchSelect(int n);
//    Q_INVOKABLE void attachSelect(int n);
//    Q_INVOKABLE void multipleSelect(int prev, int cur);
//    Q_INVOKABLE int dragGapIndex(int idx);
//    Q_INVOKABLE int itemIndexDepth(int itemIdx);
//    Q_INVOKABLE void moveSingleItem(int itemIdx, int gapIdx);
//    Q_INVOKABLE void toggleHide(int n);
//    Q_INVOKABLE void toggleLock(int n);

private:
    enum LayerListModelRoles {
        HAS_TOP_RADIUS = ElementTreeModel::HAS_CHILD + 1,
        HAS_BOTTOM_RADIUS,
        SELECT_STATUS,
        HIDE_STATUS,
        LOCK_STATUS
    };
};

}