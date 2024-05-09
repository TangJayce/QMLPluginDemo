//
// Created by Jayce on 2024/4/25.
//

#pragma once

#include <QAbstractListModel>
#include <QList>

namespace MOZA::DashboardEditor
{

class ElementTree;
class MozaTreeNode;

class ArtBoardListModel: public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(MOZA::DashboardEditor::ElementTree* tree READ tree WRITE setTree)
public:
    enum ElementListModelRoles {
        ELEMENT_ID = Qt::UserRole + 1,
        HAS_CHILD
    };
public:
    explicit ArtBoardListModel(QObject *parent = nullptr);

    QHash<int, QByteArray> roleNames() const override { return m_roleNames; }
    int rowCount(const QModelIndex &parent) const override { return m_elements.size(); }
    QVariant data(const QModelIndex &index, int role) const override;

    ElementTree* tree() const noexcept { return m_tree; }
    void setTree(ElementTree *tree);
private:
    int itemIndex(int elementID) const;
    void appendElement(MozaTreeNode *node);
    void removeElement(int elementID);
    void resetElements();
private:
    ElementTree *m_tree{};
    QHash<int, QByteArray> m_roleNames;
    QList<int> m_elements;
    mutable int m_lastItemIndex{};
};

}