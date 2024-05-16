//
// Created by Jayce on 2024/3/21.
//

#include "ElementTree.h"

#include <QSet>
#include <QQueue>
#include <QJsonArray>

namespace MOZA::DashboardEditor
{

ElementTree::ElementTree(const QJsonObject& jsonObject, ElementBuilder *builder, QObject *parent): MozaTree(parent)
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
                throw std::logic_error("The same ID exists: " + std::to_string(currentID));
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

ElementTree::ElementTree(MozaTreeNode *root, QObject *parent): MozaTree(root, parent)
{
    if (m_root == nullptr) { return; }
    if (m_root->data().type() != QVariant::Int) {
        throw std::format_error("Wrong data type! ID must be int.");
    }
    QQueue<MozaTreeNode*> q;
    q.push_back(m_root);
    m_uniqueCheck.insert(m_root->data().toInt(), m_root);
    while (!q.empty()) {
        MozaTreeNode *node = q.front();
        q.pop_front();
        for (int i = 0; i < node->childCount(); ++i) {
            auto item = node->child(i);
            if (item != nullptr && item->hasChild()) {
                if (item->data().type() != QVariant::Int) {
                    throw std::format_error("Wrong data type! ID must be int.");
                }
                if (m_uniqueCheck.contains(item->data().toInt())) {
                    throw std::logic_error("The same ID exists: " + std::to_string(item->data().toInt()));
                }
                q.push_back(item);
                m_uniqueCheck.insert(item->data().toInt(), item);
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
        throw std::logic_error("The same ID exists: " + std::to_string(elementID));
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
    m_uniqueCheck.remove(node->data().toInt());
    emit elementRemoved(node->data().toInt());
    delete node;
}

QList<int> ElementTree::getElementListByDFS(bool hasRoot)
{
    QList<int> ans;
    if (!m_root) { return ans; }
    if (hasRoot) { ans.append(m_root->data().toInt()); }
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
            list.append(cur->data().toInt());
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