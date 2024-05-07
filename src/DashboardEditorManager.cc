//
// Created by Jayce on 2024/3/22.
//

#include "DashboardEditorManager.h"

#include <QFile>
#include <QDebug>
#include <QJsonParseError>
#include <QJsonDocument>
#include "ElementTree.h"

namespace MOZA::DashboardEditor
{

DashboardEditorManager *DashboardEditorManager::instance()
{
    static auto obj = new DashboardEditorManager;
    return obj;
}

ElementTree* DashboardEditorManager::parseJsonFile(const QString &filepath)
{
    QFile file(filepath);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "打开文件失败";
        return {};
    }

    QJsonParseError jerr{};
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &jerr);
    if (doc.isNull()) {
        qDebug().noquote() << QString("转换json文件失败：%1").arg(jerr.errorString());
        return {};
    }
    if (!doc.isObject()) {
        qDebug() << "json文件格式错误, 必须以window为根节点";
        return {};
    }

    return new ElementTree(doc.object());
}

DashboardEditorManager::ElementType DashboardEditorManager::getElementType(int id)
{
    if (id < ElementType::TEXT || id > ElementType::METER) { return (ElementType)(id % (ElementType::METER + 1)); }
    return (ElementType)id;
}

}