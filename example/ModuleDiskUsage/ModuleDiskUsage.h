#pragma once

#include <QTimer>

#include "PitikappModuleInstance.h"
#include "PitikappModuleInstanceGaugeData.h"

class ModuleDiskUsage : public PitikappModuleInstance
{
    public:
        ModuleDiskUsage(const QString &rootPath);

    private:
        void updateUsage();

        QTimer m_updateTimer;
        PitikappModuleInstanceGaugeData m_data;
        QString m_rootPath;;
};
