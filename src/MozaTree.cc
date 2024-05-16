//
// Created by Jayce on 2024/5/16.
//

#include "MozaTree.h"

namespace MOZA::DashboardEditor
{

MozaTreeNode::MozaTreeNode(QVariant data): m_itemData(std::move(data))
{
}

MozaTreeNode::~MozaTreeNode()
{
    qDeleteAll(m_childItems);
}

void MozaTreeNode::appendChild(MozaTreeNode *child)
{
    if (child == nullptr) { return; }
    m_childItems.append(child);
    child->m_parentItem = this;
}

void MozaTreeNode::insertChild(int index, MozaTreeNode *child)
{
    if (child == nullptr || index < 0 || index > m_childItems.size()) { return; }
    m_childItems.insert(index, child);
    child->m_parentItem = this;
}

void MozaTreeNode::removeChild(int row)
{
    if (row < 0 || row >= m_childItems.size()) { return; }
    m_childItems[row]->m_parentItem = nullptr;
    m_childItems.removeAt(row);
}

MozaTreeNode *MozaTreeNode::child(int row)
{
    if (row < 0 || row >= m_childItems.size()) { return nullptr; }
    return m_childItems.at(row);
}

MozaTreeNode *MozaTreeNode::sibling(int row)
{
    if (!m_parentItem) { return nullptr; }
    return m_parentItem->child(row);
}

int MozaTreeNode::row() const
{
    if (!m_parentItem) { return 0; }
    return m_parentItem->m_childItems.indexOf(const_cast<MozaTreeNode*>(this));
}

MozaTree::MozaTree(MozaTreeNode *root, QObject *parent): QObject(parent), m_root(root)
{
}

}