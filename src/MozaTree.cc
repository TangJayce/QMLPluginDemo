//
// Created by Jayce on 2024/3/21.
//

#include "MozaTree.h"

#include <QSet>
#include <QQueue>
#include <QJsonArray>

namespace moza::component
{

MozaTreeNode::MozaTreeNode(QVariant data)
    : m_itemData(std::move(data))
{
}

MozaTreeNode::~MozaTreeNode()
{
    qDeleteAll(m_childItems);
}

void MozaTreeNode::appendChild(MozaTreeNode *child)
{
    if (child == nullptr) { return; }
    child->setInParentIndex(m_childItems.size());
    m_childItems.append(child);
    child->m_parentItem = this;
}

void MozaTreeNode::insertChild(int index, MozaTreeNode *child)
{
    if (child == nullptr || index < 0 || index > m_childItems.size()) { return; }
    m_childItems.insert(index, child);
    child->setInParentIndex(index);
    child->m_parentItem = this;
    for (int i = index + 1; i < m_childItems.size(); ++i) {
        m_childItems[i]->setInParentIndex(i);
    }
}

void MozaTreeNode::removeChild(int row)
{
    if (row < 0 || row >= m_childItems.size()) { return; }
    m_childItems[row]->setInParentIndex(-1);
    m_childItems[row]->m_parentItem = nullptr;
    m_childItems.removeAt(row);
    for (int i = row; i < m_childItems.size(); ++i) {
        m_childItems[i]->setInParentIndex(i);
    }
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

int MozaTreeNode::childCount() const
{
    return m_childItems.count();
}

QVariant MozaTreeNode::data() const
{
    return m_itemData;
}

int MozaTreeNode::row() const
{
    if (!m_parentItem) { return 0; }
    return m_parentItem->m_childItems.indexOf(const_cast<MozaTreeNode*>(this));
}

MozaTreeNode *MozaTreeNode::parentNode()
{
    return m_parentItem;
}

bool MozaTreeNode::hasChild() const
{
    return !m_childItems.isEmpty();
}

MozaTree::MozaTree(const QJsonObject& jsonObject)
{
    if (jsonObject.isEmpty() || jsonObject.value("id").isUndefined()) { return; }

    QSet<int> uniqueCheck;
    int currentID = jsonObject.value("id").toInt();
    uniqueCheck.insert(currentID);
    m_root = new MozaTreeNode(currentID);

    QQueue<QPair<MozaTreeNode*, QJsonObject>> q;
    q.push_back({m_root, jsonObject});
    while (!q.isEmpty()) {
        MozaTreeNode *node = q.front().first;
        QJsonValue children = q.front().second.value("children");
        q.pop_front();
        if (!children.isArray()) { continue; }
        auto list = children.toArray();
        for (const auto &item : qAsConst(list)) {
            if (!item.isObject()) { continue; }
            auto obj = item.toObject();
            if (obj.isEmpty() || obj.value("id").isUndefined()) { continue; }
            currentID = obj.value("id").toInt();
            if (uniqueCheck.contains(currentID)) {
                throw std::logic_error("存在相同的ID");
            }
            uniqueCheck.insert(currentID);
            auto child = new MozaTreeNode(currentID);
            node->appendChild(child);
            q.push_back({child, obj});
        }
    }
}

MozaTree::MozaTree(MozaTreeNode *root): m_root(root)
{
}

MozaTreeNode *MozaTree::getRoot()
{
    return m_root;
}

}