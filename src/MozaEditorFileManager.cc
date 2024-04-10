//
// Created by Jayce on 2024/3/22.
//

#include "MozaEditorFileManager.h"

#include <QFile>
#include <QDebug>
#include <QJsonParseError>
#include <QJsonDocument>
#include "MozaTree.h"

namespace moza::component
{

MozaEditorFileManager *MozaEditorFileManager::instance()
{
    static auto obj = new MozaEditorFileManager;
    return obj;
}

MozaTreeModel* MozaEditorFileManager::getModelDataFromFile(const QString &filepath)
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

    return new MozaTreeModel(new MozaTree(doc.object()), this);
}

}