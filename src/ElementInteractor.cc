//
// Created by Jayce on 2024/4/26.
//

#include "ElementInteractor.h"
#include "DashboardEditorManager.h"
#include "ElementTree.h"

namespace MOZA::DashboardEditor
{

ElementInteractor::ElementInteractor(QObject *parent)
    : QObject(parent)
{
}

void ElementInteractor::setTree(ElementTree *tree)
{
    if (m_tree == tree) { return; }
    m_tree = tree;
    emit treeChanged();
}

void ElementInteractor::setStore(DashboardStore *store)
{
    if (m_store == store) { return; }
    m_store = store;
    emit storeChanged();
}

void ElementInteractor::setSelectedElements(const QList<int>& elements)
{
    if (!m_tree || !m_store) { return; }
    for (int id : m_selectedElements) {
        m_store->getProperties(id)->insert("selected", false);
        auto list = m_tree->getChildrenElements(id);
        for (int item : list) {
            m_store->getProperties(item)->insert("parentSelected", false);
        }
    }
    for (int id : elements) {
        m_store->getProperties(id)->insert("selected", true);
        auto list = m_tree->getChildrenElements(id);
        for (int item : list) {
            m_store->getProperties(item)->insert("parentSelected", true);
        }
    }
    m_selectedElements = elements;
    emit selectedElementsChanged();
}

}