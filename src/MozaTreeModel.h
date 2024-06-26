//
// Created by Jayce on 2024/3/20.
//

#pragma once

#include <QAbstractListModel>
#include <QSet>

namespace MOZA::DashboardEditor
{

class MozaTree;
class MozaTreeNode;

// 使用ListModel实现树状效果
class MozaTreeModel: public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(MOZA::DashboardEditor::MozaTree* tree READ tree WRITE setTree NOTIFY treeChanged)
public:
    explicit MozaTreeModel(QObject *parent = nullptr);

    QHash<int, QByteArray> roleNames() const override { return m_roleNames; }
    int rowCount(const QModelIndex &parent) const override { return m_currentItems.size(); }
    QVariant data(const QModelIndex &index, int role) const override;

    MozaTree* tree() const noexcept { return m_tree; }
    void setTree(MozaTree *tree);

    Q_INVOKABLE void expandRow(int n);
    Q_INVOKABLE void collapseRow(int n);
    Q_INVOKABLE int itemIndexDepth(int n);
//    Q_INVOKABLE int dragGapIndex(int idx);

//    Q_INVOKABLE void switchSelect(int n);
//    Q_INVOKABLE void attachSelect(int n);
//    Q_INVOKABLE void multipleSelect(int prev, int cur);
//    Q_INVOKABLE void moveSingleItem(int itemIdx, int gapIdx);

    int itemIndex(MozaTreeNode* item) const;
    int countVisibleItem(MozaTreeNode *node) const;
    int lastChildIndex(MozaTreeNode *node) const;

Q_SIGNALS:
    void treeChanged();

protected:
    enum TreeModelRoles {
        VALUE = Qt::UserRole + 1,
        DEPTH,
        EXPANDED,
        HAS_CHILD
    };
    struct TreeItem {
        MozaTreeNode* value;
        int depth;
        bool expanded;
    };
    void dfs(MozaTreeNode *item, int depth);
    void showModelChildItems(const TreeItem &item, int &startIdx);

protected:
    MozaTree* m_tree{};
    QHash<int, QByteArray> m_roleNames;
    QList<TreeItem> m_currentItems;
    QSet<MozaTreeNode*> m_expandedItems;
    mutable int m_lastItemIndex{};
};

}