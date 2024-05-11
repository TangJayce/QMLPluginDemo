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
    auto children = m_tree->getChildrenElements(elementID);
    for (int item : children) {
        m_store->getCustomProperties(item)->setParentSelected(true);
    }
    m_selectedElements.insert(elementID);
    refreshBoundary();
    emit selectedElementsChanged();
}

bool ElementInteractor::switchSelect(qreal x, qreal y)
{
    int id = elementAtPosition(x, y);
    if (~id) {
        switchSelect(id);
        return true;
    }
    clearSelect();
    return false;
}

bool ElementInteractor::selectByBox(qreal startX, qreal startY, qreal stopX, qreal stopY)
{
    if (!m_tree || !m_store) { return false; }
    clearSelect(false);
    auto list = m_tree->getLeafNodeListByDFS();
    for (int id : list) {
        auto cur = m_store->getCustomProperties(id);
        if (cur->hidden() || cur->parentHidden() || cur->locked() || cur->parentLocked()) { continue; }
        if (cur->isOverlap(startX, startY, stopX, stopY) || cur->isContainedInRectangle(startX, startY, stopX, stopY)) {
            m_selectedElements.insert(id);
            cur->setSelected(true);
            auto children = m_tree->getChildrenElements(id);
            for (int item : children) {
                m_store->getCustomProperties(item)->setParentSelected(true);
            }
        }
    }
    refreshBoundary();
    emit selectedElementsChanged();
    return !m_selectedElements.empty();
}

void ElementInteractor::attachSelect(int elementID)
{
    if (!m_tree || !m_store || elementID <= 0) { return; }
    auto cur = m_store->getCustomProperties(elementID);
    bool curParentSelected;
    if (m_selectedElements.contains(elementID)) {
        cur->disconnect(this);
        cur->setSelected(false);
        curParentSelected = cur->parentSelected();
        m_selectedElements.remove(elementID);
    } else {
        connect(cur, &CustomPropertyGroup::xLowerChanged, this, [this, cur](){ setXLower(cur->xLower()); });
        connect(cur, &CustomPropertyGroup::xUpperChanged, this, [this, cur](){ setXUpper(cur->xUpper()); });
        connect(cur, &CustomPropertyGroup::yLowerChanged, this, [this, cur](){ setYLower(cur->yLower()); });
        connect(cur, &CustomPropertyGroup::yUpperChanged, this, [this, cur](){ setYUpper(cur->yUpper()); });
        cur->setSelected(true);
        curParentSelected = true;
        m_selectedElements.insert(elementID);
    }
    auto list = m_tree->getChildrenElements(elementID);
    for (int item : list) {
        m_store->getCustomProperties(item)->setParentSelected(curParentSelected);
    }
    refreshBoundary();
    emit selectedElementsChanged();
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

void ElementInteractor::refreshBoundary()
{
    if (m_selectedElements.empty()) { return; }
    auto list = m_selectedElements.values();
    auto cur = m_store->getCustomProperties(list[0]);
    qreal xLower = cur->xLower();
    qreal xUpper = cur->xUpper();
    qreal yLower = cur->yLower();
    qreal yUpper = cur->yUpper();
    for (int i = 1; i < list.size(); ++i) {
        cur = m_store->getCustomProperties(list[i]);
        xLower = qMin(xLower, cur->xLower());
        xUpper = qMax(xUpper, cur->xUpper());
        yLower = qMin(yLower, cur->yLower());
        yUpper = qMax(yUpper, cur->yUpper());
    }
    setXLower(xLower);
    setXUpper(xUpper);
    setYLower(yLower);
    setYUpper(yUpper);
}

int ElementInteractor::elementAtPosition(qreal x, qreal y)
{
    if (!m_tree || !m_store) { return -1; }
    auto list = m_tree->getLeafNodeListByDFS();
    auto p = QPointF(x, y);
    for (int id : list) {
        auto cur = m_store->getCustomProperties(id);
        if (cur->hidden() || cur->parentHidden() || cur->locked() || cur->parentLocked()) { continue; }
        if (cur->isContainPoint(p)) {
            return id;
        }
    }
    return -1;
}

}
