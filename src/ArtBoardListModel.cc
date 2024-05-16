//
// Created by Jayce on 2024/4/25.
//

#include "ArtBoardListModel.h"
#include "ElementTree.h"

namespace MOZA::DashboardEditor
{

ArtBoardListModel::ArtBoardListModel(QObject *parent) : QAbstractListModel(parent)
{
    m_roleNames.insert(ELEMENT_ID, "elementID");
    m_roleNames.insert(HAS_CHILD, "hasChild");
}

QVariant ArtBoardListModel::data(const QModelIndex &index, int role) const
{
    int row = index.row(), size = m_elements.size();
    if (row < 0 || row >= size) { return {}; }
    int id = m_elements.at(size - 1 - row);

    switch (role) {
        case ELEMENT_ID:
            return id;
        case HAS_CHILD:
            return m_tree->getNodeByID(id)->hasChild();
        default:
            return {};
    }
}

void ArtBoardListModel::setTree(ElementTree *tree)
{
    if (m_tree == tree) { return; }
    emit beginResetModel();
    if (m_tree) { m_tree->disconnect(this); }
    m_tree = tree;
    resetElements();
    connect(m_tree, &ElementTree::elementAppended, this, &ArtBoardListModel::appendElement);
    connect(m_tree, &ElementTree::elementRemoved, this, &ArtBoardListModel::removeElement);
    connect(m_tree, &ElementTree::elementReset, this, &ArtBoardListModel::resetElements);
    connect(m_tree, &QObject::destroyed, this, [this]{
        m_tree = nullptr;
        resetElements();
    });
    // todo: connect move element
    emit endResetModel();
}

void ArtBoardListModel::appendElement(MozaTreeNode *node)
{
    int idx = itemIndex(node->parentNode()->data().toInt());
    if (idx == -1) { return; }
    idx = idx + node->row();
    emit beginInsertRows(QModelIndex(), idx, idx);
    m_elements.insert(idx, node->data().toInt());
    emit endInsertRows();
}

void ArtBoardListModel::removeElement(int elementID)
{
    int idx = itemIndex(elementID);
    if (idx == -1) { return; }
    emit beginRemoveRows(QModelIndex(), idx, idx);
    m_elements.removeAt(idx);
    emit endInsertRows();
}

void ArtBoardListModel::resetElements()
{
    emit beginResetModel();
    if (m_tree) {
        m_elements = m_tree->getLeafNodeListByDFS();
    } else {
        m_elements.clear();
    }
    emit endResetModel();
}

int ArtBoardListModel::itemIndex(int elementID) const
{
    if (elementID <= 0 || m_elements.isEmpty()) { return -1; }

    const int totalCount = m_elements.size();
    int localCount = qMin(m_lastItemIndex - 1, totalCount - m_lastItemIndex), idx;

    for (int i = 0; i < localCount; ++i) {
        idx = m_lastItemIndex + i;
        if (m_elements.at(idx) == elementID) {
            m_lastItemIndex = idx;
            return m_lastItemIndex;
        }
        idx = m_lastItemIndex - i - 1;
        if (m_elements.at(idx) == elementID) {
            m_lastItemIndex = idx;
            return m_lastItemIndex;
        }
    }
    for (int j = qMax(0, m_lastItemIndex + localCount); j < totalCount; ++j) {
        if (m_elements.at(j) == elementID) {
            m_lastItemIndex = j;
            return j;
        }
    }
    for (int j = qMin(totalCount, m_lastItemIndex - localCount) - 1; j >= 0; --j) {
        if (m_elements.at(j)  == elementID) {
            m_lastItemIndex = j;
            return j;
        }
    }
    return -1;
}

}