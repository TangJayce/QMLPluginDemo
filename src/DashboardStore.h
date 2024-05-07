//
// Created by Jayce on 2024/5/6.
//

#pragma once

#include <QObject>
#include <QQmlPropertyMap>

namespace MOZA::DashboardEditor
{

class DashboardStore: public QObject
{
    Q_OBJECT
public:
    explicit DashboardStore(QObject *parent = nullptr): QObject(parent) {}

    Q_INVOKABLE QQmlPropertyMap* getProperties(int id);

private:
    QMap<int, QQmlPropertyMap*> m_elements;
};

}