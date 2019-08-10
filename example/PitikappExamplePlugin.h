#pragma once

#include <list>
#include <map>

#include <QTimer>

#include "PitikappPlugin.h"

class PitikappExamplePlugin : public PitikappPlugin
{
    public:
        PitikappExamplePlugin();

        PitikappModuleInstance *createModuleInstance(const QString &moduleId) override final;

    private:
        void updateStorageList();

        QTimer m_updateStorageTimer;
        PitikappModuleInfo m_processRunTimeModuleInfo;
        PitikappModuleInfo m_counterModuleInfo;
        std::map<QString, std::pair<PitikappModuleInfo, QString>> m_storageModuleInfo;
};
