//
// Created by Jayce on 2024/5/6.
//

#pragma once

#include <QObject>
#include <QQmlPropertyMap>

namespace MOZA::DashboardEditor
{

class CustomPropertyGroup;

class DashboardStore: public QObject
{
    Q_OBJECT
public:
    explicit DashboardStore(QObject *parent = nullptr): QObject(parent) {}

    Q_INVOKABLE MOZA::DashboardEditor::CustomPropertyGroup* getCustomProperties(int id);
    Q_INVOKABLE QQmlPropertyMap* getInternalProperties(int id);

private:
    QMap<int, CustomPropertyGroup*> m_customProperties;
    QMap<int, QQmlPropertyMap*> m_internalProperties;
};

}