//
// Created by Jayce on 2024/5/16.
//

#pragma once

#include <QVariant>

namespace MOZA::DashboardEditor
{

class MozaTreeNode
{
public:
    explicit MozaTreeNode(QVariant data);
    ~MozaTreeNode();

    void appendChild(MozaTreeNode *child);
    void insertChild(int index, MozaTreeNode *child);
    void removeChild(int row);
    MozaTreeNode *child(int row);
    MozaTreeNode *sibling(int row);
    int row() const;
    int childCount() const { return m_childItems.count(); }
    const QVariant& data() const noexcept { return m_itemData; }
    MozaTreeNode *parentNode() const noexcept { return m_parentItem; }
    bool hasChild() const { return !m_childItems.isEmpty(); }

private:
    QList<MozaTreeNode*> m_childItems;
    QVariant m_itemData;
    MozaTreeNode *m_parentItem{};
};

class MozaTree: public QObject
{
    Q_OBJECT
public:
    explicit MozaTree(QObject *parent = nullptr): QObject(parent) {}
    explicit MozaTree(MozaTreeNode* root, QObject *parent = nullptr);

    MozaTreeNode* root() const noexcept { return m_root; }

protected:
    MozaTreeNode* m_root{};
};

}