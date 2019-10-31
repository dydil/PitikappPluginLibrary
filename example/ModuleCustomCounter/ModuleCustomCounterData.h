#pragma once

#include "PitikappModuleInstanceGaugeData.h"
#include "PitikappModuleDataInfo.h"

class ModuleCustomCounterData : public QObject, public PitikappModuleInstanceGaugeData
{
    Q_OBJECT

    public:
        ModuleCustomCounterData();

        static PitikappModuleDataInfo ModuleDataInfo();

        int getCounterMaxValue() const;

    signals:
        void counterMaxValueChanged(int value);

    private:
        void onLocalParameterChanged(const QString &key, const QVariant &value);

};
