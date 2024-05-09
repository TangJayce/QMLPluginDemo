//
// Created by Jayce on 2024/5/8.
//

#pragma once
#include <QObject>
#include <QPointF>

namespace MOZA::DashboardEditor
{

class CustomPropertyGroup: public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool selected READ selected NOTIFY selectedChanged)
    Q_PROPERTY(bool parentSelected READ parentSelected NOTIFY parentSelectedChanged)
    Q_PROPERTY(bool hidden READ hidden NOTIFY hiddenChanged)
    Q_PROPERTY(bool parentHidden READ parentHidden NOTIFY parentHiddenChanged)
    Q_PROPERTY(bool locked READ locked NOTIFY lockedChanged)
    Q_PROPERTY(bool parentLocked READ parentLocked NOTIFY parentLockedChanged)
    Q_PROPERTY(QPointF p1 READ p1 NOTIFY p1Changed)
    Q_PROPERTY(QPointF p2 READ p2 NOTIFY p2Changed)
    Q_PROPERTY(QPointF p3 READ p3 NOTIFY p3Changed)
    Q_PROPERTY(QPointF p4 READ p4 NOTIFY p4Changed)
    Q_PROPERTY(qreal xLower READ xLower NOTIFY xLowerChanged)
    Q_PROPERTY(qreal xUpper READ xUpper NOTIFY xUpperChanged)
    Q_PROPERTY(qreal yLower READ yLower NOTIFY yLowerChanged)
    Q_PROPERTY(qreal yUpper READ yUpper NOTIFY yUpperChanged)
public:
    explicit CustomPropertyGroup(QObject *parent = nullptr): QObject(parent) {}

    void resetBoundary(qreal x, qreal y, qreal w, qreal h, int angle);

    bool selected() const noexcept { return m_selected; }
    bool parentSelected() const noexcept { return m_parentSelected; }
    bool hidden() const noexcept { return m_hidden; }
    bool parentHidden() const noexcept { return m_parentHidden; }
    bool locked() const noexcept { return m_locked; }
    bool parentLocked() const noexcept { return m_parentLocked; }
    QPointF p1() const noexcept { return m_p1; }
    QPointF p2() const noexcept { return m_p2; }
    QPointF p3() const noexcept { return m_p3; }
    QPointF p4() const noexcept { return m_p4; }
    qreal xLower() const noexcept { return m_xLower; }
    qreal xUpper() const noexcept { return m_xUpper; }
    qreal yLower() const noexcept { return m_yLower; }
    qreal yUpper() const noexcept { return m_yUpper; }

    void setSelected(bool newSelected);
    void setParentSelected(bool newParentSelected);
    void setHidden(bool newHidden);
    void setParentHidden(bool newParentHidden);
    void setLocked(bool newLocked);
    void setParentLocked(bool newParentLocked);
    void setP1(QPointF newP1);
    void setP2(QPointF newP2);
    void setP3(QPointF newP3);
    void setP4(QPointF newP4);
    void setXLower(qreal newXLower);
    void setXUpper(qreal newXUpper);
    void setYLower(qreal newYLower);
    void setYUpper(qreal newYUpper);

signals:
    void selectedChanged();
    void parentSelectedChanged();
    void hiddenChanged();
    void parentHiddenChanged();
    void lockedChanged();
    void parentLockedChanged();
    void p1Changed();
    void p2Changed();
    void p3Changed();
    void p4Changed();
    void xLowerChanged();
    void xUpperChanged();
    void yLowerChanged();
    void yUpperChanged();

private:
    bool m_selected{};
    bool m_parentSelected{};
    bool m_hidden{};
    bool m_parentHidden{};
    bool m_locked{};
    bool m_parentLocked{};
    QPointF m_p1;
    QPointF m_p2;
    QPointF m_p3;
    QPointF m_p4;
    qreal m_xLower{};
    qreal m_xUpper{};
    qreal m_yLower{};
    qreal m_yUpper{};
};

}
