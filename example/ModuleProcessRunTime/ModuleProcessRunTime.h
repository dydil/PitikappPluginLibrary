#pragma once

#include <Windows.h>
#include <Winuser.h>

#include <QTimer>

#include "PitikappModuleInstance.h"
#include "ModuleProcessRunTimeData.h"

class ModuleProcessRunTime : public PitikappModuleInstance
{
    public:
        ModuleProcessRunTime();

        void processData(const QVariantMap &data) override;

    private:
        void update();

        ModuleProcessRunTimeData m_data;
        QTimer m_updateTimer;
};
