//
// Created by Jayce on 2024/4/26.
//

#pragma once

#include "MozaTreeModel.h"

namespace MOZA::DashboardEditor
{

class LayerListModel: public MozaTreeModel
{
    Q_OBJECT
public:
    explicit LayerListModel(QObject *parent = nullptr);

//    Q_INVOKABLE void switchSelect(int n);
//    Q_INVOKABLE void attachSelect(int n);
//    Q_INVOKABLE void multipleSelect(int prev, int cur);
//    Q_INVOKABLE int dragGapIndex(int idx);
//    Q_INVOKABLE int itemIndexDepth(int itemIdx);
//    Q_INVOKABLE void moveSingleItem(int itemIdx, int gapIdx);

private:
    enum LayerListModelRoles {
        HAS_TOP_RADIUS = MozaTreeModel::HAS_CHILD + 1,
        HAS_BOTTOM_RADIUS,
        SELECT_STATUS
    };
};

}