#ifndef MODULECOUNTER_H
#define MODULECOUNTER_H

#include <QTimer>

#include "PitikappModuleInstance.h"
#include "PitikappModuleInstanceGaugeData.h"

class ModuleCounter : public PitikappModuleInstance
{
    public:
        ModuleCounter();

    private:
        void update();

        int m_counterValue;
        QTimer m_updateTimer;
        PitikappModuleInstanceGaugeData m_data;
};

#endif // MODULECOUNTER_H
