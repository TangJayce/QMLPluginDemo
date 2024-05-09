//
// Created by Jayce on 2024/3/21.
//

#include "ElementTree.h"

#include <QSet>
#include <QQueue>
#include <QJsonArray>

namespace MOZA::DashboardEditor
{

MozaTreeNode::MozaTreeNode(int data): m_itemData(data)
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

int MozaTreeNode::data() const
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

ElementTree::ElementTree(const QJsonObject& jsonObject, ElementBuilder *builder, QObject *parent): QObject(parent)
{
    if (jsonObject.isEmpty() || jsonObject.value("id").isUndefined()) { return; }

    int currentID = jsonObject.value("id").toInt();
    m_root = new MozaTreeNode(currentID);
    m_uniqueCheck.insert(currentID, m_root);
    if (builder != nullptr) {
        builder->processElement(currentID, jsonObject);
    }

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
            if (m_uniqueCheck.contains(currentID)) {
                throw std::logic_error("存在相同的ID" + std::to_string(currentID));
            }
            if (builder != nullptr) {
                builder->processElement(currentID, obj);
            }
            auto child = new MozaTreeNode(currentID);
            m_uniqueCheck.insert(currentID, child);
            node->appendChild(child);
            q.push_back({child, obj});
        }
    }
}

ElementTree::ElementTree(MozaTreeNode *root, QObject *parent): QObject(parent), m_root(root)
{
    if (m_root == nullptr) { return; }
    QQueue<MozaTreeNode*> q;
    q.push_back(m_root);
    m_uniqueCheck.insert(m_root->data(), m_root);
    while (!q.empty()) {
        MozaTreeNode *node = q.front();
        q.pop_front();
        for (int i = 0; i < node->childCount(); ++i) {
            auto item = node->child(i);
            if (item != nullptr && item->hasChild()) {
                if (m_uniqueCheck.contains(item->data())) {
                    throw std::logic_error("存在相同的ID" + std::to_string(item->data()));
                }
                q.push_back(item);
                m_uniqueCheck.insert(item->data(), item);
            }
        }
    }
}

ElementTree::~ElementTree()
{
    delete m_root;
}

MozaTreeNode *ElementTree::getRoot()
{
    return m_root;
}

MozaTreeNode *ElementTree::getNodeByID(int id)
{
    return m_uniqueCheck.value(id);
}

bool ElementTree::hasNodeID(int id)
{
    return m_uniqueCheck.contains(id);
}

void ElementTree::appendElement(int elementID, MozaTreeNode *parent, int index)
{
    if (m_uniqueCheck.contains(elementID)) {
        throw std::logic_error("存在相同的ID" + std::to_string(elementID));
    }
    auto node = new MozaTreeNode(elementID);
    if (~index) {
        parent->insertChild(index, node);
    } else {
        parent->appendChild(node);
    }
    m_uniqueCheck.insert(elementID, node);
    emit elementAppended(node);
}

void ElementTree::removeElement(MozaTreeNode *node)
{
    if (node == nullptr) { return; }
    m_uniqueCheck.remove(node->data());
    emit elementRemoved(node->data());
    delete node;
}

QList<int> ElementTree::getElementListByDFS(bool hasRoot)
{
    QList<int> ans;
    if (!m_root) { return ans; }
    if (hasRoot) { ans.append(m_root->data()); }
    dfs(ans, m_root);
    return ans;
}

QList<int> ElementTree::getLeafNodeListByDFS()
{
    QList<int> ans;
    if (!m_root) { return ans; }
    dfs(ans, m_root, true);
    return ans;
}

void ElementTree::dfs(QList<int> &list, MozaTreeNode *node, bool onlyLeaf)
{
    for (int i = 0; i < node->childCount(); ++i) {
        auto cur = node->child(i);
        if (!onlyLeaf || !cur->hasChild()) {
            list.append(cur->data());
        }
        if (cur->hasChild()) {
            dfs(list, cur, onlyLeaf);
        }
    }
}

QList<int> ElementTree::getChildrenElements(int id)
{
    QList<int> ans;
    if (!m_root || !m_uniqueCheck.contains(id)) { return ans; }
    dfs(ans, m_uniqueCheck.value(id));
    return ans;
}

}