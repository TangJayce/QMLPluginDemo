//
// Created by Jayce on 2024/3/21.
//

#pragma once

#include <QJsonObject>

namespace moza::component
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
    int childCount() const;
    QVariant data() const;
    int row() const;
    MozaTreeNode *parentNode();
    bool hasChild() const;
    int inParentIndex() const { return m_inParentIndex; }
    void setInParentIndex(int inParentIndex) { m_inParentIndex = inParentIndex; }

private:
    QList<MozaTreeNode*> m_childItems;
    QVariant m_itemData;
    MozaTreeNode *m_parentItem{};
    int m_inParentIndex{-1};
};

class MozaTree
{
public:
    explicit MozaTree(const QJsonObject& jsonObject);
    explicit MozaTree(MozaTreeNode* root);

    MozaTreeNode* getRoot();

private:
    MozaTreeNode* m_root{};
};

}