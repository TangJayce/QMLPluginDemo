//
// Created by Jayce on 2024/5/8.
//

#include "CustomPropertyGroup.h"
#include <QtMath>

namespace MOZA::DashboardEditor
{

void CustomPropertyGroup::setSelected(bool newSelected)
{
    if (m_selected == newSelected) { return; }
    m_selected = newSelected;
    emit selectedChanged();
}

void CustomPropertyGroup::setParentSelected(bool newParentSelected)
{
    if (m_parentSelected == newParentSelected) { return; }
    m_parentSelected = newParentSelected;
    emit parentSelectedChanged();
}

void CustomPropertyGroup::setHidden(bool newHidden)
{
    if (m_hidden == newHidden) { return; }
    m_hidden = newHidden;
    emit hiddenChanged();
}

void CustomPropertyGroup::setParentHidden(bool newParentHidden)
{
    if (m_parentHidden == newParentHidden) { return; }
    m_parentHidden = newParentHidden;
    emit parentHiddenChanged();
}

void CustomPropertyGroup::setLocked(bool newLocked)
{
    if (m_locked == newLocked) { return; }
    m_locked = newLocked;
    emit lockedChanged();
}

void CustomPropertyGroup::setParentLocked(bool newParentLocked)
{
    if (m_parentLocked == newParentLocked) { return; }
    m_parentLocked = newParentLocked;
    emit parentLockedChanged();
}

void CustomPropertyGroup::setP1(QPointF newP1)
{
    if (m_p1 == newP1) { return; }
    m_p1 = newP1;
    emit p1Changed();
}

void CustomPropertyGroup::setP2(QPointF newP2)
{
    if (m_p2 == newP2) { return; }
    m_p2 = newP2;
    emit p2Changed();
}

void CustomPropertyGroup::setP3(QPointF newP3)
{
    if (m_p3 == newP3) { return; }
    m_p3 = newP3;
    emit p3Changed();
}

void CustomPropertyGroup::setP4(QPointF newP4)
{
    if (m_p4 == newP4) { return; }
    m_p4 = newP4;
    emit p4Changed();
}

void CustomPropertyGroup::setXLower(qreal newXLower)
{
    if (qFuzzyCompare(m_xLower, newXLower)) { return; }
    m_xLower = newXLower;
    emit xLowerChanged();
}

void CustomPropertyGroup::setXUpper(qreal newXUpper)
{
    if (qFuzzyCompare(m_xUpper, newXUpper)) { return; }
    m_xUpper = newXUpper;
    emit xUpperChanged();
}

void CustomPropertyGroup::setYLower(qreal newYLower)
{
    if (qFuzzyCompare(m_yLower, newYLower)) { return; }
    m_yLower = newYLower;
    emit yLowerChanged();
}

void CustomPropertyGroup::setYUpper(qreal newYUpper)
{
    if (qFuzzyCompare(m_yUpper, newYUpper)) { return; }
    m_yUpper = newYUpper;
    emit yUpperChanged();
}

void CustomPropertyGroup::resetBoundary(qreal x, qreal y, qreal w, qreal h, int angle)
{
    qreal arc = angle * M_PI / 180;
    auto p1 = QPointF(x+w/2*(1-qCos(arc))+h/2*qSin(arc), y+h/2*(1-qCos(arc))-w/2*qSin(arc));
    auto p2 = QPointF(w*qCos(arc)+p1.x(), w*qSin(arc)+p1.y());
    auto p3 = QPointF(w*qCos(arc)-h*qSin(arc)+p1.x(), w*qSin(arc)+h*qCos(arc)+p1.y());
    auto p4 = QPointF(-h*qSin(arc)+p1.x(), h*qCos(arc)+p1.y());
    setP1(p1);
    setP2(p2);
    setP3(p3);
    setP4(p4);

    setXLower(std::min({p1.x(), p2.x(), p3.x(), p4.x()}));
    setXUpper(std::max({p1.x(), p2.x(), p3.x(), p4.x()}));
    setYLower(std::min({p1.y(), p2.y(), p3.y(), p4.y()}));
    setYUpper(std::max({p1.y(), p2.y(), p3.y(), p4.y()}));
}

}
