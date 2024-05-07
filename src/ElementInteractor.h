//
// Created by Jayce on 2024/4/26.
//

#pragma once

#include <QObject>
#include <QList>

namespace MOZA::DashboardEditor
{

class ElementTree;
class DashboardStore;

class ElementInteractor: public QObject
{
    Q_OBJECT
    Q_PROPERTY(MOZA::DashboardEditor::ElementTree* tree READ tree WRITE setTree NOTIFY treeChanged)
    Q_PROPERTY(MOZA::DashboardEditor::DashboardStore* store READ store WRITE setStore NOTIFY storeChanged)
    Q_PROPERTY(QList<int> selectedElements READ selectedElements WRITE setSelectedElements NOTIFY selectedElementsChanged)
public:
    explicit ElementInteractor(QObject *parent = nullptr);

    ElementTree* tree() const noexcept { return m_tree; }
    void setTree(ElementTree *tree);
    DashboardStore* store() const noexcept { return m_store; }
    void setStore(DashboardStore *store);
    QList<int> selectedElements() const noexcept { return m_selectedElements; }
    void setSelectedElements(const QList<int>& elements);

Q_SIGNALS:
    void treeChanged();
    void selectedElementsChanged();
    void storeChanged();

private:
    ElementTree *m_tree{};
    DashboardStore *m_store{};
    QList<int> m_selectedElements;
};

}