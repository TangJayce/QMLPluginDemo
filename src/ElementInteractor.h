//
// Created by Jayce on 2024/4/26.
//

#pragma once

#include <QObject>
#include <QSet>
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
    Q_PROPERTY(QList<int> selectedElements READ selectedElements NOTIFY selectedElementsChanged)

    Q_PROPERTY(qreal xLower READ xLower WRITE setXLower NOTIFY xLowerChanged)
    Q_PROPERTY(qreal xUpper READ xUpper WRITE setXUpper NOTIFY xUpperChanged)
    Q_PROPERTY(qreal yLower READ yLower WRITE setYLower NOTIFY yLowerChanged)
    Q_PROPERTY(qreal yUpper READ yUpper WRITE setYUpper NOTIFY yUpperChanged)
public:
    explicit ElementInteractor(QObject *parent = nullptr);

    ElementTree* tree() const noexcept { return m_tree; }
    void setTree(ElementTree *tree);
    DashboardStore* store() const noexcept { return m_store; }
    void setStore(DashboardStore *store);
    QList<int> selectedElements() const { return m_selectedElements.values(); }
    qreal xLower() const noexcept { return m_xLower; }
    void setXLower(qreal xLower);
    qreal xUpper() const noexcept { return m_xUpper; }
    void setXUpper(qreal xUpper);
    qreal yLower() const noexcept { return m_yLower; }
    void setYLower(qreal yLower);
    qreal yUpper() const noexcept { return m_yUpper; }
    void setYUpper(qreal yUpper);

    Q_INVOKABLE void switchSelect(int elementID);
    Q_INVOKABLE bool switchSelect(qreal x, qreal y);
    Q_INVOKABLE bool selectByBox(qreal startX, qreal startY, qreal stopX, qreal stopY);
    Q_INVOKABLE void attachSelect(int elementID);
    Q_INVOKABLE void clearSelect(bool notify = true);

    Q_INVOKABLE void refreshBoundary();
    Q_INVOKABLE void toggleHide(int elementID);
    Q_INVOKABLE void toggleLock(int elementID);
    Q_INVOKABLE int elementAtPosition(qreal x, qreal y);
    Q_INVOKABLE bool isElementInSelectedList(int elementID) const { return m_selectedElements.contains(elementID); }

Q_SIGNALS:
    void treeChanged();
    void selectedElementsChanged();
    void storeChanged();
    void xLowerChanged();
    void xUpperChanged();
    void yLowerChanged();
    void yUpperChanged();


private:
    ElementTree *m_tree{};
    DashboardStore *m_store{};
    QSet<int> m_selectedElements;

    qreal m_xLower{};
    qreal m_xUpper{};
    qreal m_yLower{};
    qreal m_yUpper{};
};

}