//
// Created by Jayce on 2024/3/22.
//

#pragma once

#include <QObject>
#include "MozaTreeModel.h"

namespace moza::component
{

class MozaEditorFileManager: public QObject
{
    Q_OBJECT
public:
    static MozaEditorFileManager* instance();

    Q_INVOKABLE moza::component::MozaTreeModel* getModelDataFromFile(const QString &filepath);

};

}