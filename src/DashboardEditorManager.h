//
// Created by Jayce on 2024/3/22.
//

#pragma once

#include <QObject>
#include "MozaTreeModel.h"
#include "DashboardStore.h"

namespace MOZA::DashboardEditor
{

class ElementTree;

class DashboardEditorManager: public QObject
{
    Q_OBJECT
public:
    enum ElementType {
        TEXT,
        IMAGE,
        ELLIPSE,
        BOX,
        TELEMETRY,
        MAP,
        METER
    };
    Q_ENUM(ElementType)

    static DashboardEditorManager* instance();

    Q_INVOKABLE MOZA::DashboardEditor::ElementTree* parseJsonFile(const QString &filepath);
    Q_INVOKABLE MOZA::DashboardEditor::DashboardEditorManager::ElementType getElementType(int id);
    Q_INVOKABLE MOZA::DashboardEditor::DashboardStore* getDashboardStore() { return m_store; }

private:
    DashboardStore* m_store{new DashboardStore(this)};
};

}