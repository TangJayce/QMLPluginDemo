//
// Created by Jayce on 2024/3/20.
//

#include "ElementTreeModel.h"
#include "ElementTree.h"
#include "ArtBoardListModel.h"

namespace MOZA::DashboardEditor
{

ElementTreeModel::ElementTreeModel(QObject *parent)
    : QAbstractListModel(parent)
{
    m_roleNames.insert(ELEMENT_ID, "elementID");
    m_roleNames.insert(DEPTH, "depth");
    m_roleNames.insert(EXPANDED, "expanded");
    m_roleNames.insert(HAS_CHILD, "hasChild");
}

QVariant ElementTreeModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    if (row < 0 || row >= m_currentItems.size()) { return {}; }

    const auto &item = m_currentItems.at(row);
    switch (role) {
        case ELEMENT_ID:
            return item.value->data();
        case DEPTH:
            return item.depth;
        case EXPANDED:
            return item.expanded;
        case HAS_CHILD:
            return item.value->hasChild();
//        case HAS_TOP_RADIUS:
//            if (row == 0) return true;
//            elementID = m_currentItems.at(row - 1).value->data();
//            return m_elements->getProperties(elementID)->selectStatus() == MozaElementAttachProperty::NONE;
//        case HAS_BOTTOM_RADIUS:
//            if (row == m_currentItems.size() - 1) return true;
//            elementID = m_currentItems.at(row + 1).value->data();
//            return m_elements->getProperties(elementID)->selectStatus() == MozaElementAttachProperty::NONE;
//        case SELECT_STATUS:
//            return m_elements->getProperties(elementID)->selectStatus();
//        case HIDE_STATUS:
//            return m_elements->getProperties(elementID)->hideStatus();
//        case LOCK_STATUS:
//            return m_elements->getProperties(elementID)->lockStatus();
        default:
            return {};
    }
}

void ElementTreeModel::dfs(MozaTreeNode *item, int depth)
{
    for (int i = 0; i < item->childCount(); ++i) {
        auto cur = item->child(i);
        m_currentItems.append({cur, depth, cur->hasChild()});
        if (cur->hasChild()) {
            m_expandedItems.insert(cur->data());
            dfs(cur, depth + 1);
        }
    }
}

void ElementTreeModel::setTree(ElementTree *tree)
{
    if (m_tree == tree) { return; }
    m_tree = tree;
    if (m_tree != nullptr && m_tree->getRoot() != nullptr && m_tree->getRoot()->hasChild()) {
        dfs(m_tree->getRoot(), 1);
    }
    emit treeChanged();
}

void ElementTreeModel::expandRow(int n)
{
    if (n < 0 || n >= m_currentItems.size()) { return; }
    auto &item = m_currentItems[n];
    if (item.expanded) { return; }
    item.expanded = true;
    m_expandedItems.insert(item.value->data());
    emit dataChanged(createIndex(n, 0), createIndex(n, 0), {EXPANDED});

    if (!item.value->hasChild()) { return; }

    beginInsertRows(QModelIndex(), n + 1, n + countVisibleItem(item.value) - 1);
    int startIdx = n;
    showModelChildItems(item, startIdx);
    endInsertRows();
}

void ElementTreeModel::collapseRow(int n)
{
    if (n < 0 || n >= m_currentItems.size()) { return; }
    auto &item = m_currentItems[n];
    if (!item.expanded) { return; }
    item.expanded = false;
    m_expandedItems.remove(item.value->data());
    emit dataChanged(createIndex(n, 0), createIndex(n, 0), {EXPANDED});

    if (!item.value->hasChild()) { return; }

    int lastIndex = lastChildIndex(item.value->child(item.value->childCount() - 1));
    beginRemoveRows(QModelIndex(), n + 1, lastIndex);
    m_currentItems.erase(m_currentItems.begin() + n + 1, m_currentItems.begin() + lastIndex + 1);
    endRemoveRows();
}

int ElementTreeModel::itemIndex(int elementID) const
{
    if (elementID <= 0 || m_currentItems.isEmpty()) { return -1; }

    const int totalCount = m_currentItems.size();
    int localCount = qMin(m_lastItemIndex - 1, totalCount - m_lastItemIndex), idx;

    for (int i = 0; i < localCount; ++i) {
        idx = m_lastItemIndex + i;
        if (m_currentItems.at(idx).value->data() == elementID) {
            m_lastItemIndex = idx;
            return m_lastItemIndex;
        }
        idx = m_lastItemIndex - i - 1;
        if (m_currentItems.at(idx).value->data() == elementID) {
            m_lastItemIndex = idx;
            return m_lastItemIndex;
        }
    }
    for (int j = qMax(0, m_lastItemIndex + localCount); j < totalCount; ++j) {
        if (m_currentItems.at(j).value->data() == elementID) {
            m_lastItemIndex = j;
            return j;
        }
    }
    for (int j = qMin(totalCount, m_lastItemIndex - localCount) - 1; j >= 0; --j) {
        if (m_currentItems.at(j).value->data() == elementID) {
            m_lastItemIndex = j;
            return j;
        }
    }
    return -1;
}

int ElementTreeModel::lastChildIndex(MozaTreeNode *node) const
{
    if (!m_expandedItems.contains(node->data())) { return itemIndex(node->data()); }
    MozaTreeNode *parent = node->parentNode();
    MozaTreeNode *nextSiblingNode{};
    while (parent != m_tree->getRoot()) {
        nextSiblingNode = parent->sibling(parent->row() + 1);
        if (nextSiblingNode != nullptr) { break; }
        parent = parent->parentNode();
    }
    int endIndex = nextSiblingNode ? itemIndex(nextSiblingNode->data()) : m_currentItems.size();
    return endIndex - 1;
}

int ElementTreeModel::itemIndexDepth(int itemIdx)
{
    if (itemIdx < 0 || itemIdx >= m_currentItems.size()) { return 0; }
    return m_currentItems.at(itemIdx).depth;
}

//int MozaTreeModel::dragGapIndex(int idx)
//{
//    if (idx < 0 || idx > m_currentItems.size()) { return -1; }
//    if (idx == m_currentItems.size() || m_currentItems.at(idx).value->parentNode() == m_tree->getRoot()) {
//        return -1;
//    }
//    return itemIndex(m_currentItems.at(idx).value->parentNode()->data());
//}

int ElementTreeModel::countVisibleItem(MozaTreeNode *node) const
{
    if (node == nullptr) { return 0; }
    if (!node->hasChild() || !m_expandedItems.contains(node->data())) { return 1; }
    int sum = 1;
    for (int i = 0; i < node->childCount(); ++i) {
        sum += countVisibleItem(node->child(i));
    }
    return sum;
}

void ElementTreeModel::showModelChildItems(const ElementTreeModel::TreeItem &item, int &startIdx)
{
    int childrenCount = item.value->childCount();
    for (int i = 0; i < childrenCount; ++i) {
        MozaTreeNode *cur = item.value->child(i);

        const bool expanded = m_expandedItems.contains(cur->data());
        const TreeItem treeItem {cur, item.depth + 1, expanded};
        m_currentItems.insert(++startIdx, treeItem);
        if (expanded) { showModelChildItems(treeItem, startIdx); }
    }
}

//void MozaTreeModel::switchSelect(int n)
//{
//    if (n < 0 || n >= m_currentItems.size()) { return; }
//
//    auto &currentItem = m_currentItems[n];
//    int firstIndex = n, lastIndex = n, subLastIndex = n;
//    if (currentItem.value->hasChild() && currentItem.expanded) {
//        subLastIndex = lastChildIndex(currentItem.value->child(currentItem.value->childCount() - 1));
//    }
//    for (int idx = 0; idx < m_currentItems.size(); ++idx) {
//        MozaElementAttachProperty::ItemStatus status;
//        if (idx == n) { status = MozaElementAttachProperty::ENABLE; }
//        else if (idx > n && idx <= subLastIndex) { status = MozaElementAttachProperty::SUB_ENABLE; }
//        else { status = MozaElementAttachProperty::NONE; }
//
//        auto prop = m_elements->getProperties(m_currentItems.at(idx).value->data());
//        if (prop->setSelectStatus(status)) {
//            firstIndex = qMin(firstIndex, idx);
//            lastIndex = qMax(lastIndex, idx);
//        }
//    }
//    emit dataChanged(createIndex(firstIndex, 0),
//                     createIndex(lastIndex, 0),
//                     {SELECT_STATUS, HAS_TOP_RADIUS, HAS_BOTTOM_RADIUS});
//}
//
//void MozaTreeModel::attachSelect(int n)
//{
//    if (n < 0 || n >= m_currentItems.size()) { return; }
//
//    // 父子关系应该保留父级节点
//    auto &currentItem = m_currentItems[n];
//    // 父级节点中存在已选中的节点，则该次选择无效
//    auto currentProperty = m_properties->getProperties(currentItem.value->data());
//    auto currentSelectStatus = currentProperty->selectStatus();
//    if (currentSelectStatus == MozaElementAttachProperty::SUB_ENABLE) { return; }
//    if (currentSelectStatus == MozaElementAttachProperty::NONE) {
//        currentProperty->setSelectStatus(MozaElementAttachProperty::ENABLE);
//        currentSelectStatus = MozaElementAttachProperty::SUB_ENABLE;
//    } else {
//        currentProperty->setSelectStatus(MozaElementAttachProperty::NONE);
//        currentSelectStatus = MozaElementAttachProperty::NONE;
//    }
//    int firstIndex = n, lastIndex = n;
//    if (currentItem.value->hasChild() && currentItem.expanded) {
//        int subLastIndex = lastChildIndex(currentItem.value->child(currentItem.value->childCount() - 1));
//        for (int i = n + 1; i <= subLastIndex; ++i) {
//            auto &item = m_currentItems[i];
//            currentProperty = m_properties->getProperties(item.value->data());
//            if (currentProperty->setSelectStatus(currentSelectStatus)) {
//                lastIndex = qMax(lastIndex, i);
//            }
//        }
//    }
//    if (firstIndex > 0 && m_currentItems[firstIndex - 1].selectStatus != NONE) { --firstIndex; }
//    if (lastIndex < m_currentItems.size() - 1 && m_currentItems[lastIndex + 1].selectStatus != NONE) { ++lastIndex; }
//    emit dataChanged(createIndex(firstIndex, 0),
//                     createIndex(lastIndex, 0),
//                     {SELECT_STATUS, HAS_TOP_RADIUS, HAS_BOTTOM_RADIUS});
//}
//
//void MozaTreeModel::multipleSelect(int prev, int cur)
//{
//    int start = qMax(qMin(prev, cur), 0);
//    int last = qMin(qMax(prev, cur), m_currentItems.size() - 1);
//    int firstIndex = start, lastIndex = last;
//    for (int i = start; i <= last; ++i) {
//        auto &currentItem = m_currentItems[i];
//        if (currentItem.selectStatus != NONE) { continue; }
//        currentItem.selectStatus = ENABLE;
//        m_selectedItems.insert(currentItem.value);
//        if (currentItem.value->hasChild() && currentItem.expanded) {
//            int subLastIndex = lastChildIndex(currentItem.value->child(currentItem.value->childCount() - 1));
//            for (int j = i + 1; j <= subLastIndex; ++j) {
//                auto &item = m_currentItems[j];
//                if (item.selectStatus == ENABLE) { m_selectedItems.remove(item.value); }
//                if (item.selectStatus != SUB_ENABLE) {
//                    item.selectStatus = SUB_ENABLE;
//                    lastIndex = qMax(lastIndex, j);
//                }
//            }
//        }
//    }
//    if (firstIndex > 0 && m_currentItems[firstIndex - 1].selectStatus != NONE) { --firstIndex; }
//    if (lastIndex < m_currentItems.size() - 1 && m_currentItems[lastIndex + 1].selectStatus != NONE) { ++lastIndex; }
//    emit dataChanged(createIndex(firstIndex, 0),
//                     createIndex(lastIndex, 0),
//                     {SELECT_STATUS, HAS_TOP_RADIUS, HAS_BOTTOM_RADIUS});
//}
//
//void MozaTreeModel::moveSingleItem(int itemIdx, int gapIdx)
//{
//    if (itemIdx < 0 || itemIdx >= m_currentItems.size()) { return; }
//    if (gapIdx < 0 || gapIdx > m_currentItems.size()) { return; }
//
//    auto &currentItem = m_currentItems[itemIdx];
//    int lastIndex = itemIdx;
//    if (currentItem.value->hasChild() && currentItem.expanded) {
//        lastIndex = lastChildIndex(currentItem.value->child(currentItem.value->childCount() - 1));
//    }
//    // fixme: 此情况下不需要移动，但是有可能修改深度
//    if (gapIdx >= itemIdx && gapIdx <= lastIndex + 1) { return; }
//
//    beginMoveRows({}, itemIdx, lastIndex, {}, gapIdx);
//    // 断开旧的父节点
//    MozaTreeNode *currentNode = currentItem.value;
//    MozaTreeNode *prevParent = currentNode->parentNode();
//    prevParent->removeChild(currentNode->inParentIndex());
//    if (!prevParent->hasChild()) {
//        int idx = itemIndex(prevParent);
//        emit dataChanged(createIndex(idx, 0), createIndex(idx, 0), {HAS_CHILD});
//    }
//    // 接上新的父节点
//    MozaTreeNode *siblingNode = gapIdx == m_currentItems.size() ? nullptr : m_currentItems.at(gapIdx).value;
//    MozaTreeNode *parentNode = siblingNode ? siblingNode->parentNode() : m_tree->getRoot();
//    if (siblingNode != nullptr) {
//        parentNode->insertChild(siblingNode->inParentIndex(), currentNode);
//    } else {
//        parentNode->appendChild(currentNode);
//    }
//
//    int parentIdx = itemIndex(parentNode);
//    int curDepth = currentItem.depth;
//    int parentDepth = ~parentIdx ? m_currentItems.at(parentIdx).depth : 0;
//    int diff = parentDepth + 1 - curDepth;
//    int movedItemSize = lastIndex - itemIdx + 1;
//    QList<TreeItem> movedItems {m_currentItems.begin() + itemIdx, m_currentItems.begin() + lastIndex + 1};
//    ItemStatus hideStatus = NONE, lockStatus = NONE;
//    if (~parentIdx) {
//        auto &parentItem = m_currentItems[parentIdx];
//        if (parentItem.hideStatus != NONE) { hideStatus = SUB_ENABLE; }
//        if (parentItem.lockStatus != NONE) { lockStatus = SUB_ENABLE; }
//    }
//    for (auto &item: movedItems) {
//        item.depth += diff;
//        if (item.hideStatus != ENABLE) { item.hideStatus = hideStatus; }
//        if (item.lockStatus != ENABLE) { item.lockStatus = lockStatus; }
//    }
//    if (itemIdx < gapIdx) {
//        for (int i = itemIdx; i < gapIdx - movedItemSize; ++i) {
//            m_currentItems[i] = m_currentItems[i + movedItemSize];
//        }
//        for (int i = gapIdx - 1, j = movedItemSize - 1; ~j; --i, --j) {
//            m_currentItems[i] = movedItems[j];
//        }
//    } else {
//        for (int i = lastIndex; i >= gapIdx + movedItemSize; --i) {
//            m_currentItems[i] = m_currentItems[i - movedItemSize];
//        }
//        for (int i = gapIdx, j = 0; j < movedItemSize; ++i, ++j) {
//            m_currentItems[i] = movedItems[j];
//        }
//    }
//    endMoveRows();
//    emit dataChanged(createIndex(qMin(gapIdx, itemIdx), 0),
//                     createIndex(qMax(lastIndex, gapIdx - 1), 0),
//                     {DEPTH, VALUE, HIDE_STATUS, LOCK_STATUS});
//}

}
