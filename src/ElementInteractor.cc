//
// Created by Jayce on 2024/4/26.
//

#include "ElementInteractor.h"
#include "DashboardEditorManager.h"
#include "ElementTree.h"
#include "CustomPropertyGroup.h"

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

void ElementInteractor::setXLower(qreal xLower)
{
    if (m_xLower == xLower) { return; }
    m_xLower = xLower;
    emit xLowerChanged();
}

void ElementInteractor::setXUpper(qreal xUpper)
{
    if (m_xUpper == xUpper) { return; }
    m_xUpper = xUpper;
    emit xUpperChanged();
}

void ElementInteractor::setYLower(qreal yLower)
{
    if (m_yLower == yLower) { return; }
    m_yLower = yLower;
    emit yLowerChanged();
}

void ElementInteractor::setYUpper(qreal yUpper)
{
    if (m_yUpper == yUpper) { return; }
    m_yUpper = yUpper;
    emit yUpperChanged();
}

void ElementInteractor::clearSelect(bool notify)
{
    for (int id : m_selectedElements) {
        auto cur = m_store->getCustomProperties(id);
        cur->disconnect(this);
        cur->setSelected(false);
        auto curParentSelected = cur->parentSelected();
        auto list = m_tree->getChildrenElements(id);
        for (int item : list) {
            m_store->getCustomProperties(item)->setParentSelected(curParentSelected);
        }
    }
    m_selectedElements.clear();
    if (notify) { emit selectedElementsChanged(); }
}

void ElementInteractor::switchSelect(int elementID)
{
    if (!m_tree || !m_store) { return; }
    if (m_selectedElements.size() == 1 && m_selectedElements.contains(elementID)) { return; }
    clearSelect(false);
    auto cur = m_store->getCustomProperties(elementID);
    connect(cur, &CustomPropertyGroup::xLowerChanged, this, [this, cur](){ setXLower(cur->xLower()); });
    connect(cur, &CustomPropertyGroup::xUpperChanged, this, [this, cur](){ setXUpper(cur->xUpper()); });
    connect(cur, &CustomPropertyGroup::yLowerChanged, this, [this, cur](){ setYLower(cur->yLower()); });
    connect(cur, &CustomPropertyGroup::yUpperChanged, this, [this, cur](){ setYUpper(cur->yUpper()); });
    cur->setSelected(true);
    auto list = m_tree->getChildrenElements(elementID);
    for (int item : list) {
        m_store->getCustomProperties(item)->setParentSelected(true);
    }
    m_selectedElements.insert(elementID);
    emit selectedElementsChanged();
}

void ElementInteractor::attachSelect(int elementID)
{

}

void ElementInteractor::toggleHide(int elementID)
{
    auto curProps = m_store->getCustomProperties(elementID);
    auto curHidden = curProps->hidden();
    curProps->setHidden(!curHidden);

    auto list = m_tree->getChildrenElements(elementID);
    for (int item : list) {
        m_store->getCustomProperties(item)->setParentHidden(!curHidden);
    }
}

void ElementInteractor::toggleLock(int elementID)
{
    auto curProps = m_store->getCustomProperties(elementID);
    auto curLocked = curProps->locked();
    curProps->setLocked(!curLocked);

    auto list = m_tree->getChildrenElements(elementID);
    for (int item : list) {
        m_store->getCustomProperties(item)->setParentLocked(!curLocked);
    }
}

}