//
// Created by Jayce on 2024/3/20.
//

#pragma once

#include <QAbstractListModel>
#include <QSet>

namespace moza::component
{

class MozaTree;
class MozaTreeNode;

// 使用ListModel实现树状效果
class MozaTreeModel: public QAbstractListModel
{
    Q_OBJECT
    // 框选项索引
    Q_PROPERTY(int frameIndex READ frameIndex WRITE setFrameIndex NOTIFY frameIndexChanged FINAL)
public:
    enum SelectStatus {
        NONE_SELECT,
        SELECTED,
        SUB_SELECTED
    };
    Q_ENUM(SelectStatus)

public:
    MozaTreeModel() = default;
    explicit MozaTreeModel(MozaTree* tree, QObject *parent);
    ~MozaTreeModel() override;

    QHash<int, QByteArray> roleNames() const override;
    QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;

    int frameIndex() const;
    void setFrameIndex(int n);

    Q_INVOKABLE void expandRow(int n);
    Q_INVOKABLE void collapseRow(int n);
    Q_INVOKABLE void switchSelect(int n);
    Q_INVOKABLE void attachSelect(int n);
    Q_INVOKABLE void multipleSelect(int prev, int cur);
    Q_INVOKABLE int dragGapIndex(int idx);
    Q_INVOKABLE int itemIndexDepth(int itemIdx);
    Q_INVOKABLE void moveSingleItem(int itemIdx, int gapIdx);

    int itemIndex(const MozaTreeNode *node) const;
    int countVisibleItem(MozaTreeNode *node) const;
    int lastChildIndex(MozaTreeNode *node) const;

Q_SIGNALS:
    void frameIndexChanged();

private:
    enum TreeModelRoles {
        VALUE = Qt::UserRole + 1,
        DEPTH,
        EXPANDED,
        HAS_CHILD,
        SELECTED_STATUS,
        HAS_TOP_RADIUS,
        HAS_BOTTOM_RADIUS
    };
    struct TreeItem {
        MozaTreeNode* value;
        int depth;
        bool expanded;
        SelectStatus selectedStatus;
    };
    void dfs(MozaTreeNode *item, int depth);
    void showModelChildItems(const TreeItem &item, int &startIdx);

private:
    QHash<int, QByteArray> m_roleNames;
    MozaTreeNode* m_rootNode{};
    QList<TreeItem> m_currentItems;
    QSet<MozaTreeNode*> m_expandedItems;
    QSet<MozaTreeNode*> m_selectedItems;
    mutable int m_lastItemIndex{};
    int m_frameIndex{-1};
};

}