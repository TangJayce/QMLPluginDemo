//
// Created by Jayce on 2024/3/21.
//

#pragma once

#include <QJsonObject>

namespace MOZA::DashboardEditor
{

class MozaTreeNode
{
public:
    explicit MozaTreeNode(int data);
    ~MozaTreeNode();

    void appendChild(MozaTreeNode *child);
    void insertChild(int index, MozaTreeNode *child);
    void removeChild(int row);
    MozaTreeNode *child(int row);
    MozaTreeNode *sibling(int row);
    int childCount() const;
    int data() const;
    int row() const;
    MozaTreeNode *parentNode();
    bool hasChild() const;
    int inParentIndex() const { return m_inParentIndex; }
    void setInParentIndex(int inParentIndex) { m_inParentIndex = inParentIndex; }

private:
    QList<MozaTreeNode*> m_childItems;
    int m_itemData;
    MozaTreeNode *m_parentItem{};
    int m_inParentIndex{-1};
};

class ElementBuilder
{
public:
    virtual void processElement(int id, const QJsonObject &obj) = 0;
};

class ElementTree: public QObject
{
    Q_OBJECT
public:
    explicit ElementTree(const QJsonObject& jsonObject, ElementBuilder *builder = nullptr, QObject *parent = nullptr);
    explicit ElementTree(MozaTreeNode* root, QObject *parent = nullptr);
    ~ElementTree() override;

    MozaTreeNode* getRoot();
    MozaTreeNode* getNodeByID(int id);
    bool hasNodeID(int id);
    QList<int> getSortedElementList() { return m_uniqueCheck.keys(); }
    QList<int> getElementListByDFS(bool hasRoot = true);
    QList<int> getLeafNodeListByDFS();
    QList<int> getChildrenElements(int id);

    // -1表示插到最后
    void appendElement(int elementID, MozaTreeNode *parent, int index = -1);
    void removeElement(MozaTreeNode *node);

Q_SIGNALS:
    void elementAppended(MozaTreeNode *node);
    void elementRemoved(int elementID);
    void elementReset();
    // todo: 移动节点
    void elementMoved();

private:
    void dfs(QList<int> &list, MozaTreeNode *node, bool onlyLeaf = false);

private:
    MozaTreeNode* m_root{};
    QMap<int, MozaTreeNode*> m_uniqueCheck;
};

}