//
// Created by Jayce on 2024/3/20.
//

#include "MozaTreeModel.h"
#include "MozaTree.h"

namespace moza::component
{

MozaTreeModel::MozaTreeModel(MozaTree* tree, QObject *parent)
    : QAbstractListModel(parent)
    , m_rootNode(tree->getRoot())
{
    m_roleNames.insert(VALUE, "value");
    m_roleNames.insert(DEPTH, "depth");
    m_roleNames.insert(EXPANDED, "expanded");
    m_roleNames.insert(HAS_CHILD, "hasChild");
    m_roleNames.insert(SELECTED_STATUS, "selectedStatus");
    m_roleNames.insert(HAS_TOP_RADIUS, "hasTopRadius");
    m_roleNames.insert(HAS_BOTTOM_RADIUS, "hasBottomRadius");

    if (m_rootNode && m_rootNode->hasChild()) {
        dfs(m_rootNode, 1);
    }
}

MozaTreeModel::~MozaTreeModel()
{
    delete m_rootNode;
}

QHash<int, QByteArray> MozaTreeModel::roleNames() const
{
    return m_roleNames;
}

QModelIndex MozaTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    return hasIndex(row, column, parent) ? createIndex(row, column) : QModelIndex();
}

QModelIndex MozaTreeModel::parent(const QModelIndex &child) const
{
    Q_UNUSED(child)
    return {};
}

int MozaTreeModel::rowCount(const QModelIndex &parent) const
{
    return m_currentItems.size();
}

int MozaTreeModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QVariant MozaTreeModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    if (row < 0 || row >= m_currentItems.size()) { return {}; }

    const auto &item = m_currentItems.at(row);
    switch (role) {
        case VALUE:
            return item.value->data();
        case DEPTH:
            return item.depth;
        case EXPANDED:
            return item.expanded;
        case HAS_CHILD:
            return item.value->hasChild();
        case SELECTED_STATUS:
            return item.selectedStatus;
        case HAS_TOP_RADIUS:
            if (row == 0) return true;
            return m_currentItems[row - 1].selectedStatus == NONE_SELECT;
        case HAS_BOTTOM_RADIUS:
            if (row == m_currentItems.size() - 1) return true;
            return m_currentItems[row + 1].selectedStatus == NONE_SELECT;
        default:
            return {};
    }
}

void MozaTreeModel::dfs(MozaTreeNode *item, int depth)
{
    for (int i = 0; i < item->childCount(); ++i) {
        auto cur = item->child(i);
        m_currentItems.append({cur, depth, true});
        if (cur->hasChild()) {
            m_expandedItems.insert(cur);
            dfs(cur, depth + 1);
        }
    }
}

void MozaTreeModel::expandRow(int n)
{
    if (n < 0 || n >= m_currentItems.size()) { return; }
    auto &item = m_currentItems[n];
    if (item.expanded) { return; }
    item.expanded = true;
    m_expandedItems.insert(item.value);
    emit dataChanged(createIndex(n, 0), createIndex(n, 0), {EXPANDED});

    if (!item.value->hasChild()) { return; }

    beginInsertRows(QModelIndex(), n + 1, n + countVisibleItem(item.value) - 1);
    int startIdx = n;
    showModelChildItems(item, startIdx);
    endInsertRows();
}

void MozaTreeModel::collapseRow(int n)
{
    if (n < 0 || n >= m_currentItems.size()) { return; }
    auto &item = m_currentItems[n];
    if (!item.expanded) { return; }
    item.expanded = false;
    m_expandedItems.remove(item.value);
    emit dataChanged(createIndex(n, 0), createIndex(n, 0), {EXPANDED});

    if (!item.value->hasChild()) { return; }

    int lastIndex = lastChildIndex(item.value->child(item.value->childCount() - 1));
    beginRemoveRows(QModelIndex(), n + 1, lastIndex);
    m_currentItems.erase(m_currentItems.begin() + n + 1, m_currentItems.begin() + lastIndex + 1);
    endRemoveRows();
}

int MozaTreeModel::itemIndex(const MozaTreeNode *node) const
{
    if (node == nullptr || node == m_rootNode || m_currentItems.isEmpty()) { return -1; }

    const int totalCount = m_currentItems.size();
    int localCount = qMin(m_lastItemIndex - 1, totalCount - m_lastItemIndex);

    for (int i = 0; i < localCount; ++i) {
        const TreeItem &item1 = m_currentItems.at(m_lastItemIndex + i);
        if (item1.value == node) {
            m_lastItemIndex = m_lastItemIndex + i;
            return m_lastItemIndex;
        }
        const TreeItem &item2 = m_currentItems.at(m_lastItemIndex - i - 1);
        if (item2.value == node) {
            m_lastItemIndex = m_lastItemIndex - i - 1;
            return m_lastItemIndex;
        }
    }

    for (int j = qMax(0, m_lastItemIndex + localCount); j < totalCount; ++j) {
        const TreeItem &item = m_currentItems.at(j);
        if (item.value == node) {
            m_lastItemIndex = j;
            return j;
        }
    }

    for (int j = qMin(totalCount, m_lastItemIndex - localCount) - 1; j >= 0; --j) {
        const TreeItem &item = m_currentItems.at(j);
        if (item.value == node) {
            m_lastItemIndex = j;
            return j;
        }
    }

    // nothing found
    return -1;
}

int MozaTreeModel::countVisibleItem(MozaTreeNode *node) const
{
    if (node == nullptr) { return 0; }
    if (!node->hasChild() || !m_expandedItems.contains(node)) { return 1; }
    int sum = 1;
    for (int i = 0; i < node->childCount(); ++i) {
        sum += countVisibleItem(node->child(i));
    }
    return sum;
}

void MozaTreeModel::showModelChildItems(const MozaTreeModel::TreeItem &item, int &startIdx)
{
    int childrenCount = item.value->childCount();
    for (int i = 0; i < childrenCount; ++i) {
        MozaTreeNode *cur = item.value->child(i);
        const bool expanded = m_expandedItems.contains(cur);
        SelectStatus status;
        if (m_selectedItems.contains(cur)) {
            status = SELECTED;
        } else if (item.selectedStatus != NONE_SELECT) {
            status = SUB_SELECTED;
        } else {
            status = NONE_SELECT;
        }
        const TreeItem treeItem {cur, item.depth + 1, expanded, status};
        m_currentItems.insert(++startIdx, treeItem);
        if (expanded) {
            showModelChildItems(treeItem, startIdx);
        }
    }
}

void MozaTreeModel::switchSelect(int n)
{
    if (n < 0 || n >= m_currentItems.size()) { return; }

    int firstIndex = n, lastIndex = n;
    for (int idx = 0; idx < m_currentItems.size(); ++idx) {
        auto &item = m_currentItems[idx];
        if (item.selectedStatus != NONE_SELECT) {
            item.selectedStatus = NONE_SELECT;
            firstIndex = qMin(firstIndex, idx);
            lastIndex = qMax(lastIndex, idx);
        }
    }
    m_selectedItems.clear();

    auto &currentItem = m_currentItems[n];
    currentItem.selectedStatus = SELECTED;
    m_selectedItems.insert(currentItem.value);
    if (currentItem.value->hasChild() && currentItem.expanded) {
        int subLastIndex = lastChildIndex(currentItem.value->child(currentItem.value->childCount() - 1));
        for (int i = n + 1; i <= subLastIndex; ++i) {
            m_currentItems[i].selectedStatus = SUB_SELECTED;
        }
        lastIndex = qMax(lastIndex, subLastIndex);
    }

    emit dataChanged(createIndex(firstIndex, 0),
                     createIndex(lastIndex, 0),
                     {SELECTED_STATUS, HAS_TOP_RADIUS, HAS_BOTTOM_RADIUS});
}

void MozaTreeModel::attachSelect(int n)
{
    if (n < 0 || n >= m_currentItems.size()) { return; }

    // 父子关系应该保留父级节点
    auto &currentItem = m_currentItems[n];
    // 父级节点中存在已选中的节点，则该次选择无效
    auto currentSelectedStatus = currentItem.selectedStatus;
    if (currentSelectedStatus == SUB_SELECTED) { return; }
    if (currentSelectedStatus == NONE_SELECT) {
        m_selectedItems.insert(currentItem.value);
        currentItem.selectedStatus = SELECTED;
        currentSelectedStatus = SUB_SELECTED;
    } else {
        m_selectedItems.remove(currentItem.value);
        currentItem.selectedStatus = NONE_SELECT;
        currentSelectedStatus = NONE_SELECT;
    }
    int firstIndex = n, lastIndex = n;
    if (currentItem.value->hasChild() && currentItem.expanded) {
        int subLastIndex = lastChildIndex(currentItem.value->child(currentItem.value->childCount() - 1));
        for (int i = n + 1; i <= subLastIndex; ++i) {
            auto &item = m_currentItems[i];
            if (item.selectedStatus == SELECTED) { m_selectedItems.remove(item.value); }
            item.selectedStatus = currentSelectedStatus;
        }
        lastIndex = qMax(lastIndex, subLastIndex);
    }
    if (firstIndex > 0 && m_currentItems[firstIndex - 1].selectedStatus != NONE_SELECT) { --firstIndex; }
    if (lastIndex < m_currentItems.size() - 1 && m_currentItems[lastIndex + 1].selectedStatus != NONE_SELECT) { ++lastIndex; }
    emit dataChanged(createIndex(firstIndex, 0),
                     createIndex(lastIndex, 0),
                     {SELECTED_STATUS, HAS_TOP_RADIUS, HAS_BOTTOM_RADIUS});
}

void MozaTreeModel::multipleSelect(int prev, int cur)
{
    int start = qMax(qMin(prev, cur), 0);
    int last = qMin(qMax(prev, cur), m_currentItems.size() - 1);
    int firstIndex = start, lastIndex = last;
    for (int i = start; i <= last; ++i) {
        auto &currentItem = m_currentItems[i];
        if (currentItem.selectedStatus != NONE_SELECT) { continue; }
        currentItem.selectedStatus = SELECTED;
        m_selectedItems.insert(currentItem.value);
        if (currentItem.value->hasChild() && currentItem.expanded) {
            int subLastIndex = lastChildIndex(currentItem.value->child(currentItem.value->childCount() - 1));
            for (int j = i + 1; j <= subLastIndex; ++j) {
                auto &item = m_currentItems[j];
                if (item.selectedStatus == SELECTED) { m_selectedItems.remove(item.value); }
                item.selectedStatus = SUB_SELECTED;
            }
            lastIndex = qMax(lastIndex, subLastIndex);
        }
    }
    if (firstIndex > 0 && m_currentItems[firstIndex - 1].selectedStatus != NONE_SELECT) { --firstIndex; }
    if (lastIndex < m_currentItems.size() - 1 && m_currentItems[lastIndex + 1].selectedStatus != NONE_SELECT) { ++lastIndex; }
    emit dataChanged(createIndex(firstIndex, 0),
                     createIndex(lastIndex, 0),
                     {SELECTED_STATUS, HAS_TOP_RADIUS, HAS_BOTTOM_RADIUS});
}

int MozaTreeModel::frameIndex() const
{
    return m_frameIndex;
}

void MozaTreeModel::setFrameIndex(int n)
{
    if (m_frameIndex == n) { return; }
    m_frameIndex = n;
    emit frameIndexChanged();
}

int MozaTreeModel::lastChildIndex(MozaTreeNode *node) const
{
    if (!m_expandedItems.contains(node)) { return itemIndex(node); }
    MozaTreeNode *parent = node->parentNode();
    MozaTreeNode *nextSiblingNode{};
    while (parent != m_rootNode) {
        nextSiblingNode = parent->sibling(parent->row() + 1);
        if (nextSiblingNode != nullptr) { break; }
        parent = parent->parentNode();
    }
    int endIndex = nextSiblingNode ? itemIndex(nextSiblingNode) : m_currentItems.size();
    return endIndex - 1;
}

int MozaTreeModel::dragGapIndex(int idx)
{
    if (idx < 0 || idx > m_currentItems.size()) { return -1; }
    if (idx == m_currentItems.size() || m_currentItems.at(idx).value->parentNode() == m_rootNode) {
        return -1;
    }
    return itemIndex(m_currentItems.at(idx).value->parentNode());
}

int MozaTreeModel::itemIndexDepth(int itemIdx)
{
    if (itemIdx < 0 || itemIdx >= m_currentItems.size()) { return 0; }
    return m_currentItems.at(itemIdx).depth;
}

void MozaTreeModel::moveSingleItem(int itemIdx, int gapIdx)
{
    if (itemIdx < 0 || itemIdx >= m_currentItems.size()) { return; }
    if (gapIdx < 0 || gapIdx > m_currentItems.size()) { return; }

    auto &currentItem = m_currentItems[itemIdx];
    int lastIndex = itemIdx;
    if (currentItem.value->hasChild() && currentItem.expanded) {
        lastIndex = lastChildIndex(currentItem.value->child(currentItem.value->childCount() - 1));
    }
    // 此情况下不需要移动
    if (gapIdx >= itemIdx && gapIdx <= lastIndex + 1) { return; }

    beginMoveRows({}, itemIdx, lastIndex, {}, gapIdx);
    MozaTreeNode *currentNode = m_currentItems.at(itemIdx).value;
    MozaTreeNode *siblingNode = gapIdx == m_currentItems.size() ? nullptr : m_currentItems.at(gapIdx).value;
    MozaTreeNode *parentNode = siblingNode ? siblingNode->parentNode() : m_rootNode;
    currentNode->parentNode()->removeChild(currentNode->inParentIndex());
    if (siblingNode != nullptr) {
        parentNode->insertChild(siblingNode->inParentIndex(), currentNode);
    } else {
        parentNode->appendChild(currentNode);
    }

    int curDepth = m_currentItems.at(itemIdx).depth;
    int nextDepth = gapIdx == m_currentItems.size() ? m_currentItems.first().depth : m_currentItems.at(gapIdx).depth;
    int diff = nextDepth - curDepth;
    int movedItemSize = lastIndex - itemIdx + 1;
    QList<TreeItem> movedItems {m_currentItems.begin() + itemIdx, m_currentItems.begin() + lastIndex + 1};
    for (auto &item: movedItems) {
        item.depth += diff;
    }
    if (itemIdx < gapIdx) {
        for (int i = itemIdx; i < gapIdx - movedItemSize; ++i) {
            m_currentItems[i] = m_currentItems[i + movedItemSize];
        }
        for (int i = gapIdx - 1, j = movedItemSize - 1; ~j; --i, --j) {
            m_currentItems[i] = movedItems[j];
        }
    } else {
        for (int i = lastIndex; i >= gapIdx + movedItemSize; --i) {
            m_currentItems[i] = m_currentItems[i - movedItemSize];
        }
        for (int i = gapIdx, j = 0; j < movedItemSize; ++i, ++j) {
            m_currentItems[i] = movedItems[j];
        }
    }
    endMoveRows();
    emit dataChanged(createIndex(qMin(gapIdx, itemIdx), 0),
                     createIndex(qMax(lastIndex, gapIdx - 1), 0),
                     {DEPTH, VALUE});
}

}
