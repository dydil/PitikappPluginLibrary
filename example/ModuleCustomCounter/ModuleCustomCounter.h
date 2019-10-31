#pragma once

#include <QTimer>

#include "PitikappModuleInstance.h"
#include "ModuleCustomCounterData.h"

class ModuleCustomCounter : public QObject, public PitikappModuleInstance
{
    Q_OBJECT

    public:
        ModuleCustomCounter();

    private:
        void onCounterMaxValueChanged(int value);
        void update();

        int m_counterValue;
        QTimer m_updateTimer;
        ModuleCustomCounterData m_data;
};
